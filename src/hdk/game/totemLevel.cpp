/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "totemLevel.h"

static int m_effectsLevel = -1;

totemLevel::totemLevel() 
{ 
	m_blocks = NULL;
	m_jacks = NULL;
	m_joints = NULL;
	m_events = NULL;
	
	m_backgroundLayer = NULL;
	m_midgroundLayer = NULL;
	m_foregroundLayer = NULL;
	
#ifdef LEVEL_EDITOR
	m_blocks = new hdSerializablePointerList<totemBlock, HDK_GAME_BlockMax>();
	m_jacks = new hdSerializablePointerList<totemJack, HDK_GAME_JackMax>();
	m_joints = new hdSerializablePointerList<totemJoint, HDK_GAME_JointMax>();
	m_events = new hdSerializablePointerList<totemEvent, HDK_GAME_EventMax>();
	
	m_backgroundLayer = new totemLayer(this, 200);
	m_midgroundLayer = new totemLayer(this, 100);
	m_foregroundLayer = new totemLayer(this, 75);	
#endif
	
	m_floorInfo = NULL;

	m_backgroundBottomTint[0] = 0.0f;
	m_backgroundBottomTint[1] = 0.0f;
	m_backgroundBottomTint[2] = 0.0f;
	m_backgroundBottomTint[3] = 1.0f;
	
	m_backgroundTopTint[0] = 0.0f;
	m_backgroundTopTint[1] = 0.0f;
	m_backgroundTopTint[2] = 0.0f;
	m_backgroundTopTint[3] = 1.0f;
	
	m_extendAABB = false;
	
	m_bgNearTexture = NULL;
	m_bgFarTexture = NULL;
	m_bgDistantTexture = NULL;
	m_skyTexture = NULL;
	
	m_floorLevelOffs = 0.0f;
	m_floorDrawingLevel = 0.0f;
	
    memset(m_nearBackgroundTexture, 0, sizeof(m_nearBackgroundTexture));
    memset(m_farBackgroundTexture, 0, sizeof(m_farBackgroundTexture));
    memset(m_distantBackgroundTexture, 0, sizeof(m_distantBackgroundTexture));
    memset(m_skyBackgroundTextureName, 0, sizeof(m_skyBackgroundTextureName));

#ifndef LEVEL_EDITOR
	c_effectsQualityLevel = hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY);
	
	m_blockTextureCache = new hdTypedefList<hdTexture*, 128>();
	
	m_sortedBlockCache = new hdTypedefList<totemBlock*, HDK_GAME_BlockMax>();
	
	m_jointTexture = NULL; 
#endif
}


totemLevel::~totemLevel()
{
	hdPrintf("Deleting totemLevel\n");
	
	if (m_blocks != NULL)
	{
		hdPrintf("\tDeleting m_blocks\n");
		m_blocks->RemoveAll();
		delete m_blocks;
		m_blocks = NULL;
	}
	
	if (m_jacks != NULL)
	{
		hdPrintf("\tDeleting m_jacks\n");
		m_jacks->RemoveAll();
		delete m_jacks;
		m_jacks = NULL;
	}
	
	if (m_joints != NULL)
	{
		hdPrintf("\tDeleting m_joints\n");
		m_joints->RemoveAll();
		delete m_joints;
		m_joints = NULL;
	}
	
	if (m_events != NULL)
	{
		hdPrintf("\tDeleting m_events\n");
		m_events->RemoveAll();
		delete m_events;
		m_events = NULL;
	}

	hdPrintf("\tDeleting Layers\n");
	if (m_backgroundLayer != NULL)
	{
		delete m_backgroundLayer;
		m_backgroundLayer = NULL;
	}
	
	if (m_midgroundLayer != NULL)
	{
		delete m_midgroundLayer;
		m_midgroundLayer = NULL;
	}
	
	if (m_foregroundLayer != NULL)
	{
		delete m_foregroundLayer;
		m_foregroundLayer = NULL;
	}
	
	if (m_floorInfo != NULL)
	{
		delete m_floorInfo;
	}
	
#ifndef LEVEL_EDITOR
	hdPrintf("\tDeleting caches\n");
	if (m_blockTextureCache != NULL)
	{
		delete m_blockTextureCache;
		m_blockTextureCache = NULL;
	}
	
	if (m_sortedBlockCache != NULL)
	{
		delete m_sortedBlockCache;
		m_sortedBlockCache = NULL;
	}
#endif
}


void totemLevel::Restart()
{
	m_blocks->Map(&totemBlock::ResetVertices);
	m_jacks->Map(&totemJack::ResetVertices);
	m_joints->Map(&totemJoint::ResetVertices);
}


void totemLevel::ResetBlocks() const
{	
	m_blocks->Map(&totemBlock::ResetVertices);	
	m_jacks->Map(&totemJack::ResetVertices);
	m_joints->Map(&totemJoint::ResetVertices);

	m_blocks->Map(&totemBlock::ResetPhysicsBody);	
	m_jacks->Map(&totemJack::ResetPhysicsBody);
	
	// Parent objects well and truly gone, so we don't have
	// to delete anything to do with the joints...
	m_joints->Map(&totemJoint::CreateJoint);
}


void totemLevel::InitLayers()
{
}


void totemLevel::InitGameObjects(hdGameWorld* gameWorld, hdOrthographicProjection* relativeProjection)
{
	this->Init(gameWorld);
	
	m_projection = relativeProjection;
	
	m_backgroundLayer->Init(gameWorld, this, relativeProjection);
	m_midgroundLayer->Init(gameWorld, this, relativeProjection);
	m_foregroundLayer->Init(gameWorld, this, relativeProjection);

	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		totemBlock* block = (totemBlock *)m_blocks->GetItems()[i];
		if (block != NULL)
		{
			block->Init(gameWorld);
		}
	}

	for (int i = 0; i < m_jacks->GetItemCount(); ++i)
	{
		totemJack* jack = (totemJack *)m_jacks->GetItems()[i];
		if (jack != NULL)
		{
			jack->Init(gameWorld);
		}
	}
	
	for (int i = 0; i < m_events->GetItemCount(); ++i)
	{
		totemEvent* event = (totemEvent *)m_events->GetItems()[i];
		if (event != NULL)
		{
			event->Init(gameWorld);
		}
	}
    
	ResetTextureCache();
}


void totemLevel::ResetTextureCache()
{
	if (strlen(m_skyBackgroundTextureName) > 0)
	{
		m_skyTexture = hdTextureManager::Instance()->FindTexture(m_skyBackgroundTextureName, TT_16Background);
	}
	else 
	{
		m_skyTexture = NULL;
	}
	
	if (strlen(m_farBackgroundTexture) > 0)
	{
		m_bgFarTexture = hdTextureManager::Instance()->FindTexture(m_farBackgroundTexture, TT_16Background);
	}
	else
	{
		m_bgFarTexture = NULL;
	}
	
	if (strlen(m_nearBackgroundTexture) > 0)
	{
		m_bgNearTexture = hdTextureManager::Instance()->FindTexture(m_nearBackgroundTexture, TT_16Background);
	}
	else
	{
		m_bgNearTexture = NULL;
	}
	
	if (strlen(m_distantBackgroundTexture) > 0)
	{
		m_bgDistantTexture = hdTextureManager::Instance()->FindTexture(m_distantBackgroundTexture, TT_16Background);
		
		if (m_bgDistantTexture == hdTextureManager::Instance()->GetNullTexture()) m_bgDistantTexture = NULL;
	}
	else 
	{
		m_bgDistantTexture = NULL;
	}
#ifndef LEVEL_EDITOR
	/*
	 * Sorted block cache sorts everything into a structure that is most
	 * efficient when drawing...
	 */
	
	m_blockTextureCache->RemoveAll();
	m_sortedBlockCache->RemoveAll();
	hdTexture* currTex;
	totemBlock *block;
	bool textureAlreadyCached;

	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		block = (totemBlock *)m_blocks->GetItems()[i];
		block->ResetAppearance();
		
		currTex = (hdTexture *)block->GetTexture();
		if (currTex != NULL)
		{
			textureAlreadyCached = false;
			
			for (int j = 0; j <	m_blockTextureCache->GetItemCount(); ++j)
			{
				if (currTex == (hdTexture *)m_blockTextureCache->GetItems()[j])
				{
					textureAlreadyCached = true;
					break;
				}
			}
			
			if (!textureAlreadyCached)
			{
				m_blockTextureCache->Add(currTex);
			}
		}
	}
	
	m_jointTexture = hdTextureManager::Instance()->FindTexture("Materials/p_joint02_15_16.tga", TT_Sprite);
#endif
}


void totemLevel::InitPhysicsObjects(hdGameWorld* gameWorld, b2World *physicsWorld)
{
	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		totemBlock* block = (totemBlock *)m_blocks->GetItems()[i];
		if (block != NULL)
		{
			block->Init(gameWorld, physicsWorld);
		}
	}
	
	for (int i = 0; i < m_jacks->GetItemCount(); ++i)
	{
		totemJack* jack = (totemJack *)m_jacks->GetItems()[i];
		if (jack != NULL)
		{
			jack->Init(gameWorld, physicsWorld);
		}
	}
	
	for (int i = 0; i < m_joints->GetItemCount(); ++i)
	{
		totemJoint* item = (totemJoint *)m_joints->GetItems()[i];
		if (item != NULL)
		{
			item->Init(gameWorld, physicsWorld);
		}
	}
	
	hdAABB currAABB = ((totemLevel*)this)->GetAABB();
	hdPrintf("aabb: lower (%3.2f, %3.2f, %3.2f) - upper (%3.2f, %3.2f, %3.2f)\n",
			 currAABB.lower.x, currAABB.lower.y, currAABB.lower.z,
			 currAABB.upper.x,  currAABB.upper.y,  currAABB.upper.z);
}


void totemLevel::DestroyGameObjects() const
{	
	m_backgroundLayer->DestroyGameObjects();
	m_midgroundLayer->DestroyGameObjects();
	m_foregroundLayer->DestroyGameObjects();
	
	m_world->RemoveGameObject(m_backgroundLayer);
	m_world->RemoveGameObject(m_midgroundLayer);
	m_world->RemoveGameObject(m_foregroundLayer);
	
	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		m_world->RemoveGameObject(m_blocks->GetItems()[i]);
	}
	
	for (int i = 0; i < m_jacks->GetItemCount(); ++i)
	{
		m_world->RemoveGameObject(m_jacks->GetItems()[i]);
	}
	
	for (int i = 0; i < m_joints->GetItemCount(); ++i)
	{
		m_world->RemoveGameObject(m_joints->GetItems()[i]);
	}
	
	for (int i = 0; i < m_events->GetItemCount(); ++i)
	{
		m_world->RemoveGameObject(m_events->GetItems()[i]);
	}
	
	// Will remove the joints
	m_blocks->Map(&totemBlock::DestroyPhysicsBody);	
	m_jacks->Map(&totemJack::DestroyPhysicsBody);
}


// Should really be a reduce function on the lists.
const hdAABB totemLevel::GetAABB() 
{
	hdAABB extendedAABB, aabb;
	
#ifdef LEVEL_EDITOR 
	m_aabb.lower.SetZero(); m_aabb.upper.SetZero();
	
	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		totemBlock* block = (totemBlock *)m_blocks->GetItems()[i];
		if (block != NULL && 
			(block->GetBlockType() != e_totemBlockTypeCosmetic) &&
			(block->GetBlockType() != e_totemBlockTypeBoundary) &&
			(block->GetBlockType() != e_totemBlockTypeBillboard))
		{
			m_aabb.lower = hdMin(m_aabb.lower, block->GetAABB().lower);
			m_aabb.upper = hdMax(m_aabb.upper, block->GetAABB().upper);
		}
	}
#endif

	if (m_extendAABB)
	{
		extendedAABB = m_aabb;
		
		float width = hdMin(6.4f, m_aabb.upper.x - m_aabb.lower.x);
		extendedAABB.lower.x -= width;
		extendedAABB.upper.x += width;
		if (m_floorInfo != NULL)
		{
			// ensure the camera will never go below the water level
			extendedAABB.lower.y = m_floorInfo->m_floorLevel - (0.32f * (m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y));
		}
		else
		{
			extendedAABB.lower.y *= 1.35f;
		}
		extendedAABB.upper.y += (1.5f * (extendedAABB.upper.y - GetWorldCenter().y));  //m_extendedAABB_HACKHACKHACK.upper.y;
		
		return extendedAABB;
	}
	else
	{	
#ifdef LEVEL_EDITOR
		return m_aabb;
#else
		hdAABB aabb = m_aabb;
		
		if (m_floorInfo != NULL)
		{
			// ensure the camera will never go below the water level
			aabb.lower.y = m_floorInfo->m_floorLevel - (0.32f * (m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y));
		}
		
		return aabb;
#endif
	}
}


void totemLevel::CommitBlockStates() const
{
	m_blocks->Map(&hdPolygon::SetStartingVerticesToCurrent);	
	m_jacks->Map(&hdPolygon::SetStartingVerticesToCurrent);
}


void totemLevel::DestroyBlockTextures()
{
	if (m_blocks == NULL) return;
	
	for(int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		totemBlock *block = m_blocks->GetItems()[i];
		hdTextureManager::Instance()->DestroyTexture((hdTexture*)block->GetTexture());
	}
}


static hdTypedefList<totemBlock*, 256>* m_blockPVS = NULL;
static int m_billboardBlockCount = 0;
static int m_rawBlockCount = 0;
static int m_normalBlockCount = 0;
static int m_drawBlockCount = 0;
static int m_reflectBlockCount = 0;


void totemLevel::Step()
{
#ifdef LEVEL_EDITOR 
	m_blocks->Map(&totemBlock::Step);
	m_jacks->Map(&totemJack::Step);
	m_joints->Map(&totemJoint::Step);
	
	((totemLevel *)this)->UpdateWaterLevel();
#else
	hdAABB sleepThreshold, scissorAABB, reflectAABB;
	float vel;
	b2Body *body;
	totemBlock * block;
	totemJoint *joint;
	unsigned i, j;
	
	scissorAABB = GetScissorAABB(true);
	reflectAABB = GetScissorAABB(false);
	
	if (m_effectsLevel == -1)
	{
		if ("Low" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
		{
			m_effectsLevel = 0;
		}
		else if ("Medium" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
		{
			m_effectsLevel = 1;
		}
		else if ("High" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
		{
			m_effectsLevel = 2;
		}
		else
		{
			m_effectsLevel = 0;
		}
	}
	hdAssert(m_effectsLevel >= 0 && m_effectsLevel <= 3);

	// OPTIMIZATION - potentially visible set
	// A small subset of the blocks to be drawn on screen, so no
	// need to call b2BodyIntersection on each and every one for
	// both extrusions and faces.
	if (m_blockPVS == NULL) m_blockPVS = new hdTypedefList<totemBlock*, 256>();
	m_blockPVS->RemoveAll();
	
	m_billboardBlockCount = 0;
	m_rawBlockCount = 0;
	m_normalBlockCount = 0;
	m_drawBlockCount = 0;
	m_reflectBlockCount = 0;
	
	for (i = 0; i < m_blocks->GetItemCount(); ++i)
	{	
		block = m_blocks->GetItems()[i];
		if (block->IsDrawable())
		{
			block->m_frameDraw = false;
			block->m_frameDrawReflect = false;
			
			block->m_frameStepNormal = false;
			block->m_frameStepRaw = false;
			block->m_frameStepBillboard = false;
			
			if (block->ContainsGameFlag((uint32)e_totemGameFlagsBlockDrawRaw))
			{
				block->m_frameStepRaw = true;
				++m_rawBlockCount;
			}
			else if (block->GetBlockType() == e_totemBlockTypeBillboard)
			{
				block->m_frameStepBillboard = true;
				++m_billboardBlockCount;
			}
			else
			{
				block->m_frameStepNormal = true;
				++m_normalBlockCount;
			}
			
			block->m_frameDraw = block->b2BodyIntersection(scissorAABB, true);
			if (m_effectsLevel == 0)
			{
				block->m_frameDrawReflect = false;
			}
			else if (m_effectsLevel == 1)
			{
				block->m_frameDrawReflect = block->IsStatic() && 
					block->b2BodyIntersection(reflectAABB, true);
			}
			else if (m_effectsLevel == 2)
			{
				block->m_frameDrawReflect = block->b2BodyIntersection(reflectAABB, true);
			}		
			
			if (block->m_frameDraw)
				++m_drawBlockCount;
			
			if (block->m_frameDrawReflect)
				++m_reflectBlockCount;

			/*
			 * Blocks in the PVS
			 */ 
			if (block->m_frameDraw || block->m_frameDrawReflect)
			{
				if (m_blockPVS->IsEmpty())
				{
					m_blockPVS->Add(block);
				}
				else if (block->GetZOffset() > m_blockPVS->GetItems()[0]->GetZOffset())
				{
					m_blockPVS->AddAtIndex(block, 0);
				}
				else
				{
					m_blockPVS->Add(block);
				}
			}
		}
	}
	
	sleepThreshold.lower.Set(m_projection->GetWorldCenter().x - 12.0f, 
							 m_projection->GetWorldCenter().y - 12.0f, 
							 -1.0f);
	sleepThreshold.upper.Set(m_projection->GetWorldCenter().x + 12.0f, 
							 m_projection->GetWorldCenter().y + 12.0f, 
							 1.0f);
	
	/*
	 * - Step blocks on screen 
	 * - Sleep any physics bodies outside our threshold.
	 */
	for (i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		block = m_blocks->GetItems()[i];
		if (block->ContainsGameFlag(e_totemGameFlagsBlockDrawRaw))
		{
			block->Step();
		}
		else if (block->b2BodyIntersection(m_projection->GetAABB(), true) ||
				 block->b2BodyIntersection(scissorAABB, true))
		{
			block->Step();
		} 
		
		/* 
		 * Sleep all objects that are outside the sleep threshold, not
		 * travelling over 2.5 m/s
		 */
		if (block->b2BodyIntersection(sleepThreshold, true))
		{
			if (!block->IsStatic() && block->GetPhysicsBody())
			{
				body = (b2Body *)block->GetPhysicsBody();
				if (body->IsSleeping())
				{
					body->WakeUp();
				}
			}
		}
		else
		{
			if (!block->IsStatic() && block->GetPhysicsBody())
			{
				body = (b2Body *)block->GetPhysicsBody();
				if (!body->IsSleeping())
				{
					vel = fabs(hdFastHypotenuse(body->GetLinearVelocity().x, 
												body->GetLinearVelocity().y));
					if (vel < 2.5f)
					{
						body->PutToSleep();
					}
				}
			}
		}
	}
	
	for (i = 0; i < m_joints->GetItemCount(); ++i)
	{	
		joint = m_joints->GetItems()[i];
		
		joint->m_frameDraw = false;
		joint->m_frameDrawReflect = false;
		
		if (joint->AABBIntersection(scissorAABB))
		{
			joint->Step();
		} 
		
		joint->m_frameDraw = joint->AABBIntersection(scissorAABB);
		if (m_effectsLevel == 0)
		{
			joint->m_frameDrawReflect = false;
		}
		else if (m_effectsLevel == 1)
		{
			joint->m_frameDrawReflect = false;
		}
		else if (m_effectsLevel == 2)
		{
			joint->m_frameDrawReflect = joint->AABBIntersection(reflectAABB);
		}
	}
	
	((totemLevel *)this)->UpdateWaterLevel();
#endif
}


#define kFarTextureZDistance 60.0f
#define kNearTextureZDistance 30.0f

#ifdef LEVEL_EDITOR
void totemLevel::DrawEditorView() const
{
	hdAABB currAABB;
	
	currAABB = ((totemLevel*)this)->GetAABB();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	this->DrawLevel(true);
	
	if (m_floorInfo != NULL)
	{
		hdglBegin(GL_LINES);
		hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		hdglVertex2f(currAABB.lower.x-20.0f, m_floorInfo->m_floorLevel);
		hdglVertex2f(currAABB.upper.x+20.0f, m_floorInfo->m_floorLevel);
		hdglEnd();
	}
}
#endif


void totemLevel::Draw() const
{
#ifdef LEVEL_EDITOR
	((totemLevel *)this)->UpdateWaterLevel();
#endif

	bool reflectSky, reflectBackground, reflectLevel;
	float floorLevel;
	hdAABB currAABB;
	
	hdMatrix reflectionMatrix;
	
	currAABB = ((totemLevel*)this)->GetAABB();
	
	reflectSky = (m_floorInfo != NULL && 
				  m_floorInfo->IsDrawable() && 
				  m_floorInfo->m_isReflective &&
				  m_effectsLevel > 1);
	
	reflectBackground = (m_floorInfo != NULL && 
						 m_floorInfo->IsDrawable() && 
						 m_floorInfo->m_isReflective &&
						 m_effectsLevel > 1);
	
	reflectLevel = (m_floorInfo != NULL &&
					m_floorInfo->m_isReflective &&
					m_reflectBlockCount > 0 &&
					m_effectsLevel > 0);

	floorLevel = 0.0f;
	if (m_floorInfo == NULL)
	{
		floorLevel = currAABB.lower.y;
	} 
	else
	{
		floorLevel = m_floorInfo->m_floorLevel;
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);
	
	hdglBindTexture(NULL);
	
	if (m_floorInfo == NULL || m_floorInfo->IsDrawable() == false)
	{
		this->DrawLevel(true);
	}
	else
	{
		GLfloat equation0[] = {0, 1.0f, 0, -m_floorDrawingLevel};
		glEnable( GL_CLIP_PLANE0 );
		hdglClipPlanef(GL_CLIP_PLANE0, equation0);
		
		this->DrawLevel(true);
		
		glDisable( GL_CLIP_PLANE0 );
		
		// Reflection - use the clip plane for culling.
		if (reflectLevel)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
	
			GLfloat equation1[] = {0, -1.0f, 0, m_floorDrawingLevel};
			glEnable( GL_CLIP_PLANE0 );
			hdglClipPlanef(GL_CLIP_PLANE0, equation1);
			hdglError("clip plane d");
			
			glTranslatef(0.0f, -2.0f*(0.0f-m_floorDrawingLevel), 0.0f);
			glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

			MatrixIdentity(reflectionMatrix);
			reflectionMatrix.f[__33] = -1.0f;
			glMultMatrixf(reflectionMatrix.f);
			
			// Reflection
			this->DrawLevel(false);
			glDisable( GL_CLIP_PLANE0 );
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	glDisable(GL_BLEND);
	if (m_skyTexture != NULL)
	{
		this->DrawBackgroundLayer(m_skyTexture, 180.0f, 1.25f, floorLevel, 255, reflectSky, 1.0f);
	}
	glEnable(GL_BLEND);
	
	if (m_bgDistantTexture != NULL)
	{
		this->DrawBackgroundLayer(m_bgDistantTexture, 120.0f, 0.25f, floorLevel, 255, reflectBackground, 2.0f);
	}
	
	if (m_bgFarTexture != NULL)
	{
		this->DrawBackgroundLayer(m_bgFarTexture, kFarTextureZDistance, 0.5f, floorLevel, 255, reflectBackground, 2.0f);
	} 

	if (m_bgNearTexture != NULL)
	{
		m_bgNearTexture->isPremultipliedAlpha = false;
		this->DrawBackgroundLayer(m_bgNearTexture, kNearTextureZDistance, 1.0f, floorLevel, 255, reflectBackground, 2.0f); //1.0f);			
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Draw billboards last, if there are any...
	if (m_billboardBlockCount > 0)
	{
		if (m_floorInfo == NULL || m_floorInfo->IsDrawable() == false)
		{
			this->DrawBillboards(true);
		}
		else
		{
			GLfloat equation0[] = {0, 1.0f, 0, -m_floorDrawingLevel};
			hdglClipPlanef(GL_CLIP_PLANE0, equation0);
			glEnable( GL_CLIP_PLANE0 );
			this->DrawBillboards(true);
			glDisable( GL_CLIP_PLANE0 );

			// Reflection - use the clip plane for culling.
			if (reflectLevel)
			{
				glDepthMask(GL_FALSE);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				GLfloat equation1[] = {0, -1.0f, 0, m_floorDrawingLevel};
				hdglClipPlanef(GL_CLIP_PLANE0, equation1);
				glEnable( GL_CLIP_PLANE0 );
				
				glTranslatef(0.0f, -2.0f*(0.0f-m_floorDrawingLevel), 0.0f);
				glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

				MatrixIdentity(reflectionMatrix);
				reflectionMatrix.f[__33] = -1.0f;
				glMultMatrixf(reflectionMatrix.f);
				
				// Reflection
				this->DrawBillboards(false);
				glDisable( GL_CLIP_PLANE0 );
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
				glDepthMask(GL_TRUE);
			}
		}
	}
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}


void totemLevel::UpdateWaterLevel()
{
	/* up/down water... */
	if (m_floorInfo != NULL)
	{
#ifdef LEVEL_EDITOR
		m_floorDrawingLevel = m_floorInfo->m_floorLevel;
#else
		// TODO: put magic numbers in #defines
		// TODO: Randomise this.
		m_floorLevelOffs = hdMax(m_floorLevelOffs + 0.044f, 0.0f); 
		if (m_floorLevelOffs > 6.2832f) m_floorLevelOffs = 0.0f;
		m_floorDrawingLevel = m_floorInfo->m_floorLevel + (0.05f * sinf(m_floorLevelOffs));
#endif
	}
}


void totemLevel::DrawFloor() const
{
	hdAABB currAABB;
	
	if (m_floorInfo == NULL) return;
	
	currAABB = ((totemLevel*)this)->GetAABB();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
    
    const float kfloorLength = 42.0f;
    
	if (m_floorInfo->IsDrawable() && m_floorInfo->m_isWater)
	{
		hdVec3 bcVertices[4];
		bcVertices[0].Set(currAABB.lower.x-kfloorLength, m_floorDrawingLevel, -(kNearTextureZDistance - 2.5f));
		bcVertices[1].Set(currAABB.lower.x-kfloorLength, m_floorDrawingLevel, 14.0f);
		bcVertices[2].Set(currAABB.upper.x+kfloorLength, m_floorDrawingLevel, 14.0f);
		bcVertices[3].Set(currAABB.upper.x+kfloorLength, m_floorDrawingLevel, -(kNearTextureZDistance - 2.5f));
		
		DrawMultiTextureQuad(bcVertices, m_floorInfo->GetFirstTexture(),  m_floorInfo->GetSecondTexture(), 0.75f);
	}
	else if (m_floorInfo->GetFirstTexture() != NULL)
	{
		float xFloorRatio = 3.0f * ((40.0f + (currAABB.upper.x - currAABB.lower.x)) / (10.0f + (kNearTextureZDistance - 0.5f)));
		float yFloorRatio = 3.0f;

        hdglBindTexture(m_floorInfo->GetFirstTexture());
		hdglBegin(GL_QUADS);
		hdglColor4ub(255, 255, 255, 255);
        
		hdglTexCoord2f(0.0f, yFloorRatio);
		hdglVertex3f(currAABB.lower.x-kfloorLength, m_floorDrawingLevel, -(kNearTextureZDistance - 2.5f));
		
		hdglTexCoord2f(0.0f, 0.0f);
		hdglVertex3f(currAABB.lower.x-kfloorLength, m_floorDrawingLevel, 14.0f);
		
		hdglTexCoord2f(xFloorRatio, 0.0f);
		hdglVertex3f(currAABB.upper.x+kfloorLength, m_floorDrawingLevel, 14.0f);
		
		hdglTexCoord2f(xFloorRatio, yFloorRatio);
		hdglVertex3f(currAABB.upper.x+kfloorLength, m_floorDrawingLevel, -(kNearTextureZDistance - 0.5f));
		hdglEnd();
	}
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}


/*
 * TODO: Figure out these y equations and what they mean. Then shoot the person that wrote them.
 */
void totemLevel::DrawBackgroundLayer(const hdTexture *texture, const float zDistance, 
									 const float heightFactor, const float floorLevel, 
									 const short alpha, const bool reflect, const float ratio) const
{
	if (texture == hdTextureManager::Instance()->GetNullTexture()) return;
	
	if (texture->isPremultipliedAlpha)
	{
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	hdAABB currAABB = ((totemLevel*)this)->GetAABB();
	GLuint param;
	
	hdglBindTexture(texture);
	hdglBegin(GL_QUADS);
	hdglColor4ub(255, 255, 255, alpha);
	
    float aspectRatio = 1.5f;
    ScreenSettings_GetAspectRatio(&aspectRatio);
    float quadHalfWidth = zDistance * aspectRatio;
    
	hdglTexCoord2f(0.0f, 0.995f);
	hdglVertex3f(currAABB.lower.x-quadHalfWidth, 
				 floorLevel-0.04f, 
				 -zDistance); 
	hdglTexCoord2f(0.0f, fabs((ratio/2.0f) - 0.995f));
	hdglVertex3f(currAABB.lower.x-quadHalfWidth, 
				 floorLevel+(((currAABB.upper.x+quadHalfWidth*heightFactor) - (currAABB.lower.x-quadHalfWidth)) * 0.5f), 
				 -zDistance);
	hdglTexCoord2f(ratio, fabs((ratio/2.0f) - 0.995f));
	hdglVertex3f(currAABB.upper.x+quadHalfWidth, 
				 floorLevel+(((currAABB.upper.x+quadHalfWidth*heightFactor) - (currAABB.lower.x-quadHalfWidth)) * 0.5f), 
				 -zDistance);
	hdglTexCoord2f(ratio, 0.995f);
	hdglVertex3f(currAABB.upper.x+quadHalfWidth, 
				 floorLevel-0.04f, 
				 -zDistance);
	hdglEnd();
#ifdef LEVEL_EDITOR
	if (true)
#else
	if (reflect)
#endif
	{
		hdglBegin(GL_QUADS);
		hdglColor4ub(255, 255, 255, alpha);
		hdglTexCoord2f(0.0f, fabs((ratio/2.0f) - 0.995f)); 
		hdglVertex3f(currAABB.lower.x-quadHalfWidth, 
					 floorLevel-(((currAABB.upper.x+quadHalfWidth*heightFactor) - (currAABB.lower.x-quadHalfWidth)) * 0.5f), 
					 -zDistance);
		hdglTexCoord2f(0.0f, 0.995f);
		hdglVertex3f(currAABB.lower.x-quadHalfWidth, 
					 floorLevel+0.04f, 
					 -zDistance);
		hdglTexCoord2f(ratio, 0.995f);
		hdglVertex3f(currAABB.upper.x+quadHalfWidth, 
					 floorLevel+0.04f, 
					 -zDistance);
		hdglTexCoord2f(ratio, fabs((ratio/2.0f) - 0.995f));
		hdglVertex3f(currAABB.upper.x+quadHalfWidth, 
					 floorLevel-(((currAABB.upper.x+quadHalfWidth*heightFactor) - (currAABB.lower.x-quadHalfWidth)) * 0.5f), 
					 -zDistance);
		hdglEnd();
	}
}


void totemLevel::DrawLevel(bool front) const
{
	totemJoint *joint;
	hdAABB scissorAABB;
	hdTexture* currentTexture;
	totemBlock *block;
	
	if (m_blockPVS == NULL) return;
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	if (front) 
		glCullFace(GL_FRONT);
	else 
		glCullFace(GL_BACK);
	
	/* Draw faces all at once for each texture */
	currentTexture = NULL;
	block = NULL;
	
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < m_blockTextureCache->GetItemCount(); ++i)
	{
		currentTexture = (hdTexture *)m_blockTextureCache->GetItems()[i];
		
		for(int j = 0; j < m_blockPVS->GetItemCount(); ++j)
		{
			block = m_blockPVS->GetItems()[j];
			
			if (block->GetTexture() == currentTexture)
			{
				if ((front && block->m_frameDraw) ||
					(!front && block->m_frameDrawReflect))
				{
					if (block->m_frameStepNormal)
					{
						block->DrawBlock(front, m_projection->GetWorldCenter());
					}
				}
			}
		}
	}
	
	// Raws
	for(int i = 0; i < m_blockPVS->GetItemCount(); ++i)
	{
		block = m_blockPVS->GetItems()[i];
		
		if (block->m_frameStepRaw)
		{
			if ((front && block->m_frameDraw) ||
				(!front && block->m_frameDrawReflect))
			{
				block->Draw();
			}
		}
	}
	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	hdglBindTexture(m_jointTexture);
		
	for(int i = 0; i < m_joints->GetItemCount(); ++i)
	{
		joint = m_joints->GetItems()[i];
		
		if ((front && joint->m_frameDraw) ||
			(!front && joint->m_frameDrawReflect))
		{
			joint->Draw();
		}
	}
	hdglBindTexture(NULL);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}



void totemLevel::DrawBillboards(bool front) const
{
	hdTexture* currentTexture;
	totemBlock *block;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	if (front) 
		glCullFace(GL_FRONT);
	else 
		glCullFace(GL_BACK);
	
	glEnable(GL_TEXTURE_2D);
	
	for (int i = 0; i < m_blockTextureCache->GetItemCount(); ++i)
	{
		currentTexture = (hdTexture *)m_blockTextureCache->GetItems()[i];
		
		for(int j = 0; j < m_blockPVS->GetItemCount(); ++j)
		{
			block = m_blockPVS->GetItems()[j];
			
			if (block->GetTexture() == currentTexture)
			{
				if (front && block->m_frameDraw && block->m_frameStepBillboard)
				{
					block->DrawBlock(front, m_projection->GetWorldCenter());
				}
				else if (!front && block->m_frameDrawReflect && block->m_frameStepBillboard)
				{
					block->DrawBlock(front, m_projection->GetWorldCenter());
				}
			}
		}
	}	
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}


void totemLevel::DrawPVSBlocksWithTexture(const bool front, hdTexture* texture) const
{
}


// Attempt to find the coordinates of the projected shape.
void totemLevel::DrawBBs()
{
#if DEBUG_PROJECTION
	hdMatrix projectionMat;
	hdMatrix perspectiveMat;
	hdMatrix invertMat;
	hdMatrix translationMat;
	
	MatrixPerspectiveFovRH(perspectiveMat, 90.0f*(hd_pi/180.0f), 1.0f, 0.01f, 1000.0f, false);
	
	MatrixTranslation(translationMat, -m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y, 0.0f);

	hdMatrixMultiply(projectionMat, perspectiveMat, translationMat);
	MatrixInverse(invertMat, projectionMat);
	hdVec3 temp, depthTemp;
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glOrthof(m_projection->GetAABB().lower.x,
			 m_projection->GetAABB().upper.x, 
			 m_projection->GetAABB().lower.y, 
			 m_projection->GetAABB().upper.y, 
			 m_projection->GetAABB().lower.z, 
			 m_projection->GetAABB().upper.z);
	for (int i = 0; i < m_blocks->GetItemCount(); ++i)
	{
		hdglBegin(GL_LINE_STRIP);
		
		// Multiply each vertex by the projection matrix and then render...
		for (int j = 0; j < m_blocks->GetItems()[i]->GetVertexCount(); j++)
		{
			MatrixVec3Multiply(temp, m_blocks->GetItems()[i]->GetVertices()[j], invertMat);
			hdglVertex3f(temp.x, temp.y, temp.z);
		}
		for (int j = 0; j < m_blocks->GetItems()[i]->GetVertexCount(); j++)
		{
			depthTemp = m_blocks->GetItems()[i]->GetVertices()[j];
			depthTemp.z = depthTemp.z - 1.0f;
			MatrixVec3Multiply(temp, depthTemp, invertMat);
			hdglVertex3f(temp.x, temp.y, 0.0f);
		}
		MatrixVec3Multiply(temp, m_blocks->GetItems()[i]->GetVertices()[0], invertMat);
		hdglVertex3f(temp.x, temp.y, 0.0f);
		
		hdglEnd();
	}
#endif
}


hdAABB totemLevel::GetScissorAABB(bool front)
{
	hdAssert(m_projection != NULL);
	
	hdAABB scissorAABB;

	if (front)
		scissorAABB = m_projection->GetAABB();
	else
	{
		if (m_floorInfo == NULL)
		{
			scissorAABB.lower.Set(-1000,0,0);
			scissorAABB.upper.Set(-999,0,0);
		}
		else
		{
			// Set the aabb slightly below the water level, so distant polygons
			// don't get rejected by the aabb test.
			scissorAABB.lower.Set(m_projection->GetAABB().lower.x, 
								  m_floorDrawingLevel, 
								  m_projection->GetAABB().lower.z);
			scissorAABB.upper.Set(m_projection->GetAABB().upper.x, 
								  m_floorDrawingLevel + hdMax(0.0f, m_floorDrawingLevel - m_projection->GetAABB().lower.y), 
								  m_projection->GetAABB().upper.z);
		}
	}
	
	return scissorAABB;
}


float totemLevel::GetFloorDrawingLevel()
{
	return m_floorDrawingLevel;
}
