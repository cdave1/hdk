/*
 
 The level editor controller responds to item creation events on the editor area.
 
 The following events are most relevant at the moment
 - mouse down: 
	- if "Create mode": create a temporary game object
	- if "Cursor mode": select the game object currently
						under the user's cursor.
 - mouse move: draw a box between the start and end point.
	- create a temporary game object.
 - mouse up:
	- create a new totem block; if there are no overlaps
	- create a new game object.
 
 Methods:
 - Draw: draws all the game objects created thus far, called 
	as a step method by the main thread.
 
 */


#ifndef HD_LEVELEDITOR_CONTROLLER_H
#define HD_LEVELEDITOR_CONTROLLER_H

//#include "Box2D.h"

#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "hdk.h"
#include "totemGame.h"

#include "hdLevelEditorUtilities.h"





// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


class b2World;
class totemWorld;

struct Settings;

typedef enum {
	e_interfacePaletteModeCursor = 0,
	e_interfacePaletteModeAddTotemBlock,
	e_interfacePaletteModeTotemShape,
	e_interfacePaletteModeTotemJoint,
	e_interfacePaletteModeTotemJack,
	e_interfacePaletteModeLayerPolygonQuad,
	e_interfacePaletteModeLayerPolygonQuadActualSize,
	e_interfacePaletteModeLayerPolygon,
	e_interfacePaletteModeLayerParticle,
	e_interfacePaletteModeErase,
	e_interfacePaletteModeTotemEvent,
	e_interfacePaletteModeCount
} e_interfacePaletteMode;


typedef enum {
	e_interfaceLayerModeGameLayer = 0x0001,
	e_interfaceLayerModeBackground = 0x0002,
	e_interfaceLayerModeMidground = 0x0004,
	e_interfaceLayerModeForeground = 0x0008,
} e_interfaceLayerMode;



struct Settings
{
	Settings() :
	hz(60.0f),
	
	interfacePaletteMode(e_interfacePaletteModeCursor),
	interfaceLayerMode((int)e_interfaceLayerModeGameLayer),
	
	copiedBlockCount(0),
	copiedJointCount(0),
	//selectedGameObject(NULL),
	//copiedGameObject(NULL),
	//
	
	GLUTmodifiers(0),
	screenLower(0.0f, 0.0f),
	screenUpper(0.0f, 0.0f),
	newTotemJackType(e_totemJackTypeDefault),
	newTotemMaterial(e_totemMaterialWood),
	newTotemShapeType(e_totemShapeTypeRectangle),
	newTotemJointType(e_totemJointTypeDistance),
	newTotemBlockType(e_totemBlockTypeNormal),
	newTotemEventType(e_totemEventTypeFireGun),
	iterationCount(10),
	drawShapes(1),
	drawJoints(1),
	testPhysics(0),
	newTexturePath(0),
	currentWorldPath(0),
	deleteButtonTaps(0),
	showCosmetics(true),
	showGround(true),
	showPhysics(true)
	{
		newPolygonTint[0] = 1.0f;
		newPolygonTint[1] = 1.0f;
		newPolygonTint[2] = 1.0f;
		newPolygonTint[3] = 1.0f;
		//copiedGameObject = NULL;
		//copiedBlocks = 0;
	}
	
	float32 hz;
	e_interfacePaletteMode interfacePaletteMode;
	int interfaceLayerMode;
	
	int copiedBlockCount;
	int copiedJointCount;
	hdGameObject* DEPRECATEDselectedGameObject;
	hdGameObject* copiedGameObject;
	totemBlock** copiedBlocks;
	totemJoint** copiedJoints;
	
	
	int GLUTmodifiers;
	hdVec2 screenLower;
	hdVec2 screenUpper;
	e_totemJackType newTotemJackType;
	e_totemMaterial newTotemMaterial;
	e_totemShapeType newTotemShapeType;
	e_totemJointType newTotemJointType;
	e_totemBlockType newTotemBlockType;
	e_totemEventType newTotemEventType;
	int32 iterationCount;
	int32 drawShapes;
	int32 drawJoints;
	int32 testPhysics;
	char* newTexturePath;
	char* currentWorldPath;
	float newPolygonTint[4];
	int deleteButtonTaps;
	
	bool showCosmetics;
	bool showGround;
	bool showPhysics;
};

class hdLevelEditorController
{
	 
	
public:
	hdLevelEditorController();
	~hdLevelEditorController();
	

	void MouseDown(const hdVec2& p);
	void MouseMove(const hdVec2& p);
	void MouseUp();
	
	
	void Draw();
	
	void DrawString(int x, int y, const char *string, ...);	
	
	void SetModifiers(const int GLUTmodifiers);
	
	void DeleteKey();
	
	void Save();
	
	void SaveNewWorld(const char *name);
	
	void Load();
	
	void DoUnitTest();
	
	const totemBlock* GetSelectedTotemBlock() const;
	
	//hdGameObject* GetSelectedGameObject() const;
	
	void AddTotemBlock(totemBlock* block); //hdGameObjectDef& def, e_totemMaterial material, e_totemBlockType blockType);
	
	const totemLevel* GetCurrentLevel() const;
	
	const totemJoint* GetCurrentJoint() const;
	
	const bool SetCurrentLevel(const totemLevel* level);
	
	void ToggleLayerMode(const e_interfaceLayerMode layerMode);
	
	totemWorld* GetCurrentTotemWorld() const;
	
	void GenerateNewLevel();
	
	void FinishCurrentShape();
	
	void FinishCurrentPolygon();
	
	void ReverseCCWVertices(hdVec3 *vertices, const int vertexCount);
	
	void CopySelectedObjects();
	
	void PasteCopiedObjects();
	
	void CopyCurrentLevel();
	
	void PasteCopiedLevel();
	
	void ApplyCurrentTextureToSelected(const char* texture);
	
	void RepairSelectedShape();
	
	void DumpUniqueTextureList();
	
	hdTypedefList<hdGameObject*, 512>* m_selectedGameObjects;
	
	hdTypedefList<hdGameObject*, 512>* m_copiedGameObjects;
	
	
	Settings settings;
private:
	void DrawModeInfo();
	
	void ScaleSelectedObjects();
	
	void RotateSelectedObjects();
	
	void MoveSelectedObjects();
	
	void SelectSingleGameObject();
	
	void SelectMultipleGameObjects();
	
	
	void AddNewBlock();
	
	void AddNewJoint();
	
	void AddNewJack();
	
	void AddNewEvent();
	
	void AddNewLayerPolygonQuad();
	
	void AddNewLayerPolygon();
	
	totemBlock* CopyBlockToLevel(totemBlock *block);
	
	totemJoint* CopyJointToLevel(totemJoint *srcJoint);
	
	void InitPhysics();
	
	const bool BlockPassesFilter(const totemBlock *block) const;
	
	const bool CanSelectGameObject(const hdGameObject *object) const;
	
	
	totemLevel lev;
	
	int32 m_textLine;
	
	// box 2d stuff
	b2AABB m_physicsWorldAABB;
	b2World* m_physicsWorld;
	
	// 
	hdVec2 m_targetScreenArea; // we are drawing for the iPhone.
	hdVec2 m_targetScreenPosition;
	
	bool m_dragging;
	hdVec2 m_startClickPoint;
	hdVec2 m_previousMouseDragPoint;
	hdVec2 m_currentMouseDragPoint;
	
	// gameworld stuff
	hdOrthographicProjection *m_projection;
	
	hdGameWorld *m_gameWorld;
	
	totemLevel* m_currentLevel;
	
	totemWorld* m_totemWorld;
	
	//totemShape* m_currentShape;
	
	int m_nextTotemIndex;
	
	b2MouseJoint* m_mouseJoint;
	
	hdPolygon* m_currentLayerPolygon;
	
	totemBlock* m_currentBlockShape;
	
	totemJoint *m_currentJoint;
	
	totemEvent *m_currentEvent;
	
	// list of unique textures of all worlds loaded.
	std::map<string, string> uniqueTextures;
};


inline const totemBlock* hdLevelEditorController::GetSelectedTotemBlock() const
{
	if (settings.DEPRECATEDselectedGameObject == NULL) return NULL;
	if (settings.DEPRECATEDselectedGameObject->GetUserType() != (int)e_totemTypeBlock) return NULL;
	return (totemBlock *)settings.DEPRECATEDselectedGameObject;
}


inline const totemLevel* hdLevelEditorController::GetCurrentLevel() const
{
	return m_currentLevel;
}


inline const totemJoint* hdLevelEditorController::GetCurrentJoint() const
{
	return m_currentJoint;
}


inline totemWorld* hdLevelEditorController::GetCurrentTotemWorld() const
{
	return m_totemWorld;
}


#endif