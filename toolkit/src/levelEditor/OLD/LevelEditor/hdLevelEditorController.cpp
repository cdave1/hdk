#include "hdLevelEditorController.h"

#define kScreenWidth 3.2f
#define kScreenHeight 4.8f
#define kScreenDepth 10.0f

hdLevelEditorController::hdLevelEditorController()
{
	FileSystem_Init("/Users/david/Documents/Projects/TotemGames/res");
	
	hdConfig::LoadNullConfig();
	
	m_targetScreenArea = hdVec2(3.2f, 4.8f);
	m_targetScreenPosition = hdVec2(-1.6f, -2.4f);
	
	m_physicsWorld = NULL;
	
	this->InitPhysics();
	
	hdAABB aabb;
	aabb.lower.Set(0.0f, 0.0f, 0.0f);
	aabb.upper.Set(100.0f, 100.0f, 100.0f);
	m_gameWorld = new hdGameWorld(aabb);
	
	hdAABB projectionAABB;
	projectionAABB.lower = hdVec3(0,0,0) - hdVec3(kScreenWidth, kScreenHeight, -kScreenDepth);
	projectionAABB.upper = hdVec3(kScreenWidth, kScreenHeight, -kScreenDepth);
	m_projection = new hdOrthographicProjection(m_gameWorld, projectionAABB);
	
	
	settings.DEPRECATEDselectedGameObject = NULL;
	settings.copiedGameObject = NULL;
	settings.copiedBlocks = NULL;
	settings.copiedJoints = NULL;
	settings.newTexturePath = "";
	m_mouseJoint = NULL;
	
	m_selectedGameObjects = new hdTypedefList<hdGameObject*, 512>();
	m_copiedGameObjects = new hdTypedefList<hdGameObject*, 512>();
	/*
	m_bomb = NULL;
	m_textLine = 30;
	
	m_pointCount = 0;
	
	m_destructionListener.test = this;
	m_boundaryListener.test = this;
	m_contactListener.test = this;
	m_world->SetDestructionListener(&m_destructionListener);
	m_world->SetBoundaryListener(&m_boundaryListener);
	m_world->SetContactListener(&m_contactListener);
	 */
	//glEnable(GL_TEXTURE_2D);
	m_currentLevel = NULL;
	m_totemWorld = NULL;
	m_currentJoint = NULL;
	m_currentBlockShape = NULL;
	m_currentEvent = NULL;
}



hdLevelEditorController::~hdLevelEditorController()
{
	// By deleting the world, we delete the bomb, mouse joint, etc.
	delete m_physicsWorld;
	m_physicsWorld = NULL;
	
	m_gameWorld->RemoveAllGameObjects();
	
	hdTextureManager::TearDown();
	totemWorldManager::TearDown();
	
	delete m_gameWorld;
}


void hdLevelEditorController::InitPhysics()
{
	if (m_gameWorld != NULL)
	{
		//m_gameWorld->RemoveAllGameObjects();
		delete m_gameWorld;
		m_gameWorld = NULL;
	}
	
	hdAABB aabb;
	aabb.lower.Set(0.0f, 0.0f, 0.0f);
	aabb.upper.Set(100.0f, 100.0f, 100.0f);
	m_gameWorld = new hdGameWorld(aabb);
	
	if (m_physicsWorld != NULL)
	{
		delete m_physicsWorld;
		m_physicsWorld = NULL;
		
		if (m_selectedGameObjects != NULL) 
			m_selectedGameObjects->RemoveAll();
		/*
		if (m_copiedGameObjects != NULL)
			m_copiedGameObjects->RemoveAll();
		 */
	}
	
	m_physicsWorldAABB.lowerBound.Set(-256.0f, -256.0f);
	m_physicsWorldAABB.upperBound.Set(256.0f, 256.0f);
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	bool doSleep = true;
	m_physicsWorld = new b2World(m_physicsWorldAABB, gravity, doSleep);
}


void hdLevelEditorController::GenerateNewLevel()
{
	totemLevel *level = new totemLevel();
	level->InitLayers();
	
	// Init with barrier blocks:
	totemBlock* groundBlock = new totemBlock(m_gameWorld, m_physicsWorld, 
											 hdVec2(-8.0f, -2.4f), hdVec2(8.0f, -2.3f), 
											 e_totemMaterialEarth, 
											 e_totemShapeTypeRectangle,
											 e_totemBlockTypeBoundary);
	level->AddBlock(groundBlock);
	
	totemBlock* leftBarrierBlock = new totemBlock(m_gameWorld, m_physicsWorld, 
											 hdVec2(-8.1f, -2.4f), hdVec2(-8.0f, 0.0f), 
											 e_totemMaterialEarth, 
											 e_totemShapeTypeRectangle,
											e_totemBlockTypeBoundary);	
	level->AddBlock(leftBarrierBlock);
	
	totemBlock* rightBarrierBlock = new totemBlock(m_gameWorld, m_physicsWorld, 
												  hdVec2(8.0f, -2.4f), hdVec2(8.1f, 0.0f), 
												  e_totemMaterialEarth, 
												  e_totemShapeTypeRectangle,
													e_totemBlockTypeBoundary);	
	level->AddBlock(rightBarrierBlock);
	
	/*
	 * Find the index of the current level - we want to insert AFTER this.
	 */
	int index = 0;
	for (index = 0; index < m_totemWorld->GetLevelCount(); ++index)
	{
		if (m_totemWorld->GetLevels()[index] == m_currentLevel) break;
	}
	index = hdClamp(index, 0, m_totemWorld->GetLevelCount()-1);
	
	level->SetLevelName("(UNNAMED)");
	
	m_totemWorld->AddLevelAtIndex(level, index+1);
	this->SetCurrentLevel(level);
}



void hdLevelEditorController::ToggleLayerMode(const e_interfaceLayerMode layerMode)
{
	settings.interfaceLayerMode ^= (int)layerMode;
}


void hdLevelEditorController::Load()
{
	// Load from currentWorldPath
	//assert(strlen(settings.currentWorldPath) > 0);
	
	if (m_currentLevel != NULL)
	{
		hdPrintf("[I] Calling m_currentLevel->DestroyGameObjects()\n");
		m_currentLevel->DestroyGameObjects();
	}
	
	// if m_totemWorld is not null, then save and dispose
	if (m_totemWorld != NULL)
	{
		hdPrintf("[I] Disposing of current totem world\n");
		
		//totemWorldManager::Instance()->DestroyTotemWorld(m_totemWorld);
		
		delete m_physicsWorld;
		
		delete m_gameWorld;
		hdAABB aabb;
		aabb.lower.Set(0.0f, 0.0f, 0.0f);
		aabb.upper.Set(100.0f, 100.0f, 100.0f);
		m_gameWorld = new hdGameWorld(aabb);

		m_physicsWorld = NULL;
		m_totemWorld = NULL;
		m_currentLevel = NULL;
	}
	
	this->InitPhysics();
	
	m_totemWorld = totemWorldManager::Instance()->FindTotemWorld(settings.currentWorldPath);
	
	if (m_totemWorld == NULL)
	{
		hdPrintf("Could not find a compressed world file with name %s - loading uncompressed\n", settings.currentWorldPath);
		
		char filePath[256];
		snprintf(filePath, 256, "%s%s", FileSystem_BaseDir(), settings.currentWorldPath);
		
		// Load Levels
		std::ifstream ifs(filePath);
		if (ifs.is_open()) {
			m_totemWorld = new totemWorld();
			
			//TODO: check for std assertion failed exception.
			boost::archive::text_iarchive ia(ifs);
			
			// read class state from archive
			ia >> (* m_totemWorld);
			
			assert(m_totemWorld->GetLevelCount() > 0);
			
			this->SetCurrentLevel((totemLevel *)m_totemWorld->GetLevels()[0]);
		}
		else
		{
			settings.currentWorldPath = "(Unnamed)";
			m_totemWorld = new totemWorld("New World");
			totemLevel* newlev = new totemLevel();
			this->SetCurrentLevel(newlev);
			m_totemWorld->AddLevel(newlev);
		}
		
	}
	else
	{
		hdPrintf("Compressed world loaded successfully.\n");
		assert(m_totemWorld->GetLevelCount() > 0);
		this->SetCurrentLevel((totemLevel *)m_totemWorld->GetLevels()[0]);
	}
}


void hdLevelEditorController::SaveNewWorld(const char *name)
{
	char filePath[256];
	snprintf(filePath, 256, "%s%s", FileSystem_BaseDir(), name);
	
	std::ifstream ifs(filePath);
	
	if (ifs.is_open()) 
	{
		hdPrintf("This file name was already in use.");
		return;
	}
	
	totemWorld *world = new totemWorld(name);
	totemLevel *level = new totemLevel();
	
	world->AddLevel(level);
	
	std::ofstream ofs(filePath);
	{
		boost::archive::text_oarchive oa(ofs);
		oa << (* world);
	}
}



void hdLevelEditorController::Save()
{
	if (m_totemWorld == NULL) return;
	if (strlen(settings.currentWorldPath) == 0) return;
	
	m_currentLevel->GetAABB();
	
	char filePath[256];
	snprintf(filePath, 256, "%s%s", FileSystem_BaseDir(), settings.currentWorldPath);
	/*
	// Backup before using boost - too many corrupted files...
	// This block is from ostream docs on www.cplusplus.com
	{
		char * buffer;
		long size;
		
		char backupPath[256];
		snprintf(backupPath, 256, "/Users/david/Documents/Projects/TotemGames/tmp%s", settings.currentWorldPath);
	
		std::ifstream ifs(filePath);
		std::ofstream backupStream(backupPath);
	
		ifs.seekg(0, std::ifstream::end);
		size = ifs.tellg();
		ifs.seekg(0);
	
		buffer = new char [size];
		ifs.read(buffer, size);
		
		backupStream.write(buffer, size);
		
		delete [] buffer;
		
		ifs.close();
		backupStream.close();
	}
	
	std::ofstream ofs(filePath);
	{
		boost::archive::text_oarchive oa(ofs);
		oa << (* m_totemWorld);
	}
	ofs.close();
	 */
	
	// HACK HACK HACK
	// World name is just the path
	
	if (settings.currentWorldPath[0] == '/')
	{
		m_totemWorld->SetName(settings.currentWorldPath+1);
	}
	else
	{
		m_totemWorld->SetName(settings.currentWorldPath);
	}
	
	
	
	
	
	
	totemWorldManager::Instance()->SaveTotemWorld(m_totemWorld, filePath, true);
}


void hdLevelEditorController::FinishCurrentShape()
{
	//m_currentLevel->AddShape(m_currentShape);
	//m_currentShape = NULL;
}


void hdLevelEditorController::FinishCurrentPolygon()
{
	m_currentLayerPolygon = NULL;
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedGameObjects->RemoveAll();
}


const bool hdLevelEditorController::SetCurrentLevel(const totemLevel* level)
{
	if (level == NULL) return false;
	if (level == m_currentLevel) return true;
	
	if (m_currentLevel != NULL)
	{
		//this->Save();
		hdPrintf("[I] Calling m_currentLevel->DestroyGameObjects()\n");
		m_currentLevel->DestroyGameObjects();
	}
	
	this->InitPhysics();
	
	hdPrintf("[I] Loading new level\n");
	// Clear the game world and physics world objects.
	m_currentLevel = (totemLevel *)level;
	m_currentLevel->InitGameObjects(m_gameWorld, m_projection);
	m_currentLevel->InitPhysicsObjects(m_gameWorld, m_physicsWorld);
	
	m_currentLevel->GetAABB();
	
	//settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedGameObjects->RemoveAll();
	
	return true;
}


void hdLevelEditorController::SelectMultipleGameObjects()
{
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedGameObjects->RemoveAll();
	
	const int k_maxCount = 256;
	hdGameObject* objs[k_maxCount];
	
	hdAABB mouseAABB;
	hdVec3 bottom, top;
	
	bottom.Set(m_startClickPoint.x, m_startClickPoint.y, 50.0f);
	top.Set(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y, -50.0f);
	
	mouseAABB.lower = hdMin(bottom, top);
	mouseAABB.upper = hdMax(bottom, top);
	
	int clickObjectCount = m_gameWorld->AABBQuery(mouseAABB, objs, k_maxCount);
	
	
	hdVec3 mousePoint = hdVec2toVec3(m_currentMouseDragPoint);
	
	if (clickObjectCount == 0) return;
	
	if (clickObjectCount >= 1)
	{
		for (int i = 0; i < hdMin(clickObjectCount,k_maxCount); i++)
		{
			if (CanSelectGameObject(objs[i]))
			{
				m_selectedGameObjects->Add(objs[i]);
			}
		}
	}
	
	if (m_selectedGameObjects->GetItemCount() > 0)
	{
		settings.DEPRECATEDselectedGameObject = m_selectedGameObjects->GetItems()[0];
	}
}



void hdLevelEditorController::SelectSingleGameObject()
{
	settings.DEPRECATEDselectedGameObject = NULL;
	m_selectedGameObjects->RemoveAll();
	
	const int k_maxCount = 10;
	hdGameObject* objs[k_maxCount];
	hdVec3 testPoint;
	//int clickObjectCount = m_gameWorld->PointQuery(hdVec2toVec3(m_currentMouseDragPoint), objs, k_maxCount);

	hdVec3 ray[2];
	memset(ray, 0, sizeof(ray));
	ray[0].Set(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y, 10.0f);
	ray[1].Set(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y, -100.0f);

	// HAKC HACK HACK
	// 
	// Search for joints and events first
	int clickJointCount = m_gameWorld->RayQuery((unsigned int)e_totemTypeJoint, ray, objs, k_maxCount, testPoint);
	if (clickJointCount >= 1)
	{
		m_selectedGameObjects->Add(objs[0]);
		settings.DEPRECATEDselectedGameObject = objs[0];
		return;
	}
	
	int clickEventCount = m_gameWorld->RayQuery((unsigned int)e_totemTypeEvent, ray, objs, k_maxCount, testPoint);
	if (clickEventCount >= 1)
	{
		m_selectedGameObjects->Add(objs[0]);
		settings.DEPRECATEDselectedGameObject = objs[0];
		return;
	}
	
	unsigned int global = 0xFFFF;
	int clickObjectCount = m_gameWorld->RayQuery(global, ray, objs, k_maxCount, testPoint);
	
	//for (int i = 0; i < hdMin(m_rayQueryCount,k_maxCount); i++)
	
	
	
	//int clickObjectCount = m_gameWorld->AABBQuery(mouseAABB, objs, k_maxCount);
		
	hdVec3 mousePoint = hdVec2toVec3(m_currentMouseDragPoint);
	
	if (clickObjectCount == 0) return;
	
	if (clickObjectCount >= 1)
	{
		for (int i = 0; i < hdMin(clickObjectCount,k_maxCount); i++)
		{
			if (objs[i] == m_projection) 
			{
				settings.DEPRECATEDselectedGameObject = NULL;
			}
			else if (settings.interfaceLayerMode & e_interfaceLayerModeGameLayer)
			{
				if (hdConcavePolygonContainsPoint(objs[i]->GetVertices(), objs[i]->GetVertexCount(), objs[i]->GetWorldCenter(),	mousePoint))
				{
					if (CanSelectGameObject(objs[i]))
					{
						m_selectedGameObjects->Add(objs[i]);
						break;
					}
				}
			}
		}
	} 
	
	//m_selectedGameObjects->Add(objs[i]);
	
	if (m_selectedGameObjects->GetItemCount() > 0)
	{
		settings.DEPRECATEDselectedGameObject = m_selectedGameObjects->GetItems()[0];
	}
}


const bool hdLevelEditorController::CanSelectGameObject(const hdGameObject *object) const
{
	if (object == m_projection) return false;
	if (object->GetUserType() == (int)e_totemTypeLayer) return false;
	if (object->GetUserType() == 0) return false;
	
	if (object->GetUserType() == (int)e_totemTypeBlock)
	{
		if (BlockPassesFilter((totemBlock *)object))
		{
			return true;
		}
	}
	else if (object->GetUserType() == (int)e_totemTypeJoint)
	{
		if (settings.showPhysics)
		{
			return true;
		}
	}
	else if (object->GetUserType() == (int)e_totemTypeEvent)
	{
		if (settings.showCosmetics)
		{
			return true;
		}
	}
	
	return false;
}



/*
 * Scale a group through a common central axis
 */
void hdLevelEditorController::ScaleSelectedObjects()
{
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	hdAABB groupAABB;
	groupAABB.lower.Set(100,100,0);
	groupAABB.upper.Set(-1000,-1000,0);
	
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i] != NULL)
		{
			groupAABB.lower = hdMin(groupAABB.lower, m_selectedGameObjects->GetItems()[i]->GetAABB().lower);
			groupAABB.upper = hdMax(groupAABB.upper, m_selectedGameObjects->GetItems()[i]->GetAABB().upper);
		}
	}
	
	hdVec3 center = groupAABB.lower + (0.5f * (groupAABB.upper - groupAABB.lower));
	
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i] != NULL)
		{
			hdVec3 boxPrev = hdVec2toVec3(m_previousMouseDragPoint) - center;
			hdVec3 boxCurr = hdVec2toVec3(m_currentMouseDragPoint) - center;
			
			boxPrev.Set(hdAbs(boxPrev.x), hdAbs(boxPrev.y), 1.0f);
			boxCurr.Set(hdAbs(boxCurr.x), hdAbs(boxCurr.y), 1.0f);			
			
			// Ensure square scaling so proportions are kept.
			hdVec3 scale(boxCurr.x / boxPrev.x, boxCurr.x / boxPrev.x, 1.0f);
			
			m_selectedGameObjects->GetItems()[i]->Scale(scale, center);
			
			/*
			hdTranslateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
								m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
								-center);
			
			hdScaleVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
							m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
							scale);
			
			hdTranslateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
								m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
								center);
			
			
			

			
			m_selectedGameObjects->GetItems()[i]->ResetAABB();
			
			((hdPolygon *)m_selectedGameObjects->GetItems()[i])->SetStartingVerticesToCurrent();*/
		}
	}
}



void hdLevelEditorController::RotateSelectedObjects()
{
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	// Rotate everything around the center of all the objects - find the aabb of all!
	hdAABB groupAABB;
	groupAABB.lower.Set(100,100,0);
	groupAABB.upper.Set(-1000,-1000,0);
	
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i] != NULL)
		{
			groupAABB.lower = hdMin(groupAABB.lower, m_selectedGameObjects->GetItems()[i]->GetAABB().lower);
			groupAABB.upper = hdMax(groupAABB.upper, m_selectedGameObjects->GetItems()[i]->GetAABB().upper);
		}
	}
	
	hdVec3 center = groupAABB.lower + (0.5f * (groupAABB.upper - groupAABB.lower));
	
	
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i] != NULL)
		{
			// Get angle between current and previous...
			//hdVec3 center = (m_selectedGameObjects->GetItems()[i]->GetWorldCenter());
			float delta = hdGetAngle(m_previousMouseDragPoint, m_currentMouseDragPoint, hdVec3toVec2(center));
			hdVec3 rot(0,0,delta);
			
			m_selectedGameObjects->GetItems()[i]->Rotate(rot, center);
			
			/*
			hdTranslateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
								m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
								-center);
			
			hdRotateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
							 m_selectedGameObjects->GetItems()[i]->GetVertexCount(), rot);
			
			hdTranslateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
								m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
								center);
			
			//settings.DEPRECATEDselectedGameObject->ResetOBB();
			//m_selectedGameObjects->GetItems()[i]->ResetOBB();
			m_selectedGameObjects->GetItems()[i]->ResetAABB();
			//((hdPolygon *)settings.DEPRECATEDselectedGameObject)->SetStartingVerticesToCurrent();
			
			//settings.DEPRECATEDselectedGameObject->GetOBB().transform.rotation += rot;
			((hdPolygon *)m_selectedGameObjects->GetItems()[i])->SetStartingVerticesToCurrent();
			*/
		}
	}
}



void hdLevelEditorController::MoveSelectedObjects()
{
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i] != NULL)
		{
			hdVec3 current = m_selectedGameObjects->GetItems()[i]->GetWorldCenter();
			hdVec3 diff = hdVec2toVec3(m_currentMouseDragPoint - m_previousMouseDragPoint);
			
			hdTranslateVertices(m_selectedGameObjects->GetItems()[i]->GetVertices(), m_selectedGameObjects->GetItems()[i]->GetVertexCount(), diff);
			m_selectedGameObjects->GetItems()[i]->GetTransform().translation = diff + current;
			m_selectedGameObjects->GetItems()[i]->ResetAABB();
			
			if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
			{
				((totemBlock *)m_selectedGameObjects->GetItems()[i])->ResetPhysicsBody();
				//SetStartingVerticesToCurrent
			}
			else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeJack)
			{
				((totemJack *)m_selectedGameObjects->GetItems()[i])->ResetPhysicsBody();
			}
			else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeJoint)
			{
				if (((totemJoint *)m_selectedGameObjects->GetItems()[i])->GetJointType() == e_totemJointTypeDistance)
				{
					hdVec3 jointPoint = ((totemJoint *)m_selectedGameObjects->GetItems()[i])->GetSecondJointPoint();
					jointPoint += diff;
					((totemJoint *)m_selectedGameObjects->GetItems()[i])->SetSecondJointPoint(jointPoint);
				}
				((totemJoint *)m_selectedGameObjects->GetItems()[i])->DestroyJoint();
			}
			
			// Set the starting vertices to whatever it is now.
			((hdPolygon *)m_selectedGameObjects->GetItems()[i])->SetStartingVerticesToCurrent();
		}
	}
}



void hdLevelEditorController::AddNewBlock()
{
	totemBlock* block = NULL;
	
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
				m_selectedGameObjects->Add(block);
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
				m_selectedGameObjects->Add(block);
			}		
		}
	}
}


void hdLevelEditorController::AddNewJack()
{
	totemJack* jack = new totemJack(m_gameWorld, m_physicsWorld, m_startClickPoint);
	m_currentLevel->AddJack(jack);
}


void hdLevelEditorController::AddNewJoint()
{	
	this->SelectSingleGameObject();
	if (settings.DEPRECATEDselectedGameObject == NULL) return;
	if (settings.DEPRECATEDselectedGameObject->GetUserType() != (int)e_totemTypeBlock) return;
	
	if (settings.newTotemJointType == e_totemJointTypeRevolute 
		|| settings.newTotemJointType == e_totemJointTypeDistance 
		|| settings.newTotemJointType == e_totemJointTypePrismatic)
	{
		if (m_currentJoint == NULL)
		{
			if (settings.DEPRECATEDselectedGameObject != NULL)
			{
				m_currentJoint = new totemJoint(m_gameWorld, m_physicsWorld, settings.newTotemJointType, hdVec2toVec3(m_currentMouseDragPoint), (totemBlock *)settings.DEPRECATEDselectedGameObject, NULL);
			}
		}
		else
		{
			if (0 == m_currentJoint->SetSecondBody((const totemBlock *)settings.DEPRECATEDselectedGameObject))
			{
				if (settings.newTotemJointType == e_totemJointTypeDistance 
					|| settings.newTotemJointType == e_totemJointTypePrismatic)
				{
					m_currentJoint->SetSecondJointPoint(hdVec2toVec3(m_currentMouseDragPoint));
				}
				
				m_currentLevel->AddJoint(m_currentJoint);
				m_currentJoint = NULL;
			}
		}
	}
}
	

void hdLevelEditorController::AddNewEvent()
{
	
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
	}
}



void hdLevelEditorController::AddNewLayerPolygonQuad()
{
	hdVec2 aa, bb;
	
	totemPolygon* polygon = new totemPolygon(m_gameWorld, settings.newTexturePath);
	if (settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygonQuadActualSize)
	{
		hdTexture* tex = hdTextureManager::Instance()->FindTexture(settings.newTexturePath, TT_Wall);
		aa = m_currentMouseDragPoint;
		bb.Set(aa.x + (tex->width/100.0f), aa.y + (tex->height/100.0f));
	} 
	else 
	{
		aa = hdMin(m_currentMouseDragPoint, m_startClickPoint);
		bb = hdMax(m_currentMouseDragPoint, m_startClickPoint);
		
		if (settings.GLUTmodifiers == GLUT_ACTIVE_SHIFT)
		{ 
			float sideMax = hdMax((bb - aa).x, (bb - aa).y);
			bb.Set(aa.x + sideMax, aa.y + sideMax);
		}
	}
	polygon->SetAs2DBox(aa, bb);
	polygon->ResetAABB();
	
	if (settings.interfaceLayerMode & e_interfaceLayerModeForeground)
	{
		m_currentLevel->GetForegroundLayer()->AddPolygon(polygon);
	}
	else if (settings.interfaceLayerMode & e_interfaceLayerModeMidground)
	{
		m_currentLevel->GetMidgroundLayer()->AddPolygon(polygon);
	}
	else if (settings.interfaceLayerMode & e_interfaceLayerModeBackground)
	{
		m_currentLevel->GetBackgroundLayer()->AddPolygon(polygon);
	} 
	m_selectedGameObjects->Add(polygon);
	settings.DEPRECATEDselectedGameObject = polygon;
}



void hdLevelEditorController::AddNewLayerPolygon()
{
	if (m_currentLayerPolygon == NULL)
	{
		totemPolygon *polygon = new totemPolygon(m_gameWorld, settings.newTexturePath, settings.newPolygonTint);
		polygon->AddPoint(m_startClickPoint.x, m_startClickPoint.y);
		
		int result;

		if (settings.interfaceLayerMode & e_interfaceLayerModeForeground)
		{
			result = m_currentLevel->GetForegroundLayer()->AddPolygon(polygon);
		}
		else if (settings.interfaceLayerMode & e_interfaceLayerModeMidground)
		{
			result = m_currentLevel->GetMidgroundLayer()->AddPolygon(polygon);
		}
		else if (settings.interfaceLayerMode & e_interfaceLayerModeBackground)
		{
			result = m_currentLevel->GetBackgroundLayer()->AddPolygon(polygon);
		}
		
		if (result != -1)
		{
			settings.DEPRECATEDselectedGameObject = polygon;
			m_selectedGameObjects->Add(polygon);
			m_currentLayerPolygon = polygon;
		}
	} 
	else 
	{
		m_currentLayerPolygon->AddPoint(m_startClickPoint.x, m_startClickPoint.y);
		m_currentLayerPolygon->ResetAABB();
		m_currentLayerPolygon->ResetTextureCoords();
	}
}



void hdLevelEditorController::MouseDown(const hdVec2& p)
{
	settings.GLUTmodifiers = glutGetModifiers();
	m_startClickPoint.Set(p.x, p.y);
	m_currentMouseDragPoint = m_startClickPoint;
	m_previousMouseDragPoint = m_startClickPoint;
	
	// click selects a object.
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		if (m_selectedGameObjects->GetItemCount() > 0)
		{
			// Check if m_startClickPoint was inside any of the selected items. If
			// not, then deselected all.
			bool mouseInSelection = false;
			for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
			{
				if (hdConcavePolygonContainsPoint(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
												  m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
												  m_selectedGameObjects->GetItems()[i]->GetWorldCenter(),	
												  hdVec2toVec3(m_startClickPoint)))
				{
					mouseInSelection = true;
					break;
				}
			}
			
			if (mouseInSelection == false) 
			{
				this->SelectSingleGameObject();
				///m_selectedGameObjects->RemoveAll();
				//settings.DEPRECATEDselectedGameObject = NULL;
			}
			//this->SelectSingleGameObject();
		}
		else
		{
			this->SelectSingleGameObject();
		}
		
		//if (m_selectedGameObjects->GetItemCount()) this->SelectSingleGameObject();	
	}
}



void hdLevelEditorController::DeleteKey()
{
	settings.GLUTmodifiers = glutGetModifiers();

	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	bool res = false;
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			res = m_currentLevel->RemoveBlock((totemBlock *)m_selectedGameObjects->GetItems()[i]);
		} 
		else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeJack)
		{
			res = m_currentLevel->RemoveJack((totemJack *)m_selectedGameObjects->GetItems()[i]);
		}
		else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeJoint)
		{
			res = m_currentLevel->RemoveJoint((totemJoint *)m_selectedGameObjects->GetItems()[i]);
		}
		else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeEvent)
		{
			res = m_currentLevel->GetEvents()->Remove((totemEvent *)m_selectedGameObjects->GetItems()[i]);
		}
		else if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypePolygon)
		{
			if (settings.interfaceLayerMode & e_interfaceLayerModeForeground)
			{
				m_currentLevel->GetForegroundLayer()->RemovePolygon((totemPolygon *)m_selectedGameObjects->GetItems()[i]);
			}
			else if (settings.interfaceLayerMode & e_interfaceLayerModeMidground)
			{
				m_currentLevel->GetMidgroundLayer()->RemovePolygon((totemPolygon *)m_selectedGameObjects->GetItems()[i]);
			}
			else if (settings.interfaceLayerMode & e_interfaceLayerModeBackground)
			{
				m_currentLevel->GetBackgroundLayer()->RemovePolygon((totemPolygon *)m_selectedGameObjects->GetItems()[i]);
			}
		}		
	}
	
	
	// if (settings.DEPRECATEDselectedGameObject == NULL) return;
	
	//m_currentLevel->DestroyGameObjects();
	this->InitPhysics();
	m_currentLevel->InitGameObjects(m_gameWorld, m_projection);
	m_currentLevel->InitPhysicsObjects(m_gameWorld, m_physicsWorld);
	
	m_selectedGameObjects->RemoveAll();
	settings.DEPRECATEDselectedGameObject = NULL;
}



void hdLevelEditorController::MouseMove(const hdVec2& p)
{
	m_previousMouseDragPoint = m_currentMouseDragPoint;
	m_currentMouseDragPoint.Set(p.x, p.y);
	
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		// translate the totemBlock to the new point:
		if (settings.GLUTmodifiers == GLUT_ACTIVE_CTRL)
		{
			this->RotateSelectedObjects();
		}
		else if (settings.GLUTmodifiers == 3) //& (GLUT_ACTIVE_CTRL | GLUT_ACTIVE_SHIFT))
		{
			this->ScaleSelectedObjects();
		}
		else if (settings.GLUTmodifiers == 0)
		{
			this->MoveSelectedObjects();
		}
	} 
	else if (settings.interfacePaletteMode == e_interfacePaletteModeAddTotemBlock)
	{
		
	}
}



// TODO: Refactor - switch and put into separate functions.
void hdLevelEditorController::MouseUp()
{
	settings.GLUTmodifiers = glutGetModifiers();
	
	if (settings.interfacePaletteMode == e_interfacePaletteModeCursor)
	{
		if (settings.GLUTmodifiers & (GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL) > 0)
		{
			this->SelectMultipleGameObjects();
		}
		else if (settings.GLUTmodifiers == 0)
		{
			// Need to reject the mouse up selection event when we have a selected
			// group. Reject if the mouse up event occurred while over any member
			// of the selected set.
			if (m_selectedGameObjects->GetItemCount() > 0)
			{
				// Check if m_startClickPoint was inside any of the selected items. If
				// not, then deselected all.
				bool mouseInSelection = false;
				for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
				{
					if (hdConcavePolygonContainsPoint(m_selectedGameObjects->GetItems()[i]->GetVertices(), 
													  m_selectedGameObjects->GetItems()[i]->GetVertexCount(), 
													  m_selectedGameObjects->GetItems()[i]->GetWorldCenter(),	
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
	else if (settings.interfacePaletteMode == e_interfacePaletteModeAddTotemBlock)
	{
		this->AddNewBlock();
	} 
	else if (settings.interfacePaletteMode == e_interfacePaletteModeTotemJack)
	{
		this->AddNewJack();
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeTotemJoint)
	{
		this->AddNewJoint();
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeTotemEvent)
	{
		this->AddNewEvent();
	}
	/*
	 else if (settings.interfacePaletteMode == e_interfacePaletteModeTotemShape)
	 {
	 if (m_currentShape == NULL)
	 {
	 m_currentShape = new totemShape(m_gameWorld, m_physicsWorld, m_startClickPoint, 0);
	 }
	 
	 m_currentShape->AddPoint(m_startClickPoint.x, m_startClickPoint.y);
	 } */
	else if (settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygonQuad || settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygonQuadActualSize)
	{
		this->AddNewLayerPolygonQuad();
	}
	else if (settings.interfacePaletteMode == e_interfacePaletteModeLayerPolygon)
	{
		this->AddNewLayerPolygon();
	}
	m_currentMouseDragPoint = m_startClickPoint;
	
	// Hold down shift to complete a shape.
	if (settings.GLUTmodifiers == GLUT_ACTIVE_SHIFT)
	{
		if (m_currentBlockShape == NULL) return;
		
		// Ensure vertices are ordered correctly.
		ReverseCCWVertices(m_currentBlockShape->GetVertices(), m_currentBlockShape->GetVertexCount());
		m_currentBlockShape->SetStartingVerticesToCurrent();
		m_currentBlockShape->ResetTextureCoords();
		
		// stop shape editing
		m_currentBlockShape = NULL;
		m_currentLayerPolygon = NULL;
		settings.DEPRECATEDselectedGameObject = NULL;
		m_selectedGameObjects->RemoveAll();
	}
}


void hdLevelEditorController::RepairSelectedShape()
{
	totemBlock *block;
	
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	if (m_selectedGameObjects->GetItems()[0]->GetUserType() != e_totemTypeBlock) return;
	
	block = (totemBlock *)m_selectedGameObjects->GetItems()[0];
	
	// Ensure vertices are ordered correctly.
	//ReverseCCWVertices(block->GetVertices(), block->GetVertexCount());
	//	
	hdVec3 *verticesWithOppDepth = new hdVec3[block->GetVertexCount()];
	
	for (int i = 0; i < block->GetVertexCount();++i)
	{
		verticesWithOppDepth[block->GetVertexCount() - 1 - i].x = ((totemBlock *)block)->GetVertices()[i].x;
		verticesWithOppDepth[block->GetVertexCount() - 1 - i].y = ((totemBlock *)block)->GetVertices()[i].y;
		verticesWithOppDepth[block->GetVertexCount() - 1 - i].z = ((totemBlock *)block)->GetVertices()[i].z;
	}
	
	hdVec3 tmp;
	int count = block->GetVertexCount();
	block->RemoveAllPoints();
	for (int i = 0; i < count;++i)
	{
		tmp.Set(verticesWithOppDepth[i].x, verticesWithOppDepth[i].y, verticesWithOppDepth[i].z);
		block->AddPoint(tmp);
	}
	
	block->SetStartingVerticesToCurrent();
	block->ResetTextureCoords();

	
}


void hdLevelEditorController::ReverseCCWVertices(hdVec3 *vertices, const int vertexCount)
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


void hdLevelEditorController::CopySelectedObjects()
{
	m_copiedGameObjects->RemoveAll();
	
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	for (int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		m_copiedGameObjects->Add(m_selectedGameObjects->GetItems()[i]);
	}
	
	//settings.copiedGameObject = settings.DEPRECATEDselectedGameObject;
}


void hdLevelEditorController::PasteCopiedObjects()
{
	if (m_copiedGameObjects->GetItemCount() == 0) return;
	
	m_selectedGameObjects->RemoveAll();
	settings.copiedBlockCount = 0;
	
	// Copy blocks, then joints, so joints can find their associated
	// and freshly copied blocks.
	for (int i = 0; i < m_copiedGameObjects->GetItemCount(); i++)
	{
		if (m_copiedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			m_selectedGameObjects->Add(
									   this->CopyBlockToLevel((totemBlock *)m_copiedGameObjects->GetItems()[i])
									   );
			++settings.copiedBlockCount;
		}
	}
	

	for (int i = 0; i < m_copiedGameObjects->GetItemCount(); i++)
	{
		if (m_copiedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeJoint)
		{
			m_selectedGameObjects->Add(
									   this->CopyJointToLevel((totemJoint *)m_copiedGameObjects->GetItems()[i])
									   );
		}
	}
}


void hdLevelEditorController::CopyCurrentLevel()
{
	settings.copiedBlocks = (m_currentLevel)->GetBlocks();
	settings.copiedBlockCount = (m_currentLevel)->GetBlockCount();
	
	settings.copiedJoints = m_currentLevel->GetJoints();
	settings.copiedJointCount = m_currentLevel->GetJointCount();
}


void hdLevelEditorController::PasteCopiedLevel()
{
	if (settings.copiedBlocks == NULL) return;
	
	for (int i = 0; i < this->settings.copiedBlockCount; i++)
	{
		this->CopyBlockToLevel(settings.copiedBlocks[i]);
	}
	
	for (int i = 0; i < this->settings.copiedJointCount; i++)
	{
		this->CopyJointToLevel(settings.copiedJoints[i]);
	}
	
	settings.copiedBlocks = NULL;
}


void hdLevelEditorController::ApplyCurrentTextureToSelected(const char* texture)
{
	if (m_selectedGameObjects->GetItemCount() == 0) return;
	
	totemBlock* selected = NULL;
	for(int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
	{
		if (m_selectedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			selected = (totemBlock *)m_selectedGameObjects->GetItems()[i];
			if (selected != NULL && selected->IsTextureChangeable())
			{
				selected->SetTextureName(texture);
				selected->ResetTextureCoords();
			}
		}
	}
}


void hdLevelEditorController::DumpUniqueTextureList()
{
	int i, j;
	
	totemLevel *lev;
	totemFloorInfo *floor;
	totemBlock *block;
	
	if (m_totemWorld == NULL) return;
	if (m_totemWorld->GetLevelCount() == 0) return;
	
	// For each level, add the unique texture values to a map.
	// Then just print out the map to stdout.
	
	
	
	for (i = 0; i< m_totemWorld->GetLevelCount(); ++i)
	{
		lev = (totemLevel *)m_totemWorld->GetLevels()[i];
		
		uniqueTextures[string(lev->GetSkyTextureName())] = string(lev->GetSkyTextureName());
		uniqueTextures[string(lev->GetDistantBackgroundTextureName())] = string(lev->GetDistantBackgroundTextureName());
		uniqueTextures[string(lev->GetFarBackgroundTextureName())] = string(lev->GetFarBackgroundTextureName());
		uniqueTextures[string(lev->GetNearBackgroundTextureName())] = string(lev->GetNearBackgroundTextureName());
		
		if (lev->GetFloorInfo() != NULL)
		{
			floor = (totemFloorInfo *)lev->GetFloorInfo();
			uniqueTextures[string(floor->m_firstTextureName)] = string(floor->m_firstTextureName);
			uniqueTextures[string(floor->m_secondTextureName)] = string(floor->m_secondTextureName);
		}
		
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
}


totemBlock* hdLevelEditorController::CopyBlockToLevel(totemBlock *srcBlock)
{
	//res = m_currentLevel->RemoveBlock((totemBlock *)settings.DEPRECATEDselectedGameObject);
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
}


totemJoint* hdLevelEditorController::CopyJointToLevel(totemJoint *srcJoint)
{
	// The blocks have already been copied, so we just need to find 
	// the index of the blocks in the new level.
	int idxOffset = m_currentLevel->GetBlockCount() - settings.copiedBlockCount;
	
	int firstBlockIndex = 0;
	int secondBlockIndex = 0;
	totemBlock *firstJointBody = NULL;
	totemBlock *secondJointBody = NULL;
	
	for (int i = 0; i < m_copiedGameObjects->GetItemCount(); i++)
	{
		if (m_copiedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			
			if (m_copiedGameObjects->GetItems()[i] == srcJoint->GetFirstBody()) 
			{
				break;
			}
			++firstBlockIndex;
		}
	}
	
	for (int i = 0; i < m_copiedGameObjects->GetItemCount(); i++)
	{
		if (m_copiedGameObjects->GetItems()[i]->GetUserType() == (int)e_totemTypeBlock)
		{
			
			if (m_copiedGameObjects->GetItems()[i] == srcJoint->GetSecondBody()) 
			{
				break;
			}
			++secondBlockIndex;
		}
	}
	
	hdAssert(idxOffset + firstBlockIndex < m_currentLevel->GetBlockCount());
	firstJointBody = m_currentLevel->GetBlocks()[idxOffset + firstBlockIndex];
	
	// The second body simply may not have been copied, in which case
	// we want to link the joint to the old body. 
	if (secondBlockIndex == settings.copiedBlockCount)
	{
		secondJointBody = (totemBlock *)srcJoint->GetSecondBody();
	}
	else
	{
		secondJointBody = m_currentLevel->GetBlocks()[idxOffset + secondBlockIndex];
	}
	
	hdAssert(firstJointBody != NULL && secondJointBody != NULL);
	
	totemJoint *newJoint = new totemJoint(m_gameWorld, 
										  m_physicsWorld,
										  srcJoint->GetJointType(),
										  srcJoint->GetWorldCenter() + hdVec3(0.0f, 2.0f, 0.0f),
										  firstJointBody,
										  secondJointBody);
	
	if (srcJoint->GetJointType() == e_totemJointTypeDistance)
	{
		newJoint->SetSecondJointPoint(srcJoint->GetSecondJointPoint() + hdVec3(0.0f, 2.0f, 0.0f));
	}

	
	if (srcJoint->GetJointType() == e_totemJointTypeRevolute)
	{
		newJoint->SetRevoluteJointProperties(srcJoint->GetJointMotorEnabled(), 
											 srcJoint->GetJointLimitEnabled(), 
											 srcJoint->GetRevoluteJointLowerAngle(), 
											 srcJoint->GetRevoluteJointUpperAngle(), 
											 srcJoint->GetRevoluteJointMaxTorque(), 
											 srcJoint->GetJointMotorSpeed());
	}
	else if (srcJoint->GetJointType() == e_totemJointTypePrismatic)
	{
		newJoint->SetPrismaticJointProperties(srcJoint->GetJointMotorEnabled(), 
											  srcJoint->GetJointLimitEnabled(), 
											  srcJoint->GetJointMotorSpeed(), 
											  srcJoint->GetPrismaticJointLowerTranslation(), 
											  srcJoint->GetPrismaticJointUpperTranslation(), 
											  srcJoint->GetPrismaticJointMaxMotorForce());
	}
	
	
	if (m_currentLevel->AddJoint(newJoint) == -1)
	{
		hdPrintf("Joint copy failed: level already has a full contingent of joints.\n");
		delete newJoint;
		return NULL;
	}
	return newJoint;
}


const bool hdLevelEditorController::BlockPassesFilter(const totemBlock *block) const
{
	e_totemBlockType blockType = ((totemBlock *)block)->GetBlockType();
	
	if (settings.showGround && 
		(blockType == e_totemBlockTypeGround ||
		 blockType == e_totemBlockTypeBoundary ||
		 blockType == e_totemBlockTypeHazard ||
		 blockType == e_totemBlockTypeEventSender ||
		 blockType == e_totemBlockTypeMessage ||
		 blockType == e_totemBlockTypeNormalInv ||
		 block->IsGoal()))
	{
		return true;
	}
	else if (settings.showCosmetics &&
			 (blockType == e_totemBlockTypeCosmetic ||
			  blockType == e_totemBlockTypeMessage ||
			  blockType == e_totemBlockTypeLight ||
			 blockType == e_totemBlockTypeGenericSensor ||
			 blockType == e_totemBlockTypeBillboard))
	{
		return true;
	}
	else if (settings.showPhysics && 
			 (block->IsDestroyable() || block->IsDraggable()))
	{
		return true;
	}
	return false;
}



void hdLevelEditorController::DrawModeInfo()
{
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
}



void hdLevelEditorController::DrawString(int x, int y, const char *string, ...)
{
	char buffer[128];
	
	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity(); 
	
	glColor3f(0.4f, 0.5f, 1.0f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]);
	}
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
}



void hdLevelEditorController::Draw()
{
	hdglError("ttt");
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	//glTranslatef(0.0f, 0.0f, -4.0f);
	
	if (settings.testPhysics == 1) 
	{
		this->DrawString(0, 40, "*** Physics Test On");
		m_physicsWorld->Step(1.0/60.0, 10, 10);
		m_currentLevel->Step();
	}
	
	this->DrawModeInfo();
	
	hdglError("sss");
	
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0f, 0.0f, 0.0f);
	//glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	
	hdglError("ttt");
	glBindTexture(GL_TEXTURE_2D, 0);
	// Draw a grid
	glBegin(GL_LINES);
	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	for (float x = settings.screenLower.x; x <= settings.screenUpper.x; x += 0.2f)
	{
		glVertex2f(x, settings.screenLower.y);
		glVertex2f(x, settings.screenUpper.y);
	}
	
	for (float y = settings.screenLower.y; y <= settings.screenUpper.y; y += 0.2f)
	{
		glVertex2f(settings.screenLower.x, y);
		glVertex2f(settings.screenUpper.x, y);
	}
	glEnd();
	
	
	
	// DEBUG: Draw all physics object shape vertices

	
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
	hdglError("vvv");
	
	if (settings.interfaceLayerMode & e_interfaceLayerModeBackground)
		m_currentLevel->GetBackgroundLayer()->Draw();

	
	if (settings.interfaceLayerMode & e_interfaceLayerModeGameLayer)
	{
		m_currentLevel->Draw();
		
		//showCosmetics;
		//bool showGround;
		//bool showPhysics
		
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		totemBlock *block;

		for (int i = 0; i < m_currentLevel->GetBlockCount(); ++i)
		{
			block = m_currentLevel->GetBlocks()[i];
			
			if (BlockPassesFilter(block) && !block->IsTransparent())
			{
				block->Draw();
			}			
		}
		
		for (int i = 0; i < m_currentLevel->GetBlockCount(); ++i)
		{
			block = m_currentLevel->GetBlocks()[i];
			
			if (BlockPassesFilter(block) && block->IsTransparent())
			{
				block->Draw();
			}			
		}
		
		
		glEnable(GL_BLEND);
		if (settings.showPhysics)
		{
			for (int i = 0; i < m_currentLevel->GetJointCount(); ++i)
			{
				m_currentLevel->GetJoints()[i]->Draw();
			}
		}
		
		
		
		for (int i = 0; i < m_currentLevel->GetEvents()->GetItemCount(); ++i)
		{
			m_currentLevel->GetEvents()->GetItems()[i]->Draw();
		}
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
		
	
	
	
	

	
	hdglError("aaa");
	
	const int k_maxCount = 10;
	hdGameObject* objs[k_maxCount];
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
	hdglError("eee");
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
hdglError("xxx");
#if 0
	
	b2Shape* shape;
  	b2Body* body = m_physicsWorld->GetBodyList();
	while(body != NULL)
	{
		hdglBegin(GL_POINTS);
		hdglColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		hdglVertex2f(body->GetXForm().position.x, body->GetXForm().position.y);
		hdglEnd();
		
		shape = body->GetShapeList();
		while (shape != NULL)
		{
			if (shape->GetType() == e_polygonShape)
			{
				hdglBegin(GL_LINE_LOOP);
				hdglColor4f(1.0f, 0.0f, 1.0f, 1.0f);
				for(int i = 0; i< ((b2PolygonShape *)shape)->GetVertexCount(); i++)
				{
					hdglVertex2f(((b2PolygonShape *)shape)->GetVertices()[i].x, ((b2PolygonShape *)shape)->GetVertices()[i].y);
				}
				hdglEnd();
			}
			shape = shape->GetNext();
		}
		
		body = body->GetNext();
	}
#endif
	glBindTexture(GL_TEXTURE_2D, 0);
	if (m_selectedGameObjects->GetItemCount() > 0)
	{
		for (int i = 0; i < m_selectedGameObjects->GetItemCount(); i++)
		{
			hdDrawGameObjectHandles(m_selectedGameObjects->GetItems()[i]);
		}
	}
	else if (settings.DEPRECATEDselectedGameObject != NULL)
	{
		hdDrawGameObjectHandles(settings.DEPRECATEDselectedGameObject);
	}
	
	float GameMinScreenWidth = 4.5f;
	float GameMinScreenHeight = 3.0f;
	//#warning "Bad Zoom Levels Used"
	float GameMaxScreenWidth = 15.0f; 
	float GameMaxScreenHeight = 10.0f; 
	
	hdglError("yyy");
	glBindTexture(GL_TEXTURE_2D, 0);
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(- GameMinScreenWidth/2,		-GameMinScreenHeight/2);
	glVertex2f(- GameMinScreenWidth/2,		-GameMinScreenHeight/2 + GameMinScreenHeight);
	glVertex2f(- GameMinScreenWidth/2 + GameMinScreenWidth, -GameMinScreenHeight/2 + GameMinScreenHeight);
	glVertex2f(- GameMinScreenWidth/2 + GameMinScreenWidth, -GameMinScreenHeight/2);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex2f(- GameMaxScreenWidth/2,		-GameMaxScreenHeight/2);
	glVertex2f(- GameMaxScreenWidth/2,		-GameMaxScreenHeight/2 + GameMaxScreenHeight);
	glVertex2f(- GameMaxScreenWidth/2 + GameMaxScreenWidth, -GameMaxScreenHeight/2 + GameMaxScreenHeight);
	glVertex2f(- GameMaxScreenWidth/2 + GameMaxScreenWidth, -GameMaxScreenHeight/2);
	glEnd();	
	
	glPopMatrix();
	
	hdglError("zzz");
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
