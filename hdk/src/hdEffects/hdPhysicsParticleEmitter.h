/*
 *  hdPhysicsParticleEmitter.h
 *  hdGameEngine
 *
 *  Created by david on 15/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_PHYSICS_PARTICLE_EMITTER_
#define _HD_PHYSICS_PARTICLE_EMITTER_


#include "Box2D.h"

#include "hdMath.h"
#include "hdGame.h"
#include "hdAnimationEngine.h"
#include "hdCollections.h"
#include "hdGraphicsLibrary.h"

#define kPhysicsParticleUserType 0xFF00


typedef struct
{
	hdTexture *texture;
	float tint[4];
	hdVec3 *vertices;
	hdVec3 *startingVertices;
	hdVec2 *texCoords;
	hdVec2 impactTexCoord;
	uint32 vertexCount;
	hdVec3 worldCenter;
	hdVec3 impactPoint;
	float depth;
	float zOffset;
	uint32 parentType;
	uint32 parentTag;
} polygon_info_t;


typedef struct
{
	hdVec3 *vertices;
	hdVec2 *texCoords;
	int vertexCount;
	hdVec3 impactPoint;
	float depth;
	uint32 parentType;
	uint32 parentTag;
	bool explode;
	hdVec2 linearVelocity;
} particle_info_t;


class hdPhysicsParticle : public hdPolygon, public hdDrawable
{
public:
	hdPhysicsParticle(hdGameWorld *m_gameWorld);
	
	~hdPhysicsParticle();
	
	void CreatePhysicsBody();
	
	void DestroyPhysicsBody(b2World *physicsWorld);
	
	void SetCategoryBits(b2World *physicsWorld, int bits);
	
	void Step();
	
	void Draw() const;
	
	// Overrides
	bool AABBIntersection(const hdAABB& aabb);
	
	bool RayIntersection(const hdVec3* ray, hdVec3& vOut);	
	
	hdTimeInterval life;
	
	b2Body* m_physicsBody;
	
	float m_tint[4];
	
	float m_duration;
	
	hdTexture* m_texture;
	
	float m_depth;
	
	unsigned int m_parentType;
	
	unsigned int m_parentTag;
	
	bool m_isTouchable;
	
	bool m_interacts;
	
	bool m_frameDraw;
	
	hdVec2 m_extrudedTextureCoords[kMaxPolygonVertices];
	
	hdAABB originStartingAABB;
};


#define kPhysicsParticleMax 64

// Creates physics objects as particles - lets the physics world handle particle
// parameter updates.
class hdPhysicsParticleEmitter : public hdDrawable
{
public:
	hdPhysicsParticleEmitter(hdGameWorld *gameWorld, b2World *physicsWorld, 
							 hdOrthographicProjection *projection, 
							 b2Controller *controller, float duration, 
							 float explodeFactor);
	
	~hdPhysicsParticleEmitter();
	
	void StartWithTexture(const polygon_info_t& vars);
	
	void Start(const polygon_info_t& vars);
	
	void StartNoSplit(const polygon_info_t& vars, const hdVec2& linearVelocity);
	
	void Step(hdTimeInterval elapsed);
	
	void Draw() const;
	
	void RemoveAllParticles();
	
	const hdPointerList<hdPhysicsParticle, kPhysicsParticleMax>* GetParticles() const;
	
	void SetViewportAABB(const hdAABB aabb);
	
private:
	void Init(hdGameWorld *gameWorld, b2World *physicsWorld, 
			  b2Controller *controller, float duration, float explodeFactor);
	
	bool MakeParticle(const particle_info_t& info);
	
	hdAnimation* m_animation;
	
	hdFunctionAction<hdPhysicsParticleEmitter>* m_functionAction;
	
	b2World* m_physicsWorld;
	
	hdGameWorld *m_gameWorld;
	
	b2Controller *m_controller;
	
	hdPointerList<hdPhysicsParticle, kPhysicsParticleMax>* m_particles;
	
	float m_duration;
	
	float m_tint[4];
	
	hdTexture *m_texture;
	
	float m_explodeFactor;
	
	hdOrthographicProjection *m_projection;
	
	bool m_drawWithCustomLighting;
	
	hdAABB m_viewportAABB;
};


inline const hdPointerList<hdPhysicsParticle, kPhysicsParticleMax>* hdPhysicsParticleEmitter::GetParticles() const
{
	return m_particles;
}


inline void hdPhysicsParticleEmitter::SetViewportAABB(const hdAABB aabb)
{
	m_viewportAABB = aabb;
}


#endif