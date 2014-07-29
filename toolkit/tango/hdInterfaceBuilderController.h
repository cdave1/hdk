


#ifndef HD_INTERFACE_BUILDER_CONTROLLER_H
#define HD_INTERFACE_BUILDER_CONTROLLER_H

#include <fstream>

#include <GLUT/glut.h>

#include "hdk.h"
#include "hdIB.h"

#include "hdInterfaceBuilderUtilities.h"

#define FILE_PATH_LEN 1024
#define MAX_SELECTED_OBJECTS 1024


// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>



struct Settings;

typedef enum {
	e_interfacePaletteModeCursor = 0,
	e_interfacePaletteModeAddContainer,
	e_interfacePaletteModeAddButton,
	e_interfacePaletteModeAddImage,
	e_interfacePaletteModeAddFontPolygon,
	e_interfacePaletteModeAddEvent,
	e_interfacePaletteModeCount
} e_interfacePaletteMode;


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
	
	interfacePaletteMode(e_interfacePaletteModeCursor),
	
	copiedObjectCount(0),
	screenLower(0.0f, 0.0f),
	screenUpper(0.0f, 0.0f),
	newTexturePath(0),
	pixelWidth(500),
	pixelHeight(500)
	{
		newPolygonTint[0] = 1.0f;
		newPolygonTint[1] = 1.0f;
		newPolygonTint[2] = 1.0f;
		newPolygonTint[3] = 1.0f;
		//copiedGameObject = NULL;
		//copiedBlocks = 0;
	}
	
	e_interfacePaletteMode interfacePaletteMode;
	int interfaceLayerMode;
	
	int copiedObjectCount;
	hdGameObject * DEPRECATEDselectedGameObject;
	hdGameObject * copiedGameObject;
	hdIBProxy ** copiedObjects;
	
	
	hdVec2 screenLower;
	hdVec2 screenUpper;
	char* newTexturePath;
	
	float newPolygonTint[4];
	
	float32 pixelWidth;
	float32 pixelHeight;
};

class hdInterfaceBuilderController
{
	 
	
public:
	hdInterfaceBuilderController();
	~hdInterfaceBuilderController();
	
	/**
	 * Saving/delete world files
	 */
	bool SaveCurrentInterface();
	bool SaveCurrentInterfaceTo(const char *destPath);
	bool LoadInterfaceFile(const char *path);
	bool CanSaveCurrentWorldToExistingFile();
	
	/**
	 * Input settings and functions
	 *
	 * TODO: device agnostic to support multitouch?
	 */
	void StartPan(const float32 x, const float32 y);
	void Pan(const float32 deltaX, float32 deltaY);
	void MouseDown(const int x, const int y);
	void MouseDrag(const int x, const int y);
	void MouseUp(bool shiftKeyDown);
	
	void SetDragStyleRotate();
	void SetDragStyleScale();
	void SetDragStyleMove();
	void Zoom(const float32 delta);
	void SetPerspectiveProjection();
	void SetOrthoProjection();

	void SetPaletteTexture(const char *texturePath);
	const char * GetPaletteTexture() const;
	void SetPaletteTint(const float r, const float g, const float b, const float a);
	
	
	//void DoubleTouchDown(
	//void DoubleTouchMove(const int ax, const int ay, const int bx, const int by);
	
	
	/**
	 * Drawing
	 */
	void Draw();
	
	void DrawString(int x, int y, const char *string, ...);	
	
	void SetModifiers(const int GLUTmodifiers);
	
	void DeleteKey();
	
	const hdGameObject * GetSelectedObject() const;
	
	//hdGameObject* GetSelectedGameObject() const;
	
	void AddGameObject(hdGameObject * object); //hdGameObjectDef& def, e_totemMaterial material, e_totemBlockType blockType);
	
	hdIBViewController * GetCurrentViewController() const;
	
	void GenerateNewEmptyViewControllerWithName(const char *name);
	
	void GenerateNewViewController();
	
	bool SaveCurrentViewController();
	
	bool SaveCurrentViewControllerTo(const char *destPath);
	
	bool LoadViewController(const char *path);
	
	bool CanSaveCurrentViewControllerToExistingFile();
	
	
	void FinishCurrentShape();
	
	void FinishCurrentPolygon();
	
	void ReverseCCWVertices(hdVec3 *vertices, const int vertexCount);
	
	void CopySelectedObjects();
	
	void SelectNone();
	
	void SelectAll();
	
	void PasteObjectsExternal(const hdIBProxy **objects, const unsigned int itemCount);
	
	void PasteCopiedObjects();
	
	void CopyCurrentViewController();
	
	void PasteCopiedViewController();
	
	const hdGameObject ** GetSelectedObjects();
	
	void ApplyCurrentTextureToSelected();
	
	void DumpUniqueTextureList();
	
	void Resize(float32 w, float32 h);
	
	hdTypedefList<hdIBProxy *, 1024>* m_selectedObjects;
	
	hdTypedefList<hdIBProxy *, 1024>* m_copiedObjects;
	
	hdIBProxy * CopyObjectToLevel(hdIBProxy * proxy);
	
		// Selected game object
	const uint32 GetSelectedInterfaceObjectsCount() const;
	
	const hdIBProxy * GetSelectedInterfaceObjectSingle() const;
	
	const hdTypedefList<hdIBProxy *, 1024> * GetSelectedInterfaceObjects() const;
	
	hdIBProxy * GetSelectedInterfaceObjectAtIndex(unsigned int index) const;
	
	bool SelectedItemsContainsType(const hdInterfaceBuilderProxyType_t type) const; 
	
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
	
	const bool SetCurrentViewController(const hdIBViewController * viewController);
	
	void AddNewObject();
	
	void AddNewEvent();
	
	//totemBlock* CopyBlockToLevel(totemBlock *block);
	
	const bool CanSelectInterfaceObject(const hdIBProxy *object) const;
	
	hdVec2 ConvertScreenToWorld(const float32 x, const float32 y);
	
	hdVec2 ConvertSizeToWorld(const float32 x, const float32 y); 
	
	
	int32 m_textLine;
	
	// Bookkeeping
	bool m_canSaveCurrentViewControllerToExistingFile;
	char m_currentViewControllerPath[FILE_PATH_LEN];
	
	// View port
	float32 m_viewZoom;
	hdVec2 m_viewCenter;
	float32 m_xExtents, m_yExtents;
	hdVec2 m_currentPan;
	hdVec2 m_prevPan;
	
	// Interaction
	e_dragStyle m_dragStyle;
	e_drawingStyle m_drawingStyle;
	
	// Target viewports.
	hdVec2 m_targetScreenArea; 
	hdVec2 m_targetScreenPosition;
	
	// Interaction
	bool m_dragging;
	hdVec2 m_startClickPoint;
	hdVec2 m_previousMouseDragPoint;
	hdVec2 m_currentMouseDragPoint;
	char m_currentPaletteTexture[FILE_PATH_LEN];
	
	// gameworld stuff
	hdOrthographicProjection *m_projection;
	
	hdIBViewController * m_currentViewController;
	
	hdPolygon* m_currentLayerPolygon;
	
	// list of unique textures of all worlds loaded.
	std::map<string, string> uniqueTextures;
};


inline hdIBViewController * hdInterfaceBuilderController::GetCurrentViewController() const
{
	return m_currentViewController;
}


#endif