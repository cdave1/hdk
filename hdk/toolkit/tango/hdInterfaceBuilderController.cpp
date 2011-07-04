#include "hdInterfaceBuilderController.h"

#define kScreenWidth 320.0f
#define kScreenHeight 480.0f
#define kScreenDepth 10.0f

hdInterfaceBuilderController::hdInterfaceBuilderController()
{
	hdConfig::LoadNullConfig();
	
	hdAABB projectionAABB;
	projectionAABB.lower = hdVec3(0,0,0) - hdVec3(kScreenWidth, kScreenHeight, -kScreenDepth);
	projectionAABB.upper = hdVec3(kScreenWidth, kScreenHeight, -kScreenDepth);
	m_projection = new hdOrthographicProjection(NULL, projectionAABB);
	
	m_viewZoom = 1.0f;
	
	
	
	m_xExtents = 240.0f, 
	m_yExtents = 240.0f;
	
	settings.DEPRECATEDselectedGameObject = NULL;
	settings.copiedGameObject = NULL;
	
	m_selectedObjects = new hdTypedefList<hdIBProxy *, 1024>();
	m_copiedObjects = new hdTypedefList<hdIBProxy *, 1024>();
	
	m_drawingStyle	= e_drawingStyleEditorFull; //e_drawingStyleEditorFull;

	
	
	m_currentViewController = NULL;
	
	
	m_currentViewController = new hdIBViewController("(Unnamed World)");
	m_currentViewController->SetDimensions(480.0f, 320.0f);
	m_viewCenter.Set(240.0f, 160.0f);
	
	m_canSaveCurrentViewControllerToExistingFile = false;
}



hdInterfaceBuilderController::~hdInterfaceBuilderController()
{
	hdTextureManager::TearDown();
}


void hdInterfaceBuilderController::GenerateNewViewController()
{
	m_currentViewController = new hdIBViewController();
}


bool hdInterfaceBuilderController::CanSaveCurrentViewControllerToExistingFile()
{
	// If it's a new world created in the constructor, then this is false.
	return m_canSaveCurrentViewControllerToExistingFile;
}


bool hdInterfaceBuilderController::SaveCurrentViewController()
{
	
	if (this->CanSaveCurrentViewControllerToExistingFile())
		return this->SaveCurrentViewControllerTo(m_currentViewControllerPath);
	return false;
}


bool hdInterfaceBuilderController::SaveCurrentViewControllerTo(const char *destPath)
{
	if (m_currentViewController == NULL) return false;
	if (strlen(destPath) == 0) return false;
	
	// save a backup
	if (!(hdIBLoader::Instance()->BackupViewControllerFile(destPath)))
	{
		// Log an error, but don't stop saving
		hdPrintf("Could not save a backup: %s", destPath);
	}
	
	// save text version
	if (!(hdIBLoader::Instance()->SaveViewController(m_currentViewController, destPath)))
	{
		hdPrintf("Could not save text for world to: %s", destPath);
		return false;
	}
	
	// save a binary version
#if 1
	hdPrintf("WARNING: not saving binary file.\n");
#else
	if (0 != (hdIBLoader::Instance()->SaveViewControllerBinary(m_currentViewController, destPath)))
	{
		hdPrintf("Could not save binary file for world at: %s", destPath);
		return false;
	}
#endif
	snprintf(m_currentViewControllerPath, FILE_PATH_LEN, "%s", destPath);
	m_canSaveCurrentViewControllerToExistingFile = true;
	return true;
}


bool hdInterfaceBuilderController::LoadViewController(const char *path)
{
	// Copy into currentWorldPath
	snprintf(m_currentViewControllerPath, FILE_PATH_LEN, "%s", path);
	
	// if m_totemWorld is not null, then save and dispose
	if (m_currentViewController != NULL)
	{
		hdPrintf("[I] Disposing of current totem world\n");
		
		//m_currentViewController
	}
	
	//m_totemWorld = totemWorldManager::Instance()->FindTotemWorld(m_currentWorldPath);
	if (!(m_currentViewController = hdIBLoader::Instance()->LoadViewControllerFromText(m_currentViewControllerPath)))
	{
		hdPrintf("Couldn't load file from text.\n");
		return false;
	}
	m_currentViewController->CreateInterfaceObjects();
	m_canSaveCurrentViewControllerToExistingFile = true;
	return true;
}


void hdInterfaceBuilderController::FinishCurrentShape()
{
	//m_currentLevel->AddShape(m_currentShape);
	//m_currentShape = NULL;
}


void hdInterfaceBuilderController::FinishCurrentPolygon()
{
	m_currentLayerPolygon = NULL;
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedObjects->RemoveAll();
}


const bool hdInterfaceBuilderController::SetCurrentViewController(const hdIBViewController* viewController)
{
	m_selectedObjects->RemoveAll();
	
	return true;
}


void hdInterfaceBuilderController::SelectMultipleGameObjects()
{
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedObjects->RemoveAll();
	
	hdIBProxy * objs[MAX_SELECTED_OBJECTS];
	
	hdAABB mouseAABB;
	hdVec3 bottom, top;
	
	bottom.Set(m_startClickPoint.x, m_startClickPoint.y, 50.0f);
	top.Set(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y, -50.0f);
	
	mouseAABB.lower = hdMin(bottom, top);
	mouseAABB.upper = hdMax(bottom, top);
	
	int clickObjectCount = m_currentViewController->AABBQuery(mouseAABB, objs, MAX_SELECTED_OBJECTS);
	
	
	hdVec3 mousePoint = hdVec2toVec3(m_currentMouseDragPoint);
	
	if (clickObjectCount == 0) return;
	
	if (clickObjectCount >= 1)
	{
		for (int i = 0; i < hdMin(clickObjectCount,MAX_SELECTED_OBJECTS); i++)
		{
			if (CanSelectInterfaceObject(objs[i]))
			{
				m_selectedObjects->Add(objs[i]);
			}
		}
	}
	
	if (m_selectedObjects->GetItemCount() > 0)
	{
		settings.DEPRECATEDselectedGameObject = m_selectedObjects->GetItems()[0];
	}
}



void hdInterfaceBuilderController::SelectSingleGameObject()
{
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedObjects->RemoveAll();
	
	hdIBProxy * objs[MAX_SELECTED_OBJECTS];
	
	hdAABB mouseAABB;
	hdVec3 bottom, top;
	
	bottom.Set(m_currentMouseDragPoint.x - 0.5f, m_currentMouseDragPoint.y - 0.5f, 50.0f);
	top.Set(m_currentMouseDragPoint.x + 0.5f, m_currentMouseDragPoint.y + 0.5f, -50.0f);
	
	mouseAABB.lower = hdMin(bottom, top);
	mouseAABB.upper = hdMax(bottom, top);
	
	unsigned int global = 0xFFFF;
	int clickObjectCount = m_currentViewController->AABBQuery(mouseAABB, objs, MAX_SELECTED_OBJECTS);
		
	hdVec3 mousePoint = hdVec2toVec3(m_currentMouseDragPoint);
	
	if (clickObjectCount == 0) return;
	
	if (clickObjectCount >= 1)
	{
		for (int i = 0; i < hdMin(clickObjectCount,MAX_SELECTED_OBJECTS); i++)
		{
			if (hdConcavePolygonContainsPoint(objs[i]->GetVertices(), objs[i]->GetVertexCount(), objs[i]->GetWorldCenter(),	mousePoint))
			{
				if (CanSelectInterfaceObject(objs[i]))
				{
					m_selectedObjects->Add(objs[i]);
					break;
				}
			}
		}
	}
	
	if (m_selectedObjects->GetItemCount() > 0)
	{
		settings.DEPRECATEDselectedGameObject = m_selectedObjects->GetItems()[0];
	}
}


const bool hdInterfaceBuilderController::CanSelectInterfaceObject(const hdIBProxy *object) const
{
	return true;
}



/*
 * Scale a group through a common central axis
 */
void hdInterfaceBuilderController::ScaleSelectedObjects()
{
	if (m_selectedObjects->GetItemCount() == 0) return;
	
	hdAABB groupAABB;
	groupAABB.lower.Set(100,100,0);
	groupAABB.upper.Set(-1000,-1000,0);
	
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i] != NULL)
		{
			groupAABB.lower = hdMin(groupAABB.lower, m_selectedObjects->GetItems()[i]->GetAABB().lower);
			groupAABB.upper = hdMax(groupAABB.upper, m_selectedObjects->GetItems()[i]->GetAABB().upper);
		}
	}
	
	hdVec3 center = groupAABB.lower + (0.5f * (groupAABB.upper - groupAABB.lower));
	
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i] != NULL)
		{
			hdVec3 boxPrev = hdVec2toVec3(m_previousMouseDragPoint) - center;
			hdVec3 boxCurr = hdVec2toVec3(m_currentMouseDragPoint) - center;
			
			boxPrev.Set(hdAbs(boxPrev.x), hdAbs(boxPrev.y), 1.0f);
			boxCurr.Set(hdAbs(boxCurr.x), hdAbs(boxCurr.y), 1.0f);			
			
			// Ensure square scaling so proportions are kept.
			hdVec3 scale(boxCurr.x / boxPrev.x, boxCurr.x / boxPrev.x, 1.0f);
			
			m_selectedObjects->GetItems()[i]->ScaleDelta(scale, center);
		}
	}
}



void hdInterfaceBuilderController::RotateSelectedObjects()
{
	if (m_selectedObjects->GetItemCount() == 0) return;
	
	// Rotate everything around the center of all the objects - find the aabb of all!
	hdAABB groupAABB;
	groupAABB.lower.Set(100,100,0);
	groupAABB.upper.Set(-1000,-1000,0);
	
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i] != NULL)
		{
			groupAABB.lower = hdMin(groupAABB.lower, m_selectedObjects->GetItems()[i]->GetAABB().lower);
			groupAABB.upper = hdMax(groupAABB.upper, m_selectedObjects->GetItems()[i]->GetAABB().upper);
		}
	}
	
	hdVec3 center = groupAABB.lower + (0.5f * (groupAABB.upper - groupAABB.lower));
	
	
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i] != NULL)
		{
			// Get angle between current and previous...
			//hdVec3 center = (m_selectedObjects->GetItems()[i]->GetWorldCenter());
			float delta = hdGetAngle(m_previousMouseDragPoint, m_currentMouseDragPoint, hdVec3toVec2(center));
			hdVec3 rot(0,0,delta);
			
			// TODO: Get rotation working.
			//m_selectedObjects->GetItems()[i]->RotateDelta(rot, center);
		}
	}
}



void hdInterfaceBuilderController::MoveSelectedObjects()
{
	if (m_selectedObjects->GetItemCount() == 0) return;
	
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i] != NULL)
		{
			
			hdVec3 delta = hdVec2toVec3(m_currentMouseDragPoint - m_previousMouseDragPoint);
			
			m_selectedObjects->GetItems()[i]->MoveDelta(delta);
		}
	}
}



void hdInterfaceBuilderController::AddNewObject()
{
	hdVec2 aa = hdMin(m_currentMouseDragPoint, m_startClickPoint);
	hdVec2 bb = hdMax(m_currentMouseDragPoint, m_startClickPoint);
	
	if (settings.interfacePaletteMode == e_interfacePaletteModeAddImage)
	{
		hdIBImage *image = new hdIBImage(this->GetPaletteTexture(), 
							   hdVec2toVec3(aa), hdVec2toVec3(bb));
		image->CreateInterfaceElement();
		m_currentViewController->GetProxyList()->Add(image);
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeAddButton)
	{
		hdIBButton *button = new hdIBButton(this->GetPaletteTexture(),
											this->GetPaletteTexture(),
											this->GetPaletteTexture(),
											hdVec2toVec3(aa), hdVec2toVec3(bb));
		button->CreateInterfaceElement();
		m_currentViewController->GetProxyList()->Add(button);
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeAddFontPolygon)
	{
		hdIBFontPolygon *font = new hdIBFontPolygon("Fonts/bebas20.f",
													hdVec2toVec3(aa), hdVec2toVec3(bb));
		font->CreateInterfaceElement();
		m_currentViewController->GetProxyList()->Add(font);
	}
	
	/*
	if (settings.newTotemShapeType == e_totemShapeTypeCustom || settings.newTotemShapeType == e_totemShapeTypeTriStrip)
	{
		if (m_currentBlockShape == NULL)
		{
			if (settings.newTotemMaterial == e_totemMaterialCustomTexture)
			{
				block = new totemBlock(m_gameWorld, m_physicsWorld, 
											   m_startClickPoint, m_startClickPoint, 
											   settings.newTotemMaterial, 
											   settings.newTotemShapeType,
											   settings.newTotemBlockType,
												   settings.newTexturePath);
			} 
			else
			{
				block = new totemBlock(m_gameWorld, m_physicsWorld, 
												   m_startClickPoint, m_startClickPoint, 
												   settings.newTotemMaterial, 
												   settings.newTotemShapeType,
												   settings.newTotemBlockType);
			}
			
			if (m_currentLevel->AddBlock(block) != -1)
			{
				//settings.DEPRECATEDselectedGameObject = block;
				m_selectedObjects->Add(block);
				m_currentBlockShape = block;
			}
		} 
		else 
		{
			m_currentBlockShape->AddPoint(m_startClickPoint.x, m_startClickPoint.y);
			m_currentBlockShape->ResetAABB();
		}
	}
	else 
	{
		if ((m_startClickPoint == m_currentMouseDragPoint) == false)
		{
			hdVec2 aa = hdMin(m_currentMouseDragPoint, m_startClickPoint);
			hdVec2 bb = hdMax(m_currentMouseDragPoint, m_startClickPoint);
			
			if (settings.newTotemMaterial == e_totemMaterialCustomTexture)
			{
				 block = new totemBlock(m_gameWorld, m_physicsWorld, 
												   aa, bb, 
												   settings.newTotemMaterial, 
												   settings.newTotemShapeType,
												   settings.newTotemBlockType,
												   settings.newTexturePath);
			}
			else
			{
				block = new totemBlock(m_gameWorld, m_physicsWorld, 
												   aa, bb, 
												   settings.newTotemMaterial, 
												   settings.newTotemShapeType,
												   settings.newTotemBlockType);				
			}
			
			//block->ResetAABB();
			//block->ResetTextureCoords();
			
			if (m_currentLevel->AddBlock(block) != -1)
			{
				//settings.DEPRECATEDselectedGameObject = block;
				m_selectedObjects->Add(block);
			}		
		}
	}*/
}

	

void hdInterfaceBuilderController::AddNewEvent()
{
	/*
	this->SelectSingleGameObject();
	if (settings.DEPRECATEDselectedGameObject == NULL) return;
	//if (settings.DEPRECATEDselectedGameObject->GetUserType() != (int)e_totemTypeBlock) return;
	
	if (m_currentEvent == NULL)
	{
		if (settings.DEPRECATEDselectedGameObject != NULL)
		{
			m_currentEvent = new totemEvent(m_gameWorld, settings.newTotemEventType, (hdGameObject *)settings.DEPRECATEDselectedGameObject, NULL);
		}
	}
	else
	{
		if (0 == m_currentEvent->SetSecondObject((const hdGameObject *)settings.DEPRECATEDselectedGameObject))
		{
			m_currentLevel->GetEvents()->Add(m_currentEvent);
			m_currentEvent = NULL;
		}
	}*/
}





void hdInterfaceBuilderController::MouseDown(const int x, const int y)
{
	hdVec2 p = ConvertScreenToWorld(x, y);
	
	m_startClickPoint.Set(p.x, p.y);
	m_currentMouseDragPoint = m_startClickPoint;
	m_previousMouseDragPoint = m_startClickPoint;
	
	// click selects a object.
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		if (m_selectedObjects->GetItemCount() > 0)
		{
			// Check if m_startClickPoint was inside any of the selected items. If
			// not, then deselected all.
			bool mouseInSelection = false;
			for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
			{
				if (hdConcavePolygonContainsPoint(m_selectedObjects->GetItems()[i]->GetVertices(), 
												  m_selectedObjects->GetItems()[i]->GetVertexCount(), 
												  m_selectedObjects->GetItems()[i]->GetWorldCenter(),	
												  hdVec2toVec3(m_startClickPoint)))
				{
					mouseInSelection = true;
					break;
				}
			}
			
			if (mouseInSelection == false) 
			{
				this->SelectSingleGameObject();
				///m_selectedObjects->RemoveAll();
				//settings.DEPRECATEDselectedGameObject = NULL;
			}
			//this->SelectSingleGameObject();
		}
		else
		{
			this->SelectSingleGameObject();
		}
		
		//if (m_selectedObjects->GetItemCount()) this->SelectSingleGameObject();	
	}
}



void hdInterfaceBuilderController::DeleteKey()
{
	if (m_selectedObjects->GetItemCount() == 0) return;
	
	bool res = false;
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		m_currentViewController->GetProxyList()->Remove(m_selectedObjects->GetItems()[i]);		
	}
	
	m_selectedObjects->RemoveAll();
	settings.DEPRECATEDselectedGameObject = NULL;
}



hdVec2 hdInterfaceBuilderController::ConvertScreenToWorld(const float32 x, const float32 y)
{
	hdPrintf("\n%2.2f, %2.2f\n", x, y);
	
	// find the position of hit as if it was on a screen with origin at x=0,y=0
	float32 dx = x / float32(settings.pixelWidth);
	float32 dy = y / float32(settings.pixelHeight);
	hdPrintf("%2.2f, %2.2f\n", dx, dy);
	
	// find out how far actual screen center is from (0,0)
	hdVec2 extents(m_xExtents, m_yExtents);
	extents *= m_viewZoom;
	
	hdVec2 lower = m_viewCenter - extents;
	hdVec2 upper = m_viewCenter + extents;
	hdVec2 box = upper - lower;
	
	hdVec2 p(lower.x + (dx * box.x), lower.y + (dy * box.y));
	hdPrintf("%2.2f, %2.2f\n", m_viewCenter.x, m_viewCenter.y);
	hdPrintf("%2.2f, %2.2f\n", p.x, p.y);
	return p;
}


hdVec2 hdInterfaceBuilderController::ConvertSizeToWorld(const float32 x, const float32 y)
{
	float32 dx = x / float32(settings.pixelWidth);
	float32 dy = y / float32(settings.pixelHeight);
	
	hdVec2 extents(m_xExtents, m_yExtents);
	extents *= m_viewZoom;
	
	hdVec2 lower = m_viewCenter - extents;
	hdVec2 upper = m_viewCenter + extents;
	hdVec2 box = upper - lower;
	
	hdVec2 p((dx * box.x), (dy * box.y));
	return p;
}


void hdInterfaceBuilderController::SetDragStyleRotate()
{
	this->SetDragStyle(e_dragStyleRotate);
}


void hdInterfaceBuilderController::SetDragStyleScale()
{
	this->SetDragStyle(e_dragStyleScale);
}


void hdInterfaceBuilderController::SetDragStyleMove()
{
	this->SetDragStyle(e_dragStyleMove);
}


void hdInterfaceBuilderController::SetDragStyle(e_dragStyle style)
{
	m_dragStyle = style;
}


void hdInterfaceBuilderController::MouseDrag(const int x, const int y)
{
	hdVec2 p = ConvertScreenToWorld(x, y);
	
	m_previousMouseDragPoint = m_currentMouseDragPoint;
	m_currentMouseDragPoint.Set(p.x, p.y);
	
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		if (m_dragStyle == e_dragStyleRotate)
		{
			this->RotateSelectedObjects();
		}
		else if (m_dragStyle == e_dragStyleScale)
		{
			this->ScaleSelectedObjects();
		}
		else if (m_dragStyle == e_dragStyleMove)
		{
			this->MoveSelectedObjects();
		}
	}
}


void hdInterfaceBuilderController::StartPan(const float32 x, const float32 y)
{

}


void hdInterfaceBuilderController::Pan(const float32 deltaX, float32 deltaY)
{
	hdVec2 diff = this->ConvertSizeToWorld(deltaX, deltaY);
	
	m_viewCenter.x -= (diff.x);
	m_viewCenter.y -= (diff.y);
}


void hdInterfaceBuilderController::Zoom(const float32 delta)
{
	//hdVec2 p = this->ConvertSizeToWorld(0, delta);
	m_viewZoom -= delta * 0.001f;
}


const uint32 hdInterfaceBuilderController::GetSelectedInterfaceObjectsCount() const
{
	return m_selectedObjects->GetItemCount();
}

const hdIBProxy * hdInterfaceBuilderController::GetSelectedInterfaceObjectSingle() const
{
	if (this->GetSelectedInterfaceObjectsCount() == 1)
		return m_selectedObjects->GetItems()[0];
	return NULL;
}

const hdTypedefList<hdIBProxy *, 1024> * hdInterfaceBuilderController::GetSelectedInterfaceObjects() const
{
	return m_selectedObjects;
}


hdIBProxy * hdInterfaceBuilderController::GetSelectedInterfaceObjectAtIndex(unsigned int index) const
{
	return m_selectedObjects->GetItems()[index];
}


bool hdInterfaceBuilderController::SelectedItemsContainsType(const hdInterfaceBuilderProxyType_t type) const
{
	bool containsBlocks = false;
	for (int i = 0; i < this->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ((containsBlocks = (m_selectedObjects->GetItems()[i]->GetInterfaceBuilderProxyType() == type))) break;
	}
	return containsBlocks;
}


void hdInterfaceBuilderController::SetPaletteTexture(const char *texturePath)
{
	if (strlen(texturePath) > 0)
	{
		snprintf(m_currentPaletteTexture, FILE_PATH_LEN, "%s", texturePath);
	}
}


const char * hdInterfaceBuilderController::GetPaletteTexture() const
{
	return m_currentPaletteTexture;
}


void hdInterfaceBuilderController::SetPaletteTint(const float r, const float g, const float b, const float a)
{
}


// TODO: Refactor - switch and put into separate functions.
void hdInterfaceBuilderController::MouseUp(bool shiftKeyDown)
{
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		if (shiftKeyDown)
		{
			this->SelectMultipleGameObjects();
		}
		else
		{
			// Need to reject the mouse up selection event when we have a selected
			// group. Reject if the mouse up event occurred while over any member
			// of the selected set.
			if (m_selectedObjects->GetItemCount() > 0)
			{
				// Check if m_startClickPoint was inside any of the selected items. If
				// not, then deselected all.
				bool mouseInSelection = false;
				for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
				{
					if (hdConcavePolygonContainsPoint(m_selectedObjects->GetItems()[i]->GetVertices(), 
													  m_selectedObjects->GetItems()[i]->GetVertexCount(), 
													  m_selectedObjects->GetItems()[i]->GetWorldCenter(),	
													  hdVec2toVec3(m_currentMouseDragPoint)))
					{
						mouseInSelection = true;
						break;
					}
				}
				
				if (mouseInSelection == false) 
				{
					this->SelectSingleGameObject();
				}
				//this->SelectSingleGameObject();
			}
			else
			{
				this->SelectSingleGameObject();
			}
		}
	} 
	
	this->AddNewObject();
	
	m_currentMouseDragPoint = m_startClickPoint;
	
	// Hold down shift to complete a shape.
	if (shiftKeyDown)
	{
		/*
		if (m_currentBlockShape == NULL) return;
		
		// Ensure vertices are ordered correctly.
		ReverseCCWVertices(m_currentBlockShape->GetVertices(), m_currentBlockShape->GetVertexCount());
		m_currentBlockShape->SetStartingVerticesToCurrent();
		m_currentBlockShape->ResetTextureCoords();
		*/
		// stop shape editing
		//m_currentBlockShape = NULL;
		//m_currentLayerPolygon = NULL;
		settings.DEPRECATEDselectedGameObject = NULL;
		m_selectedObjects->RemoveAll();
	}
}


void hdInterfaceBuilderController::ReverseCCWVertices(hdVec3 *vertices, const int vertexCount)
{
	if (vertexCount <= 2) return;
	
	signed int contains = 0;
	for (int i = 0; i < vertexCount; i++)
	{
		contains += hdCrossProductSign(vertices[i], vertices[(i+1)%vertexCount], vertices[(i+2)%vertexCount]); 
	}
	
	// reverse
	if (contains > 0)
	{
		hdVec3 tmp;
		for (int i = 1; i < floor((vertexCount+1)/2); i++)
		{
			tmp.Set(vertices[i].x, vertices[i].y, vertices[i].z);
			vertices[i].Set(vertices[vertexCount - i].x, vertices[vertexCount - i].y, vertices[vertexCount - i].z);
			vertices[vertexCount - i].Set(tmp.x, tmp.y, tmp.z);
		}
	}
}


void hdInterfaceBuilderController::CopySelectedObjects()
{
	m_copiedObjects->RemoveAll();
	
	if (m_selectedObjects->GetItemCount() == 0) return;
	
	for (int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		m_copiedObjects->Add(m_selectedObjects->GetItems()[i]);
	}
	
	//settings.copiedGameObject = settings.DEPRECATEDselectedGameObject;
}


void hdInterfaceBuilderController::SelectAll()	
{
	// TODO
	//m_startClickPoint.Set(m_currentLevel->GetAABB().lower.x, m_currentLevel->GetAABB().lower.y);
	//m_currentMouseDragPoint.Set(m_currentLevel->GetAABB().upper.x, m_currentLevel->GetAABB().upper.y);
	//this->SelectMultipleGameObjects();
}


void hdInterfaceBuilderController::SelectNone()
{
	m_selectedObjects->RemoveAll();
}


void hdInterfaceBuilderController::PasteObjectsExternal(const hdIBProxy **objects, const unsigned int itemCount)
{
	m_copiedObjects->RemoveAll();
	
	for (int i = 0; i < itemCount; i++)
	{
		m_copiedObjects->Add((hdIBProxy *)objects[i]);
	}
	
	this->PasteCopiedObjects();
}


void hdInterfaceBuilderController::PasteCopiedObjects()
{
	/*
	if (m_copiedObjects->GetItemCount() == 0) return;
	
	m_selectedObjects->RemoveAll();
	settings.copiedObjectCount = 0;
	
	for (int i = 0; i < m_copiedObjects->GetItemCount(); i++)
	{
		if (m_copiedObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			m_selectedObjects->Add(
									   this->CopyBlockToLevel((totemBlock *)m_copiedObjects->GetItems()[i])
									   );
			++settings.copiedObjectCount;
		}
	}
	 */
}


const hdGameObject ** hdInterfaceBuilderController::GetSelectedObjects()
{
	assert(m_selectedObjects);
	if (m_selectedObjects->GetItemCount() > 0)
		return (const hdGameObject **)m_selectedObjects->GetItems();
	return NULL;
}


void hdInterfaceBuilderController::CopyCurrentViewController()
{
	//settings.copiedObjects = (m_currentLevel)->GetBlocks();
	//settings.copiedObjectCount = (m_currentLevel)->GetBlockCount();
}


void hdInterfaceBuilderController::PasteCopiedViewController()
{
	/*
	if (settings.copiedObjects == NULL) return;
	
	for (int i = 0; i < this->settings.copiedObjectCount; i++)
	{
		this->CopyBlockToLevel(settings.copiedObjects[i]);
	}
	
	settings.copiedObjects = NULL;
	 */
}


void hdInterfaceBuilderController::ApplyCurrentTextureToSelected()
{
	/*
	if (m_selectedObjects->GetItemCount() == 0) return;
	if (strlen(m_currentPaletteTexture) == 0) return;
	
	totemBlock* selected = NULL;
	for(int i = 0; i < m_selectedObjects->GetItemCount(); i++)
	{
		if (m_selectedObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			selected = (totemBlock *)m_selectedObjects->GetItems()[i];
			if (selected != NULL && selected->IsTextureChangeable())
			{
				selected->SetTextureName(m_currentPaletteTexture);
				selected->ResetTextureCoords();
			}
		}
	}
	 */
}


void hdInterfaceBuilderController::DumpUniqueTextureList()
{
	/*
	int i, j;
	
	for (i = 0; i < m_currentViewController->GetProxyList()->GetItemCount(); ++i)
	{
		for (j = 0; j < lev->GetBlockCount(); ++j)
		{
			block = (totemBlock *)lev->GetBlocks()[j];
			
			if (block->IsTextureChangeable())
			{
				uniqueTextures[string(block->GetTextureName())] = string(block->GetTextureName());
			}
		}
	}
	
	map<string, string>::iterator iter;
	
	for (iter = uniqueTextures.begin(); 
		 iter != uniqueTextures.end(); 
		 iter++)
	{
		printf("%s\n", (*iter).first.c_str());
	}
	 */
}


void hdInterfaceBuilderController::Resize(float32 w, float32 h)
{
	settings.pixelWidth = hdMax((float32)200.0f, w);
	settings.pixelHeight = hdMax((float32)200.0f, h);
	
	m_xExtents = 0.5f * settings.pixelWidth; 
	m_yExtents = 0.5f * settings.pixelHeight;
}


hdIBProxy * hdInterfaceBuilderController::CopyObjectToLevel(hdIBProxy *proxy)
{
	//res = m_currentLevel->RemoveBlock((totemBlock *)settings.DEPRECATEDselectedGameObject);
	/*
	totemBlock *block = new totemBlock(m_gameWorld, m_physicsWorld, 
									   hdVec2(0,0), 
									   hdVec2(0,0), 
									   (srcBlock)->GetMaterial(), 
									   (srcBlock)->GetShapeType(),
									   (srcBlock)->GetBlockType());
	
	block->SetTextureName((srcBlock)->GetTextureName());
	block->SetDepth((srcBlock)->GetDepth());
	
	block->SetTint((srcBlock)->GetTint()[0],
				   (srcBlock)->GetTint()[1],
				   (srcBlock)->GetTint()[2],
				   (srcBlock)->GetTint()[3]);
	
	block->RemoveAllPoints();
	block->SetTag(srcBlock->GetTag());
	
	for (int i = 0; i < srcBlock->GetVertexCount(); i++)
	{
		block->AddPoint(srcBlock->GetVertices()[i].x,
						srcBlock->GetVertices()[i].y + 2.0f,
						srcBlock->GetVertices()[i].z);
	}
	
	
	block->SetZOffset(srcBlock->GetZOffset());
	block->SetTextureRepeatX(srcBlock->GetTextureRepeatX());
	block->SetTextureRepeatY(srcBlock->GetTextureRepeatY());
	
	block->ResetAABB();
	block->ResetOBB();
	block->ResetPhysicsBody();
	block->ResetTextureCoords();
	
	if (m_currentLevel->AddBlock(block) == -1)
	{
		hdPrintf("Block copy failed: level already has a full contingent of bricks.\n");
		delete block;
		return NULL;
	}
	return block;
	 */
	return NULL;
}






void hdInterfaceBuilderController::DrawModeInfo()
{
	/*
	if (settings.DEPRECATEDselectedGameObject != NULL)
	{
		this->DrawString(600, 40, "Selected: Block");
	}
	
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		this->DrawString(0, 20, "Cursor");
	} 
	else if (settings.interfacePaletteMode == e_interfacePaletteModeAddTotemBlock)
	{
		this->DrawString(0, 20, "Add Totem Block: Texture: %s", settings.newTexturePath);
	} 
	else if (settings.interfacePaletteMode == e_interfacePaletteModeTotemJack)
	{
		this->DrawString(0, 20, "Add Totem Jack");
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygonQuad)
	{
		this->DrawString(0, 20, "Add Polygon Quad: Texture: %s", settings.newTexturePath);
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygon)
	{
		this->DrawString(0, 20, "Add Polygon: Texture: %s", settings.newTexturePath);
	}
	
	if (settings.interfaceLayerMode & e_interfaceLayerModeForeground)
	{
		this->DrawString(800, 20, "Active Layer: Foreground");
	}
	else if (settings.interfaceLayerMode & e_interfaceLayerModeMidground)
	{
		this->DrawString(800, 20, "Active Layer: Midground");
	}
	else if (settings.interfaceLayerMode & e_interfaceLayerModeBackground)
	{
		this->DrawString(800, 20, "Active Layer: Background");
	}
	 */
}



void hdInterfaceBuilderController::DrawString(int x, int y, const char *string, ...)
{
	char buffer[128];
	
	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(0, settings.pixelWidth, settings.pixelHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity(); 
	
	glColor3f(0.4f, 0.5f, 1.0f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]);
	}
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
}


void hdInterfaceBuilderController::SetupProjection()
{
	hdVec2 extents(m_xExtents, m_yExtents);
	extents *= m_viewZoom;
	
	hdVec2 lower = m_viewCenter - extents;
	hdVec2 upper = m_viewCenter + extents;
	
	settings.screenLower = lower;
	settings.screenUpper = upper;
	glViewport(0, 0, settings.pixelWidth, settings.pixelHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(lower.x, upper.x, lower.y, upper.y, -1000.0, 1000.0);
	
	hdVec2 box = upper - lower;
}



void hdInterfaceBuilderController::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
	
	this->SetupProjection();
	
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	this->DrawModeInfo();
	
	hdglError("sss");
	
	glMatrixMode(GL_MODELVIEW);
	glPointSize(8.0f);
	glDisable(GL_TEXTURE_2D);
	
	// Draw a grid
	glBegin(GL_LINES);
	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	const float squareBoxSize = 5.0f;
	for (float x = floor(settings.screenLower.x); x <= ceil(settings.screenUpper.x); x += squareBoxSize)
	{
		glVertex2f(x, settings.screenLower.y);
		glVertex2f(x, settings.screenUpper.y);
	}
	
	for (float y = floor(settings.screenLower.y); y <= ceil(settings.screenUpper.y); y += squareBoxSize)
	{
		glVertex2f(settings.screenLower.x, y);
		glVertex2f(settings.screenUpper.x, y);
	}
	glEnd();

	
	if ((m_startClickPoint == m_currentMouseDragPoint) == false)
	{
		//glPointSize(8.0f);
		glBegin(GL_POINTS);
		glVertex2f(m_startClickPoint.x, m_startClickPoint.y);
		glVertex2f(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y);
		glEnd();
		
		glBegin(GL_LINE_LOOP);		
		hdVec2 aa = hdMin(m_currentMouseDragPoint, m_startClickPoint);
		hdVec2 bb = hdMax(m_currentMouseDragPoint, m_startClickPoint);
		
		glVertex2f(aa.x, aa.y);
		glVertex2f(bb.x, aa.y);
		glVertex2f(bb.x, bb.y);
		glVertex2f(aa.x, bb.y);
		glEnd();
	}
	
	
	//showCosmetics;
	//bool showGround;
	//bool showPhysics
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
	
	
	m_currentViewController->Draw();
	
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	
	hdglError("aaa");
	/*
	const int k_maxCount = 10;
	hdGameObject * objs[k_maxCount];
	int clickObjectCount = m_gameWorld->PointQuery(hdVec2toVec3(m_currentMouseDragPoint), objs, k_maxCount);
	glDisable(GL_TEXTURE_2D);
	hdglError("bbb");
	glEnable(GL_POINT_SPRITE_ARB);
	hdglError("ccc");
	if (clickObjectCount > 0)
	{
		for (int i = 0; i < hdMin(clickObjectCount,k_maxCount); i++)
		{
			if (objs[i] != m_projection) 
			{
				// do more fine-grained lookup
				if (hdConcavePolygonContainsPoint(objs[i]->GetVertices(), objs[i]->GetVertexCount(), objs[i]->GetWorldCenter(),	hdVec2toVec3(m_currentMouseDragPoint)))
				{
					hdglBegin(GL_POINTS);
					//glPointSize(4.0f);
					hdglColor4f(0.0f, 0.0f, 1.0f, 1.0f);
					hdglVertex2f(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y);
					hdglEnd();
					hdglBegin(GL_LINE_LOOP);
					hdglColor4f(1.0f, 0.9f, 0.3f, 1.0f);
					hdglVertex2f(objs[i]->GetAABB().lower.x, objs[i]->GetAABB().lower.y);
					hdglVertex2f(objs[i]->GetAABB().lower.x, objs[i]->GetAABB().upper.y);
					hdglVertex2f(objs[i]->GetAABB().upper.x, objs[i]->GetAABB().upper.y);
					hdglVertex2f(objs[i]->GetAABB().upper.x, objs[i]->GetAABB().lower.y);
					hdglEnd();
					break;
				}
			}
		}
		
	} 
	hdglError("eee");*/
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	hdglError("xxx");

	
	glDisable(GL_TEXTURE_2D);
	if (m_selectedObjects->GetItemCount() > 0)
	{
		for (int i = 0; i < m_selectedObjects->GetItemCount(); i++)
		{
			hdDrawGameObjectHandles(m_selectedObjects->GetItems()[i]);
		}
	}
	else if (settings.DEPRECATEDselectedGameObject != NULL)
	{
		hdDrawGameObjectHandles(settings.DEPRECATEDselectedGameObject);
	}
	
	float viewControllerWidth = m_currentViewController->GetWidth();
	float viewControllerHeight = m_currentViewController->GetHeight();
	
	hdglError("yyy");
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 0);
	glVertex2f(0, viewControllerHeight);
	glVertex2f(viewControllerWidth, viewControllerHeight);
	glVertex2f(viewControllerWidth, 0);
	glEnd();
	
	glPopMatrix();
	
	hdglError("zzz");
	
	
	glPopMatrix();
	
	glFlush();
}
