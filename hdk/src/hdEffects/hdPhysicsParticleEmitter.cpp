/*
 *  hdPhysicsParticleEmitter.cpp
 *  hdGameEngine
 *
 *  Created by david on 15/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdPhysicsParticleEmitter.h"

hdPhysicsParticle::hdPhysicsParticle(hdGameWorld *m_gameWorld) : hdPolygon(m_gameWorld)
{
	m_physicsBody = NULL;
	life = 0.0f;
	m_texture = NULL;
	m_depth = 0.0f;
	m_duration = 0.0f;
	m_userType = kPhysicsParticleUserType;
	
	m_isTouchable = true;
	m_interacts = true;
	
	((hdPolygon *)this)->Init(m_gameWorld); 
}


void hdPhysicsParticle::CreatePhysicsBody()
{
}


hdPhysicsParticle::~hdPhysicsParticle() {}


void hdPhysicsParticle::DestroyPhysicsBody(b2World *physicsWorld)
{
	hdAssert(physicsWorld != NULL);
	
	if (this->m_physicsBody != NULL)
	{
		physicsWorld->DestroyBody(this->m_physicsBody);
		this->m_physicsBody = NULL;
	}
}


void hdPhysicsParticle::SetCategoryBits(b2World *physicsWorld, int bits)
{
	if (m_physicsBody == NULL) return;
	if (m_physicsBody->GetShapeList() == NULL) return;
	
	b2FilterData data = this->m_physicsBody->GetShapeList()->GetFilterData();
	
	// We don't want to call Refilter unless absolutely necessary.
	if (data.categoryBits == bits) return;
	
	data.categoryBits = bits;
	data.maskBits |= bits;
	this->m_physicsBody->GetShapeList()->SetFilterData(data);
	physicsWorld->Refilter(this->m_physicsBody->GetShapeList());
}


bool hdPhysicsParticle::AABBIntersection(const hdAABB& aabb)
{
	hdAABB depthAABB;
	depthAABB.lower.Set(m_aabb.lower.x, m_aabb.lower.y, m_aabb.lower.z + m_depth);
	depthAABB.upper.Set(m_aabb.upper.x, m_aabb.upper.y, m_aabb.upper.z - m_depth);
	return hdAABBIntersection(aabb, depthAABB);
}


bool hdPhysicsParticle::RayIntersection(const hdVec3* ray, hdVec3& vOut)
{
	if (this->GetVertexCount() < 3) return false;
	
	hdVec3* vertices = this->GetVertices();
	hdVec3 triangle[3];
	int i1, i2;
    
	static hdVec3 verticesWithDepth[256];
	
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		verticesWithDepth[i].x = this->GetVertices()[i].x;
		verticesWithDepth[i].y = this->GetVertices()[i].y;
		verticesWithDepth[i].z = this->GetVertices()[i].z - m_depth;
	}
	
	if (hdRayIntersectsVertices(verticesWithDepth, this->GetVertexCount(), this->GetWorldCenter(), ray, vOut)) return true;
	
	// quad strip
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		i1 = i;
		i2 = (i+1)%(this->GetVertexCount());
		
		triangle[0].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z - m_depth);
		triangle[1].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z + m_depth);
		triangle[2].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z - m_depth);
		
		if (hdRayIntersectsTriangle(triangle, ray, vOut)) return true;
		
		triangle[0].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z + m_depth);
		triangle[1].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z - m_depth);
		triangle[2].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z + m_depth);
		
		if (hdRayIntersectsTriangle(triangle, ray, vOut)) return true;
	}
	
	return false; 
}


void hdPhysicsParticle::Step()
{	
	if (m_physicsBody == NULL) return;
	b2XForm xform = m_physicsBody->GetXForm();
	
	// Translate to origin, rotate, translate back again.
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
	
	hdRotateVertices(this->GetVertices(), this->GetVertexCount(), hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()) - m_obb.transform.rotation);
	
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (hdVec3(xform.position.x, xform.position.y, this->GetWorldCenter().z) - m_obb.transform.translation));
	
	m_obb.transform.translation = hdVec3(xform.position.x, xform.position.y, this->GetWorldCenter().z);
	m_obb.transform.rotation =  hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle());
	
	this->ResetAABB();
}


void hdPhysicsParticle::Draw() const
{
	if (m_physicsBody == NULL) return;
	/*
#if TARGET_IPHONE_SIMULATOR != 1 && TARGET_OS_IPHONE != 1
	hdVec3 verticesWithDepth[this->GetVertexCount()];// = m_vertices->GetItems();
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		verticesWithDepth[i].x = m_vertices->GetItems()[i].x;
		verticesWithDepth[i].y = m_vertices->GetItems()[i].y;
		verticesWithDepth[i].z = m_vertices->GetItems()[i].z - m_depth;
	}
	
	hdVec3 verticesWithOppDepth[this->GetVertexCount()];// = m_vertices->GetItems();
	for (int i = this->GetVertexCount()-1; i >= 0 ; i--)
	{
		verticesWithOppDepth[this->GetVertexCount() - 1 - i].x = m_vertices->GetItems()[i].x;
		verticesWithOppDepth[this->GetVertexCount() - 1 - i].y = m_vertices->GetItems()[i].y;
		verticesWithOppDepth[this->GetVertexCount() - 1 - i].z = m_vertices->GetItems()[i].z + m_depth;
	}
	
	DrawVerticesWithTint(verticesWithDepth, 
						 ((hdPhysicsParticle*)this)->GetTextureCoordinates(),
						 ((hdPhysicsParticle*)this)->GetVertexCount(), 
						 ((hdPhysicsParticle*)this)->GetAABB(),
						 m_texture, m_tint, hdMin((float)fabs(life) / ((float)m_duration * 0.5f), 1.0f));
	
	DrawVerticesWithTint(verticesWithOppDepth, 
						 ((hdPhysicsParticle*)this)->GetTextureCoordinates(),
						 ((hdPhysicsParticle*)this)->GetVertexCount(), 
						 ((hdPhysicsParticle*)this)->GetAABB(),
						 m_texture, m_tint, hdMin((float)fabs(life) / ((float)m_duration * 0.5f), 1.0f));
	
	DrawExtrusion(((hdPhysicsParticle*)this)->GetVertices(), 
				  ((hdPhysicsParticle*)this)->GetTextureCoordinates(),
				  ((hdPhysicsParticle*)this)->GetVertexCount(), 
				  ((hdPhysicsParticle*)this)->GetAABB(),
				  m_texture, m_tint, hdMin((float)fabs(life) / ((float)m_duration * 0.5f), 1.0f), 
				  m_depth, true, m_projection->GetWorldCenter());
#endif*/
}




hdPhysicsParticleEmitter::hdPhysicsParticleEmitter(hdGameWorld *gameWorld, b2World *physicsWorld, 
												   hdOrthographicProjection *projection, b2Controller *controller, 
												   float duration, float explodeFactor)
{
	m_tint[0] = 1.0f;
	m_tint[1] = 1.0f;
	m_tint[2] = 1.0f;
	m_tint[3] = 1.0f;
	
	this->Init(gameWorld, physicsWorld, controller, duration, explodeFactor);
	m_texture = NULL;
	m_projection  = projection;
	m_viewportAABB = m_projection->GetAABB();
	
	if ("Low" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY) ||
		"Medium" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
	{
		m_drawWithCustomLighting = false;
	}
	else
	{
		m_drawWithCustomLighting = true;
	}
}


hdPhysicsParticleEmitter::~hdPhysicsParticleEmitter()
{
	hdAnimationController::Instance()->StopAnimations(this);
	hdAnimationController::Instance()->PurgeAnimations(this);
	this->RemoveAllParticles();
	delete m_particles;
}


void hdPhysicsParticleEmitter::Init(hdGameWorld *gameWorld, b2World *physicsWorld, 
									b2Controller *controller, float duration, 
									float explodeFactor)
{
	m_gameWorld = gameWorld;
	m_physicsWorld = physicsWorld;
	m_controller = controller;
	
	m_particles = new hdPointerList<hdPhysicsParticle, kPhysicsParticleMax>();
	
	m_texture = NULL;
	
	m_duration = duration;
	m_explodeFactor = explodeFactor;
	
	// We use the animation framework for step-wise update of the 
	// particle information.
	m_animation = hdAnimationController::CreateAnimation(this, false);
	m_animation->AddGameObject((hdGameObject *)this);
	m_functionAction = new hdFunctionAction<hdPhysicsParticleEmitter>();
	m_functionAction->SetFunctionObject(this);
	m_functionAction->SetFunction(&hdPhysicsParticleEmitter::Step);
	m_functionAction->SetDuration(m_duration + 0.1f);
	m_animation->AddAction(m_functionAction);
}



void hdPhysicsParticleEmitter::StartWithTexture(const polygon_info_t& vars)
{
	m_texture = vars.texture;
	m_tint[0] = vars.tint[0];
	m_tint[1] = vars.tint[1];
	m_tint[2] = vars.tint[2];
	m_tint[3] = vars.tint[3];
	this->Start(vars);
}


void hdPhysicsParticleEmitter::StartNoSplit(const polygon_info_t& vars, const hdVec2& linearVelocity)
{
	particle_info_t particleInfo;
	
	m_texture = vars.texture;
	m_tint[0] = vars.tint[0];
	m_tint[1] = vars.tint[1];
	m_tint[2] = vars.tint[2];
	m_tint[3] = vars.tint[3];
	
	particleInfo.vertices = vars.vertices;
	particleInfo.texCoords = vars.texCoords;
	particleInfo.vertexCount = vars.vertexCount;
	particleInfo.impactPoint = vars.impactPoint;
	particleInfo.depth = vars.depth;
	particleInfo.parentType = vars.parentType;
	particleInfo.parentTag = 0;
	particleInfo.explode = false;
	particleInfo.linearVelocity = linearVelocity;
	
	this->MakeParticle(particleInfo);
}


// Vertices2Particles breaking code.
//
// These particle co-ords should be more closely aligned with the 
// actual objects being broken apart; the particle making function
// should be virtual; different implentations do what they need
// to do.
void hdPhysicsParticleEmitter::Start(const polygon_info_t& vars)
{
	hdVec2 texDiff, texImpactPoint;
	hdVec3 dist, verDiff;
	particle_info_t particleInfo;
	uint32 i, i1, i2;
	
	if (m_particles->IsFull()) return;
	
	particleInfo.impactPoint = vars.impactPoint;
	particleInfo.depth = vars.depth;
	particleInfo.parentType = vars.parentType;
	particleInfo.parentTag = vars.parentTag;
	particleInfo.explode = true;
	particleInfo.linearVelocity.Set(0,0);
	
	if (vars.vertexCount <= 4)
	{
		for (i = 0; i < vars.vertexCount; ++i)
		{
			i1 = (i+1)%vars.vertexCount;
			
			if (rand() % 10 < 3)
			{
				// random midpoint between vertices[i] and vertices[i+1]
				float midPointRatio = (float)hdRandom(0.25, 0.75);
				hdVec3 midPoint = vars.vertices[i] + (midPointRatio * (vars.vertices[i1] - vars.vertices[i]));
				hdVec2 texMidPoint = vars.texCoords[i] + (midPointRatio * (vars.texCoords[i1] - vars.texCoords[i]));
				
				hdVec3 _verts1[3];
				hdVec3 _verts2[3];
				
				hdVec2 _tex1[3];
				hdVec2 _tex2[3];			
				
				_verts1[0].Set(vars.impactPoint.x, 
							   vars.impactPoint.y, 
							   vars.zOffset); 
				_verts1[1].Set(vars.vertices[i].x, 
							   vars.vertices[i].y, 
							   vars.zOffset);
				_verts1[2].Set(midPoint.x, 
							   midPoint.y, 
							   vars.zOffset); 
				
				if (hdPolygonArea(_verts1, 3) < hdPolygonArea(vars.vertices, vars.vertexCount))
				{
					_tex1[0].Set(vars.impactTexCoord.x, 
								 vars.impactTexCoord.y);
					_tex1[1].Set(vars.texCoords[i].x, 
								 vars.texCoords[i].y);
					_tex1[2].Set(texMidPoint.x, 
								 texMidPoint.y);
				
					particleInfo.vertices = _verts1;
					particleInfo.texCoords = _tex1;
					particleInfo.vertexCount = 3;
					
					this->MakeParticle(particleInfo);
				}
				
				_verts2[0].Set(vars.impactPoint.x, 
							   vars.impactPoint.y, 
							   vars.zOffset);
				_verts2[1].Set(midPoint.x, 
							   midPoint.y, 
							   vars.zOffset);
				_verts2[2].Set(vars.vertices[i1].x, 
							   vars.vertices[i1].y, 
							   vars.zOffset);
				
				if (hdPolygonArea(_verts2, 3) < hdPolygonArea(vars.vertices, vars.vertexCount))
				{
					_tex2[0].Set(vars.impactTexCoord.x, 
								 vars.impactTexCoord.y);
					_tex2[1].Set(texMidPoint.x, 
								 texMidPoint.y);
					_tex2[2].Set(vars.texCoords[i1].x, 
								 vars.texCoords[i1].y);
					
					particleInfo.vertices = _verts2;
					particleInfo.texCoords = _tex2;
					particleInfo.vertexCount = 3;
					
					this->MakeParticle(particleInfo);
				}
			}
			else
			{
				hdVec3 _verts[3];
				hdVec2 _tex[3];			
				
				
				_verts[0].Set(vars.impactPoint.x, 
							  vars.impactPoint.y, 
							  vars.zOffset); 
				_verts[1].Set(vars.vertices[i].x, 
							  vars.vertices[i].y, 
							  vars.zOffset);
				_verts[2].Set(vars.vertices[i1].x, 
							  vars.vertices[i1].y, 
							  vars.zOffset); 
				
				if (hdPolygonArea(_verts, 3) < hdPolygonArea(vars.vertices, vars.vertexCount))
				{
					_tex[0].Set(vars.impactTexCoord.x, 
								vars.impactTexCoord.y);
					_tex[1].Set(vars.texCoords[i].x, 
								vars.texCoords[i].y);
					_tex[2].Set(vars.texCoords[i1].x, 
								vars.texCoords[i1].y);
					
					particleInfo.vertices = _verts;
					particleInfo.texCoords = _tex;
					particleInfo.vertexCount = 3;
					
					this->MakeParticle(particleInfo);
				}
			}
		}
	}
	else if (vars.vertexCount > 4 && vars.vertexCount <= 8)
	{
		for (i = 0; i < vars.vertexCount; ++i)
		{
			i1 = (i+1)%vars.vertexCount;
			
			hdVec3 _verts[3];
			hdVec2 _tex[3];			
		
			_verts[0].Set(vars.impactPoint.x, 
						  vars.impactPoint.y, 
						  vars.zOffset); 
			_verts[1].Set(vars.vertices[i].x, 
						  vars.vertices[i].y, 
						  vars.zOffset);
			_verts[2].Set(vars.vertices[i1].x, 
						  vars.vertices[i1].y, 
						  vars.zOffset); 
		
			if (hdPolygonArea(_verts, 3) < hdPolygonArea(vars.vertices, vars.vertexCount))
			{
				_tex[0].Set(vars.impactTexCoord.x, 
							vars.impactTexCoord.y);
				_tex[1].Set(vars.texCoords[i].x, 
							vars.texCoords[i].y);
				_tex[2].Set(vars.texCoords[i1].x, 
							vars.texCoords[i1].y);
		
				particleInfo.vertices = _verts;
				particleInfo.texCoords = _tex;
				particleInfo.vertexCount = 3;
				
				this->MakeParticle(particleInfo);
			}
		}
	}
	else if (vars.vertexCount > 8)
	{
		for (i = 0; i < vars.vertexCount; i = i + 2)
		{
			i1 = (i+1)%vars.vertexCount;
			i2 = (i+2)%vars.vertexCount;
			
			hdVec3 _verts[4];
			hdVec2 _tex[4];			
			
			_verts[0].Set(vars.impactPoint.x, vars.impactPoint.y, vars.zOffset); 
			_verts[1].Set(vars.vertices[i].x, vars.vertices[i].y, vars.zOffset);
			_verts[2].Set(vars.vertices[i1].x, vars.vertices[i1].y, vars.zOffset);
			_verts[3].Set(vars.vertices[i2].x, vars.vertices[i2].y, vars.zOffset);
			
			_tex[0].Set(vars.impactTexCoord.x, vars.impactTexCoord.y);
			_tex[1].Set(vars.texCoords[i].x, vars.texCoords[i].y);
			_tex[2].Set(vars.texCoords[i1].x, vars.texCoords[i1].y);
			_tex[3].Set(vars.texCoords[i2].x, vars.texCoords[i2].y);
			
			particleInfo.vertices = _verts;
			particleInfo.texCoords = _tex;
			particleInfo.vertexCount = 4;
			
			this->MakeParticle(particleInfo);
		}
	}
}


bool hdPhysicsParticleEmitter::MakeParticle(const particle_info_t& info)
{
	hdPhysicsParticle* p;
	uint32 i;
	b2Vec2 norm;

	p = new hdPhysicsParticle(m_gameWorld);
	
	p->m_tint[0] = m_tint[0];
	p->m_tint[1] = m_tint[1];
	p->m_tint[2] = m_tint[2];
	p->m_tint[3] = m_tint[3];
	
	p->m_duration = m_duration;
	p->m_depth = info.depth;
	p->m_texture = m_texture;
	p->m_parentType = info.parentType;
	p->m_parentTag = info.parentTag;
	
	for (int i = 0; i < info.vertexCount; ++i)
	{
		p->AddPoint(info.vertices[i]);
	}
	
	
	p->ResetAABB();
	p->ResetOBB();
	
	if (info.vertexCount <= b2_maxPolygonVertices)
	{
		hdMake2DSingleShapePhysicsBody(m_physicsWorld, &p->m_physicsBody, p->GetWorldCenter(), 
									   p->GetVertices(), p->GetVertexCount(), 0.4f, 0.5f, 0.1f, 
									   false, false, 1, 0x02,  0x02);
	}
	else
	{
		hd2DMultiShapePhysicsBody(m_physicsWorld, &p->m_physicsBody, p->GetWorldCenter(), 
								  p->GetVertices(), p->GetVertexCount(), 0.4f, 0.5f, 0.1f,
								  false, false, 1, 0x02,  0x02);
	}
	
	p->ResetTextureCoords();
	for (i = 0; i < info.vertexCount; ++i)
	{
		p->GetTextureCoordinates()[i].Set(info.texCoords[i].x, info.texCoords[i].y);
	}
	
	memset(p->m_extrudedTextureCoords, 0, sizeof(p->m_extrudedTextureCoords));
	
	GenerateExtrusionTextureVertices(p->m_extrudedTextureCoords, 
									 p->GetVertices(), 
									 p->GetTextureCoordinates(), 
									 p->GetVertexCount(), 
									 p->GetAABB(), 
									 p->m_texture, 
									 p->m_depth,
									 1.0f);
	
	
	
	/* 
	 * Particles explode away from the central touch point,
	 * so we give them a linear velocity along the normal from
	 * the touch point to the last vertex on the particle.
	 */
	if (p->m_physicsBody != NULL) 
	{
		if (p->m_physicsBody->GetShapeList() == NULL)
		{
			m_gameWorld->RemoveGameObject(p);
			delete p;
			return false;
		}
		else
		{
			p->m_physicsBody->SetUserData(p);
			
			if (info.explode)
			{
				norm = (b2Vec2(p->GetVertices()[info.vertexCount-1].x, 
							p->GetVertices()[info.vertexCount-1].y) - 
							   b2Vec2(info.impactPoint.x, info.impactPoint.y));
				norm.Normalize();
				p->m_physicsBody->SetLinearVelocity(m_explodeFactor * norm);
				p->m_physicsBody->SetAngularVelocity(m_explodeFactor * norm.x);
			}
			else
			{
				p->m_physicsBody->SetLinearVelocity(b2Vec2(info.linearVelocity.x, info.linearVelocity.y));
			}
		}
		//if (m_controller != NULL) m_controller->AddBody(p->m_physicsBody);
	}
	else
	{
		m_gameWorld->RemoveGameObject(p);
		delete p;
		return false;
	}
	
	if (-1 == m_particles->Add(p))
	{
		if (p->m_physicsBody != NULL)
		{
			
			m_physicsWorld->DestroyBody(p->m_physicsBody);
		}
		m_gameWorld->RemoveGameObject(p);
		delete p;
		return false;
	}
	
	p->life = m_duration - 0.1f;
	return true;
}



void hdPhysicsParticleEmitter::Step(hdTimeInterval elapsed)
{
	hdPhysicsParticle* p; 
	
	// Destroy any bodies with zero life.
	for (int i = (m_particles->GetItemCount() - 1); i >= 0; --i)
	{
		p = m_particles->GetItems()[i];
		if (p->life <= 0.0f)
		{
			p->life = 0.0f;
			p->DestroyPhysicsBody(m_physicsWorld);
			m_gameWorld->RemoveGameObject(p);
			m_particles->RemoveAtIndex(i);
		} 
	}
	
	for (int i = 0; i < m_particles->GetItemCount(); ++i) 
	{
		p = m_particles->GetItems()[i];
		p->Step();
		p->life -= elapsed;
	}
}


 

void hdPhysicsParticleEmitter::Draw() const
{
#if 1
	hdPhysicsParticle *p;
	hdTexture *currTexture;
	int lastVertexCount;
	/*
	 * Speed up shard drawing by doing the following:
	 * - Only call glEnd() when it is absolutely necessary.
	 * - Only bind a new texture when the shard texture changes.
	 */
	
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);	
	glEnable(GL_TEXTURE_2D);
	
	lastVertexCount = 0;
	currTexture = NULL;
	
	for (int i = 0; i < m_particles->GetItemCount(); ++i)
	{
		p = m_particles->GetItems()[i];
		p->m_frameDraw = false;
		if (p->m_physicsBody != NULL && 
			p->m_texture != NULL && 
			p->AABBIntersection(m_viewportAABB))
		{
			p->m_frameDraw = true;

			if (currTexture != p->m_texture)
			{
				hdglEnd();
				hdglBindTexture(p->m_texture);
				currTexture = p->m_texture;
				lastVertexCount = 0;
			}
			
			if (p->GetVertexCount() == 4)
			{
				if (p->GetVertexCount() != lastVertexCount)
				{
					lastVertexCount = p->GetVertexCount();
					hdglEnd();
					hdglBegin(GL_QUADS);
				}
			}
			else
			{
				if (p->GetVertexCount() != lastVertexCount)
				{
					lastVertexCount = p->GetVertexCount();
					hdglEnd();
					hdglBegin(GL_TRIANGLES);
				}
			}
			
			if (hdMin(float(fabs(p->life) / 0.32f), 1.0f) < 1.0f)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
			
			hdglColor4f(p->m_tint[0], p->m_tint[1], p->m_tint[2], hdMin(float(fabs(p->life) / 0.32f), 1.0f)); //p->m_tint[3]));
			for (int j = 0; j < p->GetVertexCount(); ++j)
			{
				hdglTexCoord2f(p->GetTextureCoordinates()[j].x, p->GetTextureCoordinates()[j].y);
				hdglVertex3f(p->GetVertices()[j].x, p->GetVertices()[j].y, p->GetVertices()[j].z - p->m_depth);
			}
			
			for (int j = p->GetVertexCount()-1; j >= 0 ; --j)
			{
				hdglTexCoord2f(p->GetTextureCoordinates()[j].x, p->GetTextureCoordinates()[j].y);
				hdglVertex3f(p->GetVertices()[j].x, p->GetVertices()[j].y, p->GetVertices()[j].z + p->m_depth);
			}
			
		}
	}
	hdglEnd();
	currTexture = NULL;
	
	for (int i = 0; i < m_particles->GetItemCount(); ++i)
	{
		p = m_particles->GetItems()[i];
		if (p->m_physicsBody != NULL && 
			p->m_texture != NULL && 
			p->m_frameDraw)
		{
			if (p->m_texture != currTexture)
			{
				if (currTexture != NULL)
				{
					hdglEnd();
					if (m_drawWithCustomLighting)
					{
						StopTextureEffect(HDGL_EFFECT_DOUBLE_INTENSITY);
					}
				}
				
				hdglBindTexture(p->m_texture);
				
				if (m_drawWithCustomLighting)
				{
					StartTextureEffect(HDGL_EFFECT_DOUBLE_INTENSITY);
				}
				hdglBegin(GL_QUADS);
				if (hdMin(float(fabs(p->life) / 0.32f), 1.0f) < 1.0f)
				{
					glEnable(GL_BLEND);
				}
				else
				{
					glDisable(GL_BLEND);
				}
				currTexture = p->m_texture;
			}
			
			DrawExtrusion(p->GetVertices(), 
						  p->m_extrudedTextureCoords,
						  p->GetVertexCount(), 
						  p->GetStartingAABB(),
						  p->m_texture, 
						  p->m_tint, 
						  hdMin(float(fabs(p->life) / 0.32f), 1.0f),
						  p->m_depth, 
						  m_drawWithCustomLighting, 
						  false,
						  false,
						  m_projection->GetWorldCenter());		}
	}
	
	if (currTexture != NULL)
	{
		hdglEnd();
		if (m_drawWithCustomLighting)
		{
			StopTextureEffect(HDGL_EFFECT_DOUBLE_INTENSITY);
		}
	}
    
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
#else
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	m_particles->Map(&hdPhysicsParticle::Draw);
	glDisable(GL_CULL_FACE);
#endif
}


void hdPhysicsParticleEmitter::RemoveAllParticles()
{
	while(!m_particles->IsEmpty())
	{
		hdPhysicsParticle *p = m_particles->GetItems()[0];
		m_gameWorld->RemoveGameObject(p);
		p->DestroyPhysicsBody(m_physicsWorld);
		m_particles->Remove(p);
	}
}
