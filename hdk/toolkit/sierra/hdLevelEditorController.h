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

#include <GLUT/glut.h>

#include "hdk.h"
#include "totemGame.h"

#include "hdLevelEditorUtilities.h"

#define TOTEM_WORLD_TEXTFILE_EXTENSION ".ttw"




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


typedef enum
{
	e_dragStyleMove = 0,
	e_dragStyleRotate,
	e_dragStyleScale,
} e_dragStyle;


typedef enum
{
	e_drawingStyleEditorFull = 0,
	e_drawingStyleEditorWireframe,
	e_drawingStylePreview,
	e_drawingStyleCount,
} e_drawingStyle;



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
	deleteButtonTaps(0),
	isPerspectiveProjection(false),
	pixelWidth(500),
	pixelHeight(500),
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
	
	float newPolygonTint[4];
	int deleteButtonTaps;
	bool isPerspectiveProjection;
	
	float32 pixelWidth;
	float32 pixelHeight;
	
	bool showCosmetics;
	bool showGround;
	bool showPhysics;
	
	char currentWorldPath[256];
	
	
};

class hdLevelEditorController
{
	 
	
public:
	hdLevelEditorController();
	~hdLevelEditorController();
	
	void StartPan(const float32 x, const float32 y);
	void Pan(const float32 deltaX, float32 deltaY);
	
	
	
	void MouseDown(const int x, const int y);
	void MouseDrag(const int x, const int y);
	void MouseUp();
	
	bool isPhysicsOn();
	void PhysicsOn();
	void PhysicsOff();
	
	void SetPaletteTexture(const char *texturePath);
	
	void SetPaletteTint(const float r, const float g, const float b, const float a);
	
	
	//void DoubleTouchDown(
	//void DoubleTouchMove(const int ax, const int ay, const int bx, const int by);
	
	
	
	void Draw();
	
	void DrawString(int x, int y, const char *string, ...);	
	
	void SetModifiers(const int GLUTmodifiers);
	
	void DeleteKey();
	
	void Save();
	
	void SaveNewWorld(const char *name);
	
	bool LoadWorld(const char *path);
	
	void DoUnitTest();
	
	const totemBlock* GetSelectedTotemBlock() const;
	
	//hdGameObject* GetSelectedGameObject() const;
	
	void AddTotemBlock(totemBlock* block); //hdGameObjectDef& def, e_totemMaterial material, e_totemBlockType blockType);
	
	const bool SetLevel(const uint32 levelId);
	
	const totemLevel* GetCurrentLevel() const;
	
	const totemJoint* GetCurrentJoint() const;
	
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
	
	void Resize(float32 w, float32 h);
	
	hdTypedefList<hdGameObject*, 1024>* m_selectedGameObjects;
	
	hdTypedefList<hdGameObject*, 1024>* m_copiedGameObjects;
	
	void SetDragStyleRotate();
	void SetDragStyleScale();
	void SetDragStyleMove();
	void Zoom(const float32 delta);
	void TogglePerspectiveProjection();
	void SetPerspectiveProjection();
	void SetOrthoProjection();
	
	void SetNewBlockShapeType(e_totemShapeType shapeType);
	
	void SetNewBlockMaterialType(e_totemMaterial material);
	
	void SetNewBlockType(e_totemBlockType blockType);
	
	void SetNewJointType(e_totemJointType jointType);
	
	// Selected game object
	const uint32 GetSelectedGameObjectsCount() const;
	
	const hdGameObject * GetSelectedGameObjectSingle() const;
	
	const hdTypedefList<hdGameObject *, 1024> * GetSelectedGameObjects() const;
	
	hdGameObject *GetSelectedGameObjectAtIndex(unsigned int index) const;
	
	Settings settings;
	

	
private:
	
	void SetDragStyle(e_dragStyle style);
	
	void SetupProjection();
	
	void DrawModeInfo();
	
	void ScaleSelectedObjects();
	
	void RotateSelectedObjects();
	
	void MoveSelectedObjects();
	
	void SelectSingleGameObject();
	
	void SelectMultipleGameObjects();
	
	const bool SetCurrentLevel(const totemLevel* level);
	
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
	
	hdVec2 ConvertScreenToWorld(const float32 x, const float32 y);
	
	hdVec2 ConvertSizeToWorld(const float32 x, const float32 y); 
	
	totemLevel lev;
	
	int32 m_textLine;
	
	
	// View port
	float32 m_viewZoom;
	hdVec2 m_viewCenter;
	float32 m_xExtents, m_yExtents;
	hdVec2 m_currentPan;
	hdVec2 m_prevPan;
	
	// Interaction
	e_dragStyle m_dragStyle;
	e_drawingStyle m_drawingStyle;
	
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