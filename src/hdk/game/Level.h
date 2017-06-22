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

#ifndef _HDK_GAME_LEVEL_H_
#define _HDK_GAME_LEVEL_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "Box2D.h"

#include <hdk/graphics.h>
#include "Enums.h"
#include "Constants.h"
#include "FloorInfo.h"
#include "Layer.h"
#include "GameBlock.h"
#include "Event.h"
#include "Jack.h"
#include "Joint.h"

#define kMaxLevelNameSize 256

class Level : public hdGameObject, public hdDrawable
{
public:
	Level();
	
	~Level();

	void Init(hdGameWorld *gameWorld) { ((hdGameObject *)this)->Init(gameWorld); }
	
	const hdAABB GetAABB();
	
	void ResetOBB() {}
	
	hdVec3* GetVertices()
	{
		return m_obb.vertices;
	}
	
	const int GetVertexCount() const
	{
		return m_obb.GetVertexCount();
	}
	
	void ApplyTransform(const hdTransform& transform)
	{
		m_obb.transform = transform;
		ResetOBB();
	}
	
	void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) {}
	
	void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) {}
	
	void Step();
	
	void Draw() const;
	
#ifdef LEVEL_EDITOR
	void DrawEditorView() const;
#endif
	
	void DrawFloor() const;
	
	
	void CommitBlockStates() const;
	
	void DestroyGameObjects() const;
	
	void InitGameObjects(hdGameWorld* gameWorld, hdOrthographicProjection* relativeProjection);
	
	void InitPhysicsObjects(hdGameWorld* gameWorld, b2World *physicsWorld);
	
	void InitLayers();
	
	void Restart();
	
	
	void ResetBlocks() const; // sets all block states back to the original
	
	void ResetTextureCache();
	
	
	// Block wrapper functions
	int AddBlock(Block* block);
	
	const bool RemoveBlockAtIndex(const int index);
	
	const bool RemoveBlock(const Block* block);
	
	Block** GetBlocks() const;
	
	const int GetBlockCount() const;

	const int GetBlockIndex(const Block *block);
	
	
	
	// Jack functions
	int AddJack(const Jack* jack);
	
	int AddJackAtIndex(const Jack* jack, const int index);
	
	bool RemoveJack(const Jack* jack);
	
	bool RemoveJackAtIndex(const int index);
	
	Jack** GetJacks() const;
	
	const int GetJackCount() const;
	
	
	
	// Events
	hdSerializablePointerList<Event, HDK_GAME_EventMax>* GetEvents(); 
	
	
	
	// Joints
	int AddJoint(const Joint* Joint);
	
	int AddJointAtIndex(const Joint* Joint, const int index);
	
	bool RemoveJoint(const Joint* Joint);
	
	bool RemoveJointAtIndex(const int index);
	
	Joint** GetJoints() const;
	
	const int GetJointCount() const;

	void DrawBBs();
	
	
	Layer* GetBackgroundLayer()
	{
		return m_backgroundLayer;
	}
	
	Layer* GetForegroundLayer()
	{
		return m_foregroundLayer;
	}
	
	Layer* GetMidgroundLayer()
	{
		return m_midgroundLayer;
	}
	
	const char* GetLevelName() const;
	
	void SetLevelName(const char* name);

	void DestroyBlockTextures();

	// BG
	const char* GetSkyTextureName() const;
	
	void SetSkyTextureName(const char* textureName);
	
	const char* GetDistantBackgroundTextureName() const;
	
	void SetDistantBackgroundTextureName(const char* textureName);
	
	const char* GetFarBackgroundTextureName() const;
	
	void SetFarBackgroundTextureName(const char* textureName);
	
	const char* GetNearBackgroundTextureName() const;
	
	void SetNearBackgroundTextureName(const char* textureName);
	
	const float* GetBackgroundBottomTint() const;
	
	void SetBackgroundBottomTint(const int r, const int g, const int b);
	
	const float* GetBackgroundTopTint() const;
	
	void SetBackgroundTopTint(const int r, const int g, const int b);
	
	
	// Floor info
	const FloorInfo* GetFloorInfo() const;
	
	void SetFloorInfo(FloorInfo *floorInfo);
	
	bool GetExtendAABB();
	
	void SetExtendAABB(bool extendAABB);

	float GetFloorDrawingLevel();

	hdAABB GetScissorAABB(bool front);
	
	
private:
	friend class LevelController;
	
	void DrawLevel(bool front) const;
	
	void DrawPVSBlocksWithTexture(const bool front, hdTexture* texture) const;
	
	void DrawBackgroundLayer(const hdTexture *texture, const float zDistance, 
							 const float heightFactor, const float floorLevel, 
							 const short alpha, const bool reflect, const float ratio) const;
	
	void DrawBillboards(bool front) const;
	
	
	
	void UpdateWaterLevel();
	
	/*
	 * Temporary cache of texture pointers - cleared 
	 * and created on InitGameObjects
	 */
	hdTypedefList<hdTexture*, 128>* m_blockTextureCache;
	
	hdTypedefList<Block*, HDK_GAME_BlockMax>* m_sortedBlockCache;

	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);
		ar & m_blocks;
		ar & m_jacks;
		
		if (version >= 5)
		{
			ar & m_backgroundLayer;
			ar & m_midgroundLayer;
			ar & m_foregroundLayer;
		}
		if (version >= 6)
		{
			ar & m_levelName;
		}
		if (version >= 7)
		{
			ar & m_joints;
		}
		if (version >= 8)
		{
			ar & m_floorInfo;
			ar & m_nearBackgroundTexture;
			ar & m_farBackgroundTexture;
			ar & m_backgroundBottomTint;
			ar & m_backgroundTopTint;
		}
		if (version >= 9)
		{
			ar & m_extendAABB;
		}
		if (version >= 10)
		{
			ar & m_events;
		}
		if (version >= 11)
		{
			ar & m_distantBackgroundTexture;
		}
		if (version >= 12)
		{
			ar & m_skyBackgroundTextureName;
		}
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);
		ar & m_blocks;
		ar & m_jacks;
		
		if (version >= 5)
		{
			ar & m_backgroundLayer;
			ar & m_midgroundLayer;
			ar & m_foregroundLayer;
		}
		if (version >= 6)
		{
			ar & m_levelName;
		}
		if (version >= 7)
		{
			ar & m_joints;
		}
		if (version >= 8)
		{
			ar & m_floorInfo;
			ar & m_nearBackgroundTexture;
			ar & m_farBackgroundTexture;
			ar & m_backgroundBottomTint;
			ar & m_backgroundTopTint;
		}
		if (version >= 9)
		{
			ar & m_extendAABB;
		}
		if (version >= 10)
		{
			ar & m_events;
		}
		if (version >= 11)
		{
			ar & m_distantBackgroundTexture;
		}
		if (version >= 12)
		{
			ar & m_skyBackgroundTextureName;
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	
	hdSerializablePointerList<Jack, HDK_GAME_JackMax>* m_jacks;
	
	hdSerializablePointerList<Block, HDK_GAME_BlockMax>* m_blocks;
	
	hdSerializablePointerList<Joint, HDK_GAME_JointMax>* m_joints;
	
	hdSerializablePointerList<Event, HDK_GAME_EventMax>* m_events;
	
	
	Layer* m_backgroundLayer;
	
	Layer* m_midgroundLayer;
	
	Layer* m_foregroundLayer;
	
	hdOrthographicProjection* m_projection;
	
	char m_levelName[kMaxLevelNameSize];
	
	
	FloorInfo *m_floorInfo;
	
	char m_nearBackgroundTexture[kMaxTexturePathSize];
	
	char m_farBackgroundTexture[kMaxTexturePathSize];
	
	char m_distantBackgroundTexture[kMaxTexturePathSize];
	
	char m_skyBackgroundTextureName[kMaxTexturePathSize];
	
	hdTexture *m_bgNearTexture;
	
	hdTexture *m_bgFarTexture;
	
	hdTexture *m_bgDistantTexture;
	
	hdTexture *m_skyTexture;
		
	float m_backgroundBottomTint[4];
	
	float m_backgroundTopTint[4];
	
	bool m_extendAABB;
	
	hdAABB m_extendedAABB_HACKHACKHACK;
	
	hdTexture *m_jointTexture;
	
	float m_floorLevelOffs;
	
	float m_floorDrawingLevel;
	
	/*
	 * Cached config values
	 */
    std::string c_effectsQualityLevel;
};

BOOST_CLASS_VERSION(Level, 12)


inline int Level::AddBlock(Block* block)
{
	return m_blocks->Add(block);
}


inline const bool Level::RemoveBlockAtIndex(const int index)
{
	return m_blocks->RemoveAtIndex(index);
}


inline const bool Level::RemoveBlock(const Block* block)
{		
	return m_blocks->Remove(block);
}


inline Block** Level::GetBlocks() const
{
	return m_blocks->GetItems();
}


inline const int Level::GetBlockCount() const
{
	return m_blocks->GetItemCount();
}


inline const int Level::GetBlockIndex(const Block *block)
{
	for (int i = 0; i < m_blocks->GetItemCount(); i++)
	{
		if (m_blocks->GetItems()[i] == block) return i;
	}
	return -1;
}


inline int Level::AddJack(const Jack* jack)
{
	return m_jacks->Add(jack);
}


inline int Level::AddJackAtIndex(const Jack* jack, const int index)
{
	return m_jacks->AddAtIndex(jack, index);
}


inline bool Level::RemoveJack(const Jack* jack)
{
	return m_jacks->Remove(jack);
}


inline bool Level::RemoveJackAtIndex(const int index)
{
	return m_jacks->RemoveAtIndex(index);
}


inline Jack** Level::GetJacks() const
{
	return m_jacks->GetItems();
}


inline const int Level::GetJackCount() const
{
	return m_jacks->GetItemCount();
}


inline int Level::AddJoint(const Joint* Joint)
{
	return m_joints->Add(Joint);
}


inline int Level::AddJointAtIndex(const Joint* Joint, const int index)
{
	return m_joints->AddAtIndex(Joint, index);
}


inline bool Level::RemoveJoint(const Joint* Joint)
{
	return m_joints->Remove(Joint);
}


inline bool Level::RemoveJointAtIndex(const int index)
{
	return m_joints->RemoveAtIndex(index);
}


inline Joint** Level::GetJoints() const
{
	return m_joints->GetItems();
}


inline const int Level::GetJointCount() const
{
	return m_joints->GetItemCount();
}


inline hdSerializablePointerList<Event, HDK_GAME_EventMax>* Level::GetEvents()
{
	return m_events;
}


inline const char* Level::GetLevelName() const
{
	return m_levelName;
}


inline void Level::SetLevelName(const char* name)
{
	if (strlen(name) == 0) 
	{
		snprintf(m_levelName, kMaxLevelNameSize, "(Unnamed)");
	}
	else
	{
		snprintf(m_levelName, kMaxLevelNameSize, "%s", name);
	}
}


inline const char* Level::GetSkyTextureName() const
{
	return m_skyBackgroundTextureName;
}


inline void Level::SetSkyTextureName(const char* textureName)
{
	if (strlen(textureName) == 0) 
	{
		snprintf(m_skyBackgroundTextureName, kMaxTexturePathSize, "%c", '\0');
	}
	else
	{
		snprintf(m_skyBackgroundTextureName, kMaxTexturePathSize, "%s", textureName);
	}
}


inline const char* Level::GetDistantBackgroundTextureName() const
{
	return m_distantBackgroundTexture;
}


inline void Level::SetDistantBackgroundTextureName(const char* textureName)
{
	if (strlen(textureName) == 0) 
	{
		snprintf(m_distantBackgroundTexture, kMaxTexturePathSize, "%c", '\0');
	}
	else
	{
		snprintf(m_distantBackgroundTexture, kMaxTexturePathSize, "%s", textureName);
	}
}


inline const char* Level::GetFarBackgroundTextureName() const
{
	return m_farBackgroundTexture;
}


inline void Level::SetFarBackgroundTextureName(const char* textureName)
{
	if (strlen(textureName) == 0) 
	{
		snprintf(m_farBackgroundTexture, kMaxTexturePathSize, "%c", '\0');
	}
	else
	{
		snprintf(m_farBackgroundTexture, kMaxTexturePathSize, "%s", textureName);
	}
}


inline const char* Level::GetNearBackgroundTextureName() const
{
	return m_nearBackgroundTexture;
}


inline void Level::SetNearBackgroundTextureName(const char* textureName)
{
	if (strlen(textureName) == 0) 
	{
		snprintf(m_nearBackgroundTexture, kMaxTexturePathSize, "%c", '\0');
	}
	else
	{
		snprintf(m_nearBackgroundTexture, kMaxTexturePathSize, "%s", textureName);
	}
}


inline const float* Level::GetBackgroundBottomTint() const
{
	return m_backgroundBottomTint;
}


inline void Level::SetBackgroundBottomTint(const int r, const int g, const int b)
{
	m_backgroundBottomTint[0] = r/255.0f;
	m_backgroundBottomTint[1] = g/255.0f;
	m_backgroundBottomTint[2] = b/255.0f;
}


inline const float* Level::GetBackgroundTopTint() const
{
	return m_backgroundTopTint;
}


inline void Level::SetBackgroundTopTint(const int r, const int g, const int b)
{
	m_backgroundTopTint[0] = r/255.0f;
	m_backgroundTopTint[1] = g/255.0f;
	m_backgroundTopTint[2] = b/255.0f;
}


// Floor info
inline const FloorInfo* Level::GetFloorInfo() const
{
	return m_floorInfo;
}


inline void Level::SetFloorInfo(FloorInfo *floorInfo)
{
	m_floorInfo = floorInfo;
}


inline bool Level::GetExtendAABB()
{
	return m_extendAABB;
}


inline void Level::SetExtendAABB(bool extendAABB)
{
	m_extendAABB = extendAABB;
}

#endif
