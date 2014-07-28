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

#include "hdk.h"

#include <stdio.h>
#include <fstream>
#include <list>
#include <string>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "glui.h"

#include "hdLevelEditorController.h"
#include "totemGame.h"


using namespace std;

namespace
{
	int32 testIndex = 0;
	int32 testSelection = 0;
	hdLevelEditorController* levelEditorController;
	int32 width = 1910;
	int32 height = 1060;
	int32 framePeriod = 16;
	int32 mainWindow;
	float settingsHz = 60.0;
	GLUI *glui;
	bool ctrlDown = false;
	
	GLUI_Listbox* worldList;
	GLUI_EditText* txtWorldSaveAsName;
	GLUI_EditText* txtWorldName;
	GLUI_Listbox* levelList;
	GLUI_EditText* txtLevelName;
	
	
	GLUI_Panel* newPolygonPropertiesPanel;
	GLUI_Listbox* newPolygonTextureList;
	GLUI_Button* btnEditorFinishCurrentPolygon;
	
	GLUI_Panel* newJackPropertiesPanel;
	GLUI_Listbox* newJackTypesList;
	
	/* Events */
	GLUI_Panel* newEventsPropertiesPanel;
	GLUI_Button* btnNewEvent;
	GLUI_Listbox* newEventTypesList;
	
	/* Block Properties */
	GLUI_Panel* newBlockPropertiesPanel;
	GLUI_Listbox* newBlockTypesList;
	GLUI_Listbox* newBlockShapesList;
	GLUI_Listbox* newMaterialList;
	
	GLUI_Panel* newJointPropertiesPanel;
	GLUI_Listbox* newJointTypesList;
	GLUI_Button* btnAddNewJoint;
	
	GLUI_Panel* newShapePropertiesPanel;
	
	GLUI_StaticText* modeInfo;
	
	// Filters
	GLUI_Checkbox* toggleFilterCosmetics;
	GLUI_Checkbox* toggleFilterGround;
	GLUI_Checkbox* toggleFilterPhysics;
	
	GLUI *gluiBlockProperties;
	GLUI_Panel* selectedBlockPropertiesPanel;
	GLUI_Listbox* selectedBlockMaterialList;
	GLUI_Listbox* selectedBlockTypesList;
	GLUI_EditText *selectedBlockTag;
	GLUI_Listbox *selectedBlockDepth;
	GLUI_EditText *selectedBlockTiling;
	GLUI_EditText *selectedBlockZOffset;
	GLUI_Listbox *selectedBlockTexture; 

	GLUI_Panel* selectedPolygonPropertiesPanel;
	GLUI_Listbox* selectedTextureList;
	
	// Level properties
	GLUI_Panel *currentLevelProperties;
	GLUI_Checkbox *currentLevelExtendAABB;
	GLUI_Listbox *currentLevelSkyTexture;
	GLUI_Listbox *currentLevelNearBGTexture;
	GLUI_Listbox *currentLevelFarBGTexture;
	GLUI_Listbox *currentLevelDistantBGTexture;

	// Sky tint
	GLUI_Panel *currentLevelSkyTintPanel;
	GLUI_EditText *currentLevelSkyTintBottomRed;
	GLUI_EditText *currentLevelSkyTintBottomGreen;
	GLUI_EditText *currentLevelSkyTintBottomBlue;
	
	GLUI_EditText *currentLevelSkyTintTopRed;
	GLUI_EditText *currentLevelSkyTintTopGreen;
	GLUI_EditText *currentLevelSkyTintTopBlue;
	
	// Level floor info
	GLUI_Checkbox *currentLevelHasFloor;
	GLUI_Listbox *currentLevelFloorTexture1;
	GLUI_Listbox *currentLevelFloorTexture2;
	GLUI_EditText *currentLevelFloorLevel;
	GLUI_Checkbox *currentLevelFloorIsWater;
	GLUI_Checkbox *currentLevelReflectiveFloor;
	
	GLUI_Button *currentLevelDeleteButton;
	GLUI_Button *currentLevelCopy;
	GLUI_Button *currentLevelPaste;
	
	// Joint Properties - used for both revolute and prismatic joints
	GLUI_Panel *currentJointPanel;
	GLUI_Checkbox *currentJointEnableMotor;
	GLUI_EditText *currentJointMaxForce;
	GLUI_EditText *currentJointMotorSpeed;
	GLUI_Checkbox *currentJointEnableLimit;
	GLUI_EditText *currentJointLowerLimit;
	GLUI_EditText *currentJointUpperLimit;
	//GLUI_EditText *currentRevJoint
	
	list<string> m_worldFilenames;
	list<string> m_textureFilenames;
	
	float32 viewZoom = 5.5f;
	hdVec2 viewCenter(0.0f, 0.2f);
	int tx, ty, tw, th;
	bool rMouseDown;
	hdVec2 lastp;
	bool m_perspectiveProjection = false;
	
	float32 xExtents = 2.4f, yExtents = 2.4f;
}

/*
void Resize(int32 w, int32 h)
{
	width = w;
	height = h;
	
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float32 ratio = float32(tw) / float32(th);
	
	hdVec2 extents(ratio * xExtents, yExtents);
	extents *= viewZoom;
	
	hdVec2 lower = viewCenter - extents;
	hdVec2 upper = viewCenter + extents;
	
	levelEditorController->settings.screenLower = lower;
	levelEditorController->settings.screenUpper = upper;
	
	if (m_perspectiveProjection)
	{
		gluPerspective(90.0f, ratio, 0.01f, 100000.0f);
		
		//MatrixPerspectiveFovRH(mProjection, 90.0f*(hd_pi/180.0f), 1.0f/1.5f, 0.01f, 1000.0f, false);
		//glMultMatrixf(mProjection.f);
		
		glTranslatef(-viewCenter.x, -viewCenter.y, 
					 -((upper.y - lower.y)/2.0f));
		
		//glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y, 
		//			 -((m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y)/2.0f));
	}
	else
	{
		glOrtho(lower.x, upper.x, lower.y, upper.y, -1000.0, 1000.0);
		
		// L/R/B/T
		//gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
		//gluOrtho2D(0.0, upper.x, 0.0, upper.y);
	}
}*/


/*
hdVec2 ConvertScreenToWorld(int32 x, int32 y)
{
	float32 u = x / float32(tw);
	float32 v = (th - y) / float32(th);
	
	float32 ratio = float32(tw) / float32(th);
	hdVec2 extents(ratio * xExtents, yExtents);
	extents *= viewZoom;
	
	hdVec2 lower = viewCenter - extents;
	hdVec2 upper = viewCenter + extents;
	
	hdVec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}



// This is used to control the frame rate (60Hz).
void Timer(int)
{
	glutSetWindow(mainWindow);

	glutPostRedisplay();
	glutTimerFunc(framePeriod, Timer, 0);

}



void SimulationLoop()
{
	levelEditorController->Draw();

	//DrawString(5, 15, entry->name);
	
	glutSwapBuffers();
	
	if (testSelection != testIndex)
	{
		//testIndex = testSelection;
		//delete test;
		//entry = g_testEntries + testIndex;
		//test = entry->createFcn();
		viewZoom = 1.0f;
		viewCenter.Set(0.0f, 0.2f);
		Resize(width, height);
	}
}*/


/*
void TestPhysics(int i)
{
	//settings.pause = !settings.pause;
	
	levelEditorController->settings.testPhysics = !levelEditorController->settings.testPhysics;
	//if (levelEditorController->settings.testPhysics == 0)
	//{
		levelEditorController->GetCurrentLevel()->ResetBlocks();
	//}
}
*/
/*
void UpdateBlockMaterial(int)
{
}
*/
/*

void Commit(int)
{
	// Current shapes: make the current positions of the 
	// blocks into the start positions.
	levelEditorController->GetCurrentLevel()->CommitBlockStates();
}*/


//void QuitClick(int)
//{
	/*
	for (int i = 0; m_worldFilenames[i] != NULL; i++)
	{
		delete [] m_worldFilenames[i];
	}
	free(m_worldFilenames);
	
	
	for (int i = 0; m_textureFilenames[i] != NULL; i++)
	{
		delete [] m_textureFilenames[i];
	}
	free(m_textureFilenames);*/
	
	//(GLUI_Update_CB)exit();
	//delete levelEditorController;
	
	//m_dumpMemoryReport(0,0);
	
	
	
	//exit(0);
//}

/*

void SetInterfaceMode_ActiveCursor(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
}*/


/*
void SetInterfaceMode_AddTotemBlock(int)
{
	// HACK HACK HACK
	//
	// While the level editor is in an unstable state, save
	// each time this button is pressed.
	if (levelEditorController != NULL) levelEditorController->Save();
	
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
}*/


/*
void SetInterfaceMode_AddJoint(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
}



void SetInterfaceMode_AddJack(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemJack;
}


void SetInterfaceMode_AddEvent(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemEvent;
}


void UpdateNewJackType(int)
{
	levelEditorController->settings.newTotemJackType = (e_totemJackType)newJackTypesList->get_int_val();
}


void UpdateNewBlockType(int newType)
{
	levelEditorController->settings.newTotemBlockType = (e_totemBlockType)newBlockTypesList->get_int_val();
}


void UpdateNewBlockShapeType(int)
{
	levelEditorController->settings.newTotemShapeType = (e_totemShapeType)newBlockShapesList->get_int_val();	
}


void UpdateNewEventType(int)
{
	levelEditorController->settings.newTotemEventType = (e_totemEventType)newEventTypesList->get_int_val();	
}


void UpdateNewJointType(int)
{
	levelEditorController->settings.newTotemJointType = (e_totemJointType)newJointTypesList->get_int_val();
}


void UpdateNewBlockMaterial(int newType)
{
	levelEditorController->settings.newTotemMaterial = (e_totemMaterial)newMaterialList->get_int_val();
}


void MakeNewWorld(int)
{
	if (levelEditorController != NULL) levelEditorController->SaveNewWorld((char *)txtWorldSaveAsName->get_text());
}
*/
/*
void SaveChanges(int)
{
	// create a totemLevel, save as per boost example, destroy instance
	if (levelEditorController != NULL) levelEditorController->Save();
}*/


/*
void UpdateSelectedBlockMaterial(int)
{
	totemBlock* selected = NULL;
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			selected->SetMaterial((e_totemMaterial)selectedBlockMaterialList->get_int_val());
		}
	}*/
	
	/*
	totemBlock* selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL)
	{
		selected->SetMaterial((e_totemMaterial)selectedBlockMaterialList->get_int_val());
	}
	 */
//}

/*
void UpdateSelectedBlockType(int)
{
	totemBlock* selected = NULL;
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			selected->SetBlockType((e_totemBlockType)selectedBlockTypesList->get_int_val());
			
			selected->ResetAABB();
			selected->ResetOBB();
		}
	}*/
	
	/*
	totemBlock* selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL)
	{
		selected->SetBlockType((e_totemBlockType)selectedBlockTypesList->get_int_val());
		
		selected->ResetAABB();
		selected->ResetOBB();
	}*/
//}

/*
void UpdateSelectedBlockTag(int)
{
	totemBlock* selected = NULL;
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			selected->SetTag((int) atoi(selectedBlockTag->get_text()));
		}
	}
	*/
	/*
	totemBlock* selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL)
	{
		selected->SetTag((int) atoi(selectedBlockTag->get_text()));
	} 
	 */
//}

/*
void UpdateSelectedBlockDepth(int)
{
	totemBlock* selected = NULL;
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			if (selectedBlockDepth->get_int_val() == 0)
			{
				selected->SetDepth(-0.02f);
			}
			else
			{
				selected->SetDepth(-((float)selectedBlockDepth->get_int_val())/10.0f);
			}
		}
	}*/
	/*
	
	totemBlock* selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL)
	{
		if (selectedBlockDepth->get_int_val() == 0)
		{
			selected->SetDepth(-0.02f);
		}
		else
		{
			selected->SetDepth(-((float)selectedBlockDepth->get_int_val())/10.0f);
		}
	}	*/
//}

/*
void UpdateSelectedBlockTiling(int)
{
	hdVec3 dim;
	float hwRatio, xTiling;
	totemBlock* selected = NULL;
	
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			dim = (selected->GetAABB().upper - selected->GetAABB().lower);
			
			if (dim.x <= 0.0f) return;
			
			hwRatio = (dim.y / dim.x);
			xTiling		= (float) strtof(selectedBlockTiling->get_text(), NULL);
			
			if (xTiling < 0.01f) xTiling = 1.0f;
			
			selected->SetTextureRepeatX(xTiling);
			selected->SetTextureRepeatY(xTiling);
			
			selected->ResetTextureCoords();
		}
	}
	
	*/
	/*
	totemBlock* selected;
	hdVec3 dim;
	float hwRatio, xTiling;
	
	
	selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL)
	{
		dim = (selected->GetAABB().upper - selected->GetAABB().lower);
		
		if (dim.x <= 0.0f) return;
		
		hwRatio = (dim.y / dim.x);
		xTiling		= (float) strtof(selectedBlockTiling->get_text(), NULL);
		
		if (xTiling < 0.01f) xTiling = 1.0f;
		
		selected->SetTextureRepeatX(xTiling);
		selected->SetTextureRepeatY(xTiling);
		
		selected->ResetTextureCoords();
	}
	 */
//}

/*
void UpdateSelectedBlockZOffset(int)
{
		float zOffset;
	totemBlock* selected = NULL;
	for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
	{
		if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() == e_totemTypeBlock)
		{
			selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[i];
			zOffset = (float) strtof(selectedBlockZOffset->get_text(), NULL);
			zOffset = hdClamp(zOffset, -10.0f, 10.0f);
			selected->SetZOffset(zOffset);
		}
	}
*	
	
	/*
	totemBlock* selected;
	float zOffset;
	
	selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	
	if (selected != NULL)
	{
		zOffset = (float) strtof(selectedBlockZOffset->get_text(), NULL);
		zOffset = hdClamp(zOffset, -10.0f, 10.0f);
		selected->SetZOffset(zOffset);
	}
	 */
/*}*/

/*
void UpdateSelectedBlockTexture(int)
{
	totemBlock* selected = (totemBlock *)levelEditorController->GetSelectedTotemBlock();
	if (selected != NULL && (selected->IsTextureChangeable()))
	{
		if (selectedBlockTexture->get_int_val() != -1)
		{
			selected->SetTextureName(selectedBlockTexture->curr_text.c_str());
		}
		selected->ResetTextureCoords();
	}
}*/


/*
void RefreshLevelList()
{
	if (levelList == NULL) return;
	
	// Refresh the list
	levelList->delete_all();
	
	int count = levelEditorController->GetCurrentTotemWorld()->GetLevelCount();
	for (int i = 0; i < count; i++)
	{
		char levelName[256];
		snprintf(levelName, 256, "%d: %s", i, levelEditorController->GetCurrentTotemWorld()->GetLevels()[i]->GetLevelName());
		levelList->add_item((long)levelEditorController->GetCurrentTotemWorld()->GetLevels()[i], levelName);
	}
	levelList->set_int_val((long)levelEditorController->GetCurrentLevel());
}


void SelectListItem(GLUI_Listbox *list, const char *text)
{
	GLUI_Listbox_Item *item;

	list->do_selection(-1);
	item = (GLUI_Listbox_Item *) list->items_list.first_child();
	while (item)
	{
		if (strncmp(item->text.c_str(), text, 256) == 0) 
		{
			list->do_selection(item->id);
			break;
		}
		item = (GLUI_Listbox_Item *) item->next();
	}	
}*/



void UpdateInterfaceState()
{
	/*
	if (toggleFilterCosmetics == NULL) return;
	
	totemLevel *level = (totemLevel *)levelEditorController->GetCurrentLevel();
	
	//toggleBackground->set_int_val((int)levelEditorController->settings.interfaceLayerMode & e_interfaceLayerModeBackground);
	//toggleGameLayer->set_int_val((int)levelEditorController->settings.interfaceLayerMode & e_interfaceLayerModeGameLayer);
	//toggleMidground->set_int_val((int)levelEditorController->settings.interfaceLayerMode & e_interfaceLayerModeMidground);
	//toggleForground->set_int_val((int)levelEditorController->settings.interfaceLayerMode & e_interfaceLayerModeForeground);
	
	currentLevelExtendAABB->set_int_val((int)level->GetExtendAABB());
	
	toggleFilterCosmetics->set_int_val((int)levelEditorController->settings.showCosmetics);
	toggleFilterGround->set_int_val((int)levelEditorController->settings.showGround);
	toggleFilterPhysics->set_int_val((int)levelEditorController->settings.showPhysics);
	*/
	/*
	char tintParam[32];
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundBottomTint()[0]));
	currentLevelSkyTintBottomRed->set_text(tintParam);
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundBottomTint()[1]));
	currentLevelSkyTintBottomGreen->set_text(tintParam);
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundBottomTint()[2]));
	currentLevelSkyTintBottomBlue->set_text(tintParam);
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundTopTint()[0]));
	currentLevelSkyTintTopRed->set_text(tintParam);
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundTopTint()[1]));
	currentLevelSkyTintTopGreen->set_text(tintParam);
	
	snprintf(tintParam, 32, "%d", (int)(255.0f * level->GetBackgroundTopTint()[2]));
	currentLevelSkyTintTopBlue->set_text(tintParam);*/
	/*
	// Textures
	SelectListItem(currentLevelSkyTexture, level->GetSkyTextureName());
	SelectListItem(currentLevelFarBGTexture, level->GetFarBackgroundTextureName());
	SelectListItem(currentLevelNearBGTexture, level->GetNearBackgroundTextureName());
	SelectListItem(currentLevelDistantBGTexture, level->GetDistantBackgroundTextureName());
	
	totemFloorInfo *floorInfo = (totemFloorInfo *)(level->GetFloorInfo());
	
	if (floorInfo != NULL)
	{
		currentLevelHasFloor->set_int_val(1);
		SelectListItem(currentLevelFloorTexture1, floorInfo->m_firstTextureName);
		SelectListItem(currentLevelFloorTexture2, floorInfo->m_secondTextureName);
		
		char floorLevel[32];
		snprintf(floorLevel, 32, "%3.5f", floorInfo->m_floorLevel);
		currentLevelFloorLevel->set_text(floorLevel);
		currentLevelFloorIsWater->set_int_val(floorInfo->m_isWater);
		currentLevelReflectiveFloor->set_int_val(floorInfo->m_isReflective);
		
		currentLevelFloorTexture1->enable();
		currentLevelFloorTexture2->enable();
		currentLevelFloorLevel->enable();
		currentLevelFloorIsWater->enable();
		currentLevelReflectiveFloor->enable();
	}
	else
	{
		currentLevelHasFloor->set_int_val(0);
		currentLevelFloorTexture1->do_selection(-1);
		currentLevelFloorTexture2->do_selection(-1);
		
		currentLevelFloorLevel->set_text("0");
		currentLevelReflectiveFloor->set_int_val(0);
		
		currentLevelFloorTexture1->disable();
		currentLevelFloorTexture2->disable();
		currentLevelFloorLevel->disable();
		currentLevelFloorIsWater->disable();
		currentLevelReflectiveFloor->disable();
	}
	
	if (levelEditorController->settings.interfaceLayerMode & e_interfaceLayerModeGameLayer)
	{
		if (levelEditorController->m_selectedGameObjects->GetItemCount() == 1)
		{
			if (levelEditorController->m_selectedGameObjects->GetItems()[0]->GetUserType() == e_totemTypeJoint)
			{
				totemJoint* jnt = (totemJoint *)levelEditorController->m_selectedGameObjects->GetItems()[0];
				if (jnt->GetJointType() == e_totemJointTypeRevolute)
				{
					currentJointPanel->enable();
					currentJointPanel->set_name("Revolute Joint Properties");
					currentJointEnableMotor->set_int_val(jnt->GetJointMotorEnabled());
						
					char tmp[32];
					snprintf(tmp, 32, "%3.5f", jnt->GetJointMotorSpeed());
					currentJointMotorSpeed->set_text(tmp);
						
					snprintf(tmp, 32, "%3.5f", jnt->GetRevoluteJointMaxTorque());					
					currentJointMaxForce->set_text(tmp);
					
					currentJointEnableLimit->set_int_val(jnt->GetJointLimitEnabled());
					
					snprintf(tmp, 32, "%3.5f", jnt->GetRevoluteJointLowerAngle());	
					currentJointLowerLimit->set_text(tmp);
					
					snprintf(tmp, 32, "%3.5f", jnt->GetRevoluteJointUpperAngle());
					currentJointUpperLimit->set_text(tmp);
				}
				else if (jnt->GetJointType() == e_totemJointTypePrismatic)
				{
					currentJointPanel->enable();
					currentJointPanel->set_name("Prismatic Joint Properties");
					currentJointEnableMotor->set_int_val(jnt->GetJointMotorEnabled());
					
					char tmp[32];
					snprintf(tmp, 32, "%3.5f", jnt->GetJointMotorSpeed());
					currentJointMotorSpeed->set_text(tmp);
					
					snprintf(tmp, 32, "%3.5f", jnt->GetPrismaticJointMaxMotorForce());					
					currentJointMaxForce->set_text(tmp);
					
					currentJointEnableLimit->set_int_val(jnt->GetJointLimitEnabled());
					
					snprintf(tmp, 32, "%3.5f", jnt->GetPrismaticJointLowerTranslation());	
					currentJointLowerLimit->set_text(tmp);
					
					snprintf(tmp, 32, "%3.5f", jnt->GetPrismaticJointUpperTranslation());
					currentJointUpperLimit->set_text(tmp);
				}
				else
				{
					currentJointPanel->disable();				
				}
			} 
			else if (levelEditorController->m_selectedGameObjects->GetItems()[0]->GetUserType() == e_totemTypeBlock)
			{
				totemBlock* selected = (totemBlock *)levelEditorController->m_selectedGameObjects->GetItems()[0];
				if (selected != NULL)
				{
					selectedBlockPropertiesPanel->enable();
					selectedBlockMaterialList->set_int_val((int)selected->GetMaterial());
					selectedBlockTypesList->set_int_val((int)selected->GetBlockType());
					
					char tmp[32];
					snprintf(tmp, 32, "%d", selected->GetTag());
					selectedBlockTag->set_text(tmp);
					
					snprintf(tmp, 32, "%3.5f", selected->GetTextureRepeatX());
					selectedBlockTiling->set_text(tmp);
					
					snprintf(tmp, 32, "%2.3f", selected->GetZOffset());
					selectedBlockZOffset->set_text(tmp);
					
					
					if (abs(selected->GetDepth()) == 0.02f)
					{
						selectedBlockDepth->set_int_val(0);
					} 
					else
					{
						selectedBlockDepth->set_int_val((int)(-selected->GetDepth()*10));
					}
					
					if (selected->IsTextureChangeable())
					{
						selectedBlockTexture->enable();
						SelectListItem(selectedBlockTexture, selected->GetTextureName());
					}
					else
					{
						selectedBlockTexture->disable();
					}
				}
			}
		}
		else
		{
	 */
			bool allBlocks = levelEditorController->m_selectedGameObjects->GetItemCount() > 1;
			
			for (int i = 0; i < levelEditorController->m_selectedGameObjects->GetItemCount(); ++i)
			{
				 if (levelEditorController->m_selectedGameObjects->GetItems()[i]->GetUserType() != e_totemTypeBlock)
				 {
					 allBlocks = false;
					 break;
				 }
			}
			
			if (allBlocks)
				selectedBlockPropertiesPanel->enable();
			else
				selectedBlockPropertiesPanel->disable();

			
			currentJointPanel->disable();	
		}
	}
	else // we're editing a layer
	{
		// Get selected polygon:
		
	}
}
/*

void UpdateSelectedLevel(int)
{
	if (levelList == NULL) return;
	
	// set current level to the pointer in the list.
	totemLevel *level = (totemLevel *)levelList->get_int_val();
	levelEditorController->SetCurrentLevel(level);
	
	txtLevelName->set_text(level->GetLevelName());
	UpdateInterfaceState();
}*/


void UpdateSelectedWorld(int)
{
	// do nothing yet.
	// Force a load
	txtWorldSaveAsName->set_text(worldList->curr_text);
	levelEditorController->Save();
	levelEditorController->settings.currentWorldPath = (char *)worldList->curr_text.c_str(); //(char*)worldList->get_int_val();
	levelEditorController->Load();
	
	RefreshLevelList();
	UpdateSelectedLevel(0);
	
	UpdateInterfaceState();
}


/*
void AddLevelToSelectedWorld(int)
{
	// create a new level and then add to the list in currnet world.
	levelEditorController->GenerateNewLevel();
	
	RefreshLevelList();
	txtLevelName->set_text(levelEditorController->GetCurrentLevel()->GetLevelName());
}
*/


void EditorFinishCurrentShape(int)
{
	levelEditorController->FinishCurrentShape();
}


/*
void UpdateSelectedPolygonTexture(int)
{
}*/


void UpdateNewPolygonTexture(int)
{
	if (newPolygonTextureList->get_int_val() != -1)
	{
		levelEditorController->settings.newTexturePath = (char *)newPolygonTextureList->curr_text.c_str();
		levelEditorController->settings.newPolygonTint[0] = 1.0f;
		levelEditorController->settings.newPolygonTint[1] = 1.0f;
		levelEditorController->settings.newPolygonTint[2] = 1.0f;
		levelEditorController->settings.newPolygonTint[3] = 1.0f;
	} 
	else
	{
		levelEditorController->settings.newTexturePath = "";
		levelEditorController->settings.newPolygonTint[0] = 0.0f;
		levelEditorController->settings.newPolygonTint[1] = 0.0f;
		levelEditorController->settings.newPolygonTint[2] = 0.0f;
		levelEditorController->settings.newPolygonTint[3] = 1.0f;
	}		
}


/*
 
 const char* GetFarBackgroundTextureName() const;
 void SetFarBackgroundTextureName(const char* textureName);
 
 const char* GetNearBackgroundTextureName() const;
 void SetNearBackgroundTextureName(const char* textureName);
 
 const float* GetBackgroundBottomTint() const;
 void SetBackgroundBottomTint(const int r, const int g, const int b);
 
 const float* GetBackgroundTopTint() const;
 void SetBackgroundTopTint(const int r, const int g, const int b);
 
 // Level properties
 GLUI_Panel *currentLevelProperties;
 GLUI_Listbox *currentLevelNearBGTexture;
 GLUI_Listbox *currentLevelFarBGTexture;
 
 // Sky tint
 GLUI_Panel *currentLevelSkyTintPanel;
 GLUI_EditText *currentLevelSkyTintBottomRed;
 GLUI_EditText *currentLevelSkyTintBottomGreen;
 GLUI_EditText *currentLevelSkyTintBottomBlue;
 
 GLUI_EditText *currentLevelSkyTintTopRed;
 GLUI_EditText *currentLevelSkyTintTopGreen;
 GLUI_EditText *currentLevelSkyTintTopBlue;
 
 // Level floor info
 GLUI_Checkbox *currentLevelHasFloor;
 GLUI_Listbox *currentLevelFloorTexture1;
 GLUI_Listbox *currentLevelFloorTexture2;
 GLUI_EditText *currentLevelFloorLevel;
 GLUI_Checkbox *currentLevelReflectiveFloor;
 
 
 */
/*
void UpdateLevelExtendAABB(int)
{
	totemLevel *level = (totemLevel *)levelEditorController->GetCurrentLevel();
	
	if (currentLevelExtendAABB->get_int_val() == 0)
	{
		level->SetExtendAABB(false);
	} 
	else 
	{
		level->SetExtendAABB(true);
	}	
}
*/
/*
void UpdateLevelBG(int)
{
	totemLevel *level = (totemLevel *)levelEditorController->GetCurrentLevel();
	
	if (currentLevelSkyTexture->get_int_val() != -1)
	{
		level->SetSkyTextureName(currentLevelSkyTexture->curr_text.c_str());
	}
	else
	{
		level->SetSkyTextureName("");
	}
	
	
	if (currentLevelFarBGTexture->get_int_val() != -1)
	{
		level->SetFarBackgroundTextureName(currentLevelFarBGTexture->curr_text.c_str());
	}
	else
	{
		level->SetFarBackgroundTextureName("");
	}
	
	if (currentLevelNearBGTexture->get_int_val() != -1)
	{
		level->SetNearBackgroundTextureName(currentLevelNearBGTexture->curr_text.c_str());
	}
	else
	{
		level->SetNearBackgroundTextureName("");
	}
	
	if (currentLevelDistantBGTexture->get_int_val() != -1)
	{
		level->SetDistantBackgroundTextureName(currentLevelDistantBGTexture->curr_text.c_str());
	}
	else
	{
		level->SetDistantBackgroundTextureName("");
	}
	*/
	/*
	level->SetBackgroundBottomTint((int) atoi(currentLevelSkyTintBottomRed->get_text()),
								   (int) atoi(currentLevelSkyTintBottomGreen->get_text()),
								   (int) atoi(currentLevelSkyTintBottomBlue->get_text()));
	
	level->SetBackgroundTopTint((int) atoi(currentLevelSkyTintTopRed->get_text()),
								(int) atoi(currentLevelSkyTintTopGreen->get_text()),
								(int) atoi(currentLevelSkyTintTopBlue->get_text()));*/
///}


/*
 * Convenience function applies the background values to all levels in the
 * world after the current.
 */
void ApplyBGValuesToAllFollowingLevels()
{
	totemLevel *level;
	
	int index = -1;
	for (int i = 0; i < levelEditorController->GetCurrentTotemWorld()->GetLevelCount(); i++)
	{
		if (levelEditorController->GetCurrentTotemWorld()->GetLevels()[i] == levelEditorController->GetCurrentLevel())
		{
			index = i;
			break;
		}
	}
	
	if (index == -1) return;
	
	for (int i = index; i < levelEditorController->GetCurrentTotemWorld()->GetLevelCount(); i++)
	{
		level = (totemLevel *)levelEditorController->GetCurrentTotemWorld()->GetLevels()[i];
						
		if (currentLevelSkyTexture->get_int_val() != -1)
		{
			level->SetSkyTextureName(currentLevelSkyTexture->curr_text.c_str());
		}
		else
		{
			level->SetSkyTextureName("");
		}
		
		if (currentLevelFarBGTexture->get_int_val() != -1)
		{
			level->SetFarBackgroundTextureName(currentLevelFarBGTexture->curr_text.c_str());
		}
		else
		{
			level->SetFarBackgroundTextureName("");
		}
		
		if (currentLevelNearBGTexture->get_int_val() != -1)
		{
			level->SetNearBackgroundTextureName(currentLevelNearBGTexture->curr_text.c_str());
		}
		else
		{
			level->SetNearBackgroundTextureName("");
		}
		
		if (currentLevelDistantBGTexture->get_int_val() != -1)
		{
			level->SetDistantBackgroundTextureName(currentLevelDistantBGTexture->curr_text.c_str());
		}
		else
		{
			level->SetDistantBackgroundTextureName("");
		}
		
		/*
		level->SetBackgroundBottomTint((int) atoi(currentLevelSkyTintBottomRed->get_text()),
									   (int) atoi(currentLevelSkyTintBottomGreen->get_text()),
									   (int) atoi(currentLevelSkyTintBottomBlue->get_text()));
		
		level->SetBackgroundTopTint((int) atoi(currentLevelSkyTintTopRed->get_text()),
									(int) atoi(currentLevelSkyTintTopGreen->get_text()),
									(int) atoi(currentLevelSkyTintTopBlue->get_text()));*/
	}
}


/*
void UpdateLevelFloor(int)
{
	totemLevel *level = (totemLevel *)levelEditorController->GetCurrentLevel();
	if (currentLevelHasFloor->get_int_val() != 0)
	{
		currentLevelFloorTexture1->enable();
		currentLevelFloorTexture2->enable();
		currentLevelFloorLevel->enable();
		currentLevelFloorIsWater->enable();
		currentLevelReflectiveFloor->enable();
				
		totemFloorInfo *floorInfo = (totemFloorInfo *)(level->GetFloorInfo());
		
		if (floorInfo == NULL)
		{
			floorInfo = new totemFloorInfo();
			level->SetFloorInfo(floorInfo);
		}
		
		if (currentLevelFloorTexture1->get_int_val() != -1)
		{
			snprintf(floorInfo->m_firstTextureName, kMaxTexturePathSize, "%s", currentLevelFloorTexture1->curr_text.c_str());
		}
		
		if (currentLevelFloorTexture2->get_int_val() != -1)
		{
			snprintf(floorInfo->m_secondTextureName, kMaxTexturePathSize, "%s", currentLevelFloorTexture2->curr_text.c_str());
		}
		floorInfo->m_floorLevel =  (float) strtof(currentLevelFloorLevel->get_text(), NULL);
		floorInfo->m_isWater = (currentLevelFloorIsWater->get_int_val() != 0);
		floorInfo->m_isReflective = (currentLevelReflectiveFloor->get_int_val() != 0);
	}
	else
	{
		currentLevelFloorTexture1->disable();
		currentLevelFloorTexture2->disable();
		currentLevelFloorLevel->disable();
		currentLevelFloorIsWater->disable();
		currentLevelReflectiveFloor->disable();
		level->SetFloorInfo(NULL);
	}
}
*/






void SetInterfaceMode_AddPolygonQuad(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeLayerPolygonQuad;
}


void SetInterfaceMode_AddPolygonQuadActualSize(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeLayerPolygonQuadActualSize;
}


void SetInterfaceMode_AddPolygon(int)
{
	levelEditorController->FinishCurrentPolygon();
	btnEditorFinishCurrentPolygon->enable();
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeLayerPolygon;
}


void EditorFinishCurrentPolygon(int)
{
	btnEditorFinishCurrentPolygon->disable();
	levelEditorController->FinishCurrentPolygon();
}

/*
void SetFilters(int)
{
	
	levelEditorController->settings.showCosmetics = (toggleFilterCosmetics->get_int_val() != 0);
	levelEditorController->settings.showGround = (toggleFilterGround->get_int_val() != 0);
	levelEditorController->settings.showPhysics = (toggleFilterPhysics->get_int_val() != 0);
	
	UpdateInterfaceState();
}*/


void SetLayer_Background(int)
{
	levelEditorController->ToggleLayerMode(e_interfaceLayerModeBackground);
	UpdateInterfaceState();
}


void SetLayer_GameLayer(int)
{
	levelEditorController->ToggleLayerMode(e_interfaceLayerModeGameLayer);
	UpdateInterfaceState();
}


void SetLayer_Midground(int)
{
	levelEditorController->ToggleLayerMode(e_interfaceLayerModeMidground);
	UpdateInterfaceState();
}


void SetLayer_Foreground(int)
{
	levelEditorController->ToggleLayerMode(e_interfaceLayerModeForeground);
	UpdateInterfaceState();
}


void UpdateCurrentLevelName(int)
{
	totemLevel* lev = (totemLevel*)levelEditorController->GetCurrentLevel();
	lev->SetLevelName(txtLevelName->get_text());
	RefreshLevelList();
}

/*
void UpdateCurrentWorldName(int)
{	
	txtWorldSaveAsName->set_text(worldList->curr_text);
	
	// Get the value 
	levelEditorController->settings.currentWorldPath = (char *)worldList->curr_text.c_str();
	levelEditorController->Load();
	
	RefreshLevelList();
}


void UpdateWorldFileSaveName(int)
{
	levelEditorController->settings.currentWorldPath = (char *)txtWorldSaveAsName->get_text();
}*/

/*
void DeleteCurrentLevel(int)
{
	totemLevel *lev;
	levelEditorController->settings.deleteButtonTaps += 1;
	
	if (levelEditorController->settings.deleteButtonTaps >= 3)
	{
		// Do a bakup save
		levelEditorController->Save();
		
		// Remove the level
		int index = -1;
		for (int i = 0; i < levelEditorController->GetCurrentTotemWorld()->GetLevelCount(); i++)
		{
			if (levelEditorController->GetCurrentTotemWorld()->GetLevels()[i] == levelEditorController->GetCurrentLevel())
			{
				index = i;
				break;
			}
		}
		
		if (index == -1) return;
		
		if (index == 0)
		{
			lev = levelEditorController->GetCurrentTotemWorld()->GetLevels()[1];
		}
		else
		{
			lev = levelEditorController->GetCurrentTotemWorld()->GetLevels()[0];
		}
		
		
		levelEditorController->SetCurrentLevel(lev);
		txtLevelName->set_text(lev->GetLevelName());
		
		levelEditorController->GetCurrentTotemWorld()->RemoveLevelAtIndex(index);
		
		UpdateInterfaceState();
		
		RefreshLevelList();
		
		levelEditorController->settings.deleteButtonTaps = 0;
	}
}*/



void CopyCurrentLevel(int)
{
	levelEditorController->CopyCurrentLevel();
	currentLevelPaste->enable();
}


void PasteCopiedLevel(int)
{
	if (levelEditorController == NULL) return;
	
	levelEditorController->Save();
	levelEditorController->PasteCopiedLevel();
	currentLevelPaste->disable();
}

/*
void UpdateJointProperties(int)
{
	if (levelEditorController->settings.DEPRECATEDselectedGameObject != NULL)
	{
		if (levelEditorController->settings.DEPRECATEDselectedGameObject->GetUserType() == e_totemTypeJoint)
		{
			totemJoint* jnt = (totemJoint *)levelEditorController->settings.DEPRECATEDselectedGameObject;
			if (jnt->GetJointType() == e_totemJointTypeRevolute)
			{
				bool enableMotor, enableLimit;
				float motorSpeed, maxMotorTorque, lowerAngle, upperAngle;
				
				enableMotor = (currentJointEnableMotor->get_int_val() != 0);
				enableLimit = (currentJointEnableLimit->get_int_val() != 0);
				
				motorSpeed		= (float) strtof(currentJointMotorSpeed->get_text(), NULL);
				maxMotorTorque	= (float) strtof(currentJointMaxForce->get_text(), NULL);
				
				if (enableLimit)
				{
					lowerAngle		= (float) strtof(currentJointLowerLimit->get_text(), NULL);
					upperAngle		= (float) strtof(currentJointUpperLimit->get_text(), NULL);
				}
				else
				{
					lowerAngle = upperAngle = 0.0f;
				}
				
				jnt->SetRevoluteJointProperties(enableMotor, enableLimit, lowerAngle, upperAngle, maxMotorTorque, motorSpeed);
			}
			else if (jnt->GetJointType() == e_totemJointTypePrismatic)
			{
				bool enableMotor, enableLimit;
				float motorSpeed, maxMotorForce, lowerTranslation, upperTranslation;
				
				enableMotor = (currentJointEnableMotor->get_int_val() != 0);
				enableLimit = (currentJointEnableLimit->get_int_val() != 0);
				
				motorSpeed		= (float) strtof(currentJointMotorSpeed->get_text(), NULL);
				maxMotorForce	= (float) strtof(currentJointMaxForce->get_text(), NULL);
				
				if (enableLimit)
				{
					lowerTranslation		= (float) strtof(currentJointLowerLimit->get_text(), NULL);
					upperTranslation		= (float) strtof(currentJointUpperLimit->get_text(), NULL);
				}
				else
				{
					lowerTranslation = upperTranslation = 0.0f;
				}
				
				jnt->SetPrismaticJointProperties(enableMotor, enableLimit, motorSpeed, lowerTranslation, upperTranslation, maxMotorForce);
			}
		}
	}
}*/

/*
void Keyboard(unsigned char key, int x, int y)
{
	B2_NOT_USED(x);
	B2_NOT_USED(y);
	
	int GLUTmodifiers = glutGetModifiers();
	
	// HACK HACK HACK 
	// key != ascii char num when modifiers on
	if (GLUTmodifiers > 0)
	{
		key = 96 + (unsigned int)key;
	}
	
	switch (key)
	{
		case 8: // Delete key
		case 127:
			levelEditorController->DeleteKey();
			break;
		case 9: // tab - switch view
			m_perspectiveProjection = !m_perspectiveProjection;
			break;
		case 27:
			exit(0);
			break;
			
			// Press 'z' to zoom out.
		case 'z':
			viewZoom = b2Min(1.1f * viewZoom, 20.0f);
			Resize(width, height);
			break;
			
			// Press 'x' to zoom in.
		case 'x':
			viewZoom = b2Max(0.9f * viewZoom, 0.02f);
			Resize(width, height);
			break;
		case 'c':
			if (GLUTmodifiers == GLUT_ACTIVE_ALT)
			{ 
				levelEditorController->CopySelectedObjects();
			}
			
			break;
		case 'v':
			if (GLUTmodifiers == GLUT_ACTIVE_ALT)
			{
				levelEditorController->PasteCopiedObjects();
			}
			else if (GLUTmodifiers == 0)
			{
				levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
			}
			break;
		case 't':
			if (GLUTmodifiers == GLUT_ACTIVE_ALT)
			{
				levelEditorController->ApplyCurrentTextureToSelected(newPolygonTextureList->curr_text.c_str());
			}
			break;
		case 'p':
			if (GLUTmodifiers == GLUT_ACTIVE_ALT)
			{
				levelEditorController->RepairSelectedShape();
			}
			break;
		case 'd':
			if (GLUTmodifiers == GLUT_ACTIVE_ALT)
			{
				levelEditorController->DumpUniqueTextureList();
			}
			break;
		case 'b':
		case 'B':
			if (GLUTmodifiers == (GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT))
			{
				ApplyBGValuesToAllFollowingLevels();
			}
			break;
		case 'r':
			//delete test;
			hdAssert(b2_byteCount == 0);
			//test = entry->createFcn();
			break;
			
			// Press space to launch a bomb.
		case ' ':
			//if (test)
			//{
			//test->LaunchBomb();
			//}
			break;
			
		default:
			//if (test)
			//{
			//test->Keyboard(key);
			//}
			break;
	}
}*/


/*
void KeyboardSpecial(int key, int x, int y)
{
	B2_NOT_USED(x);
	B2_NOT_USED(y);
	
	switch (key)
	{
			// Press left to pan left.
		case GLUT_KEY_LEFT:
			viewCenter.x -= 0.5f;
			Resize(width, height);
			break;
			
			// Press right to pan right.
		case GLUT_KEY_RIGHT:
			viewCenter.x += 0.5f;
			Resize(width, height);
			break;
			
			// Press down to pan down.
		case GLUT_KEY_DOWN:
			viewCenter.y -= 0.5f;
			Resize(width, height);
			break;
			
			// Press up to pan up.
		case GLUT_KEY_UP:
			viewCenter.y += 0.5f;
			Resize(width, height);
			break;
			
			// Press home to reset the view.
		case GLUT_KEY_HOME:
			viewZoom = 1.0f;
			viewCenter.Set(0.0f, 20.0f);
			Resize(width, height);
			break;
		default:
			ctrlDown = false;
			break;
	}
}*/



/*
void Mouse(int32 button, int32 state, int32 x, int32 y)
{
	int GLUTmodifiers = glutGetModifiers();
	if (GLUTmodifiers == (GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT))
	{
		hdPrintf("ALTSHIFT\n");
	}
	
	if (button == GLUT_LEFT_BUTTON)
	{
		//rMouseDown = false;
		if (state == GLUT_DOWN)
		{
			hdVec2 p = ConvertScreenToWorld(x, y);
			levelEditorController->MouseDown(p);
		}
		
		if (state == GLUT_UP)
		{
			
			levelEditorController->MouseUp();
		}
	} 
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{	
			lastp = ConvertScreenToWorld(x, y);
			rMouseDown = true;
		}
		
		if (state == GLUT_UP)
		{
			rMouseDown = false;
		}
	}
	UpdateInterfaceState();
}






void MouseMotion(int32 x, int32 y)
{
	hdVec2 p = ConvertScreenToWorld(x, y);
	
	if (rMouseDown == false) 
	{
		levelEditorController->MouseMove(p);
		
	}
	
	if (rMouseDown){
		hdVec2 diff = p - lastp;
		viewCenter.x -= diff.x;
		viewCenter.y -= diff.y;
		Resize(width, height);
		lastp = ConvertScreenToWorld(x, y);
	}
}*/


/*
void MouseWheel(int wheel, int direction, int x, int y)
{	B2_NOT_USED(wheel);
	B2_NOT_USED(x);
	B2_NOT_USED(y);
	if (direction > 0) {
		viewZoom /= 1.1f;
	} else {
		viewZoom *= 1.1f;
	}
	Resize(width, height);
}
*/

///////////////////////////////////////////////////////////////////////////
///
/// NOTE TO SELF: GLUI sucks. Find something better.
///
///////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv) 
{
	int i = 0;
	list<string>::iterator it;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	
	char title[32];
	sprintf(title, "iTotem Level Designer");
	mainWindow = glutCreateWindow(title);
	//popupWindow = glutCreateSubWindow(mainWindow, 100, 100, 200, 200);
	
	FileSystem_Init("/Users/david/Projects/Hackdirt/TotemGames/res");
	levelEditorController = new hdLevelEditorController();
	//levelEditorController->Load();
	//levelEditorController->DoUnitTest();
	
	
	
	
	glutDisplayFunc(SimulationLoop);
	GLUI_Master.set_glutReshapeFunc(Resize);  
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
	GLUI_Master.set_glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	
	
	glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);
	
	/*
	glui->add_separator();
	GLUI_Spinner* hertzSpinner = glui->add_spinner("Hertz", GLUI_SPINNER_FLOAT, &settingsHz);
	hertzSpinner->set_float_limits(5.0f, 200.0f);
	glui->add_separator();
	modeInfo = glui->add_statictext("Cursor");
	*/
	 
	///////////////////////////////////////////////////////////////////////////
	///
	/// world and level selector
	///
	///////////////////////////////////////////////////////////////////////////
	GLUI_Panel* worldPanel = glui->add_panel("Level/World");
	
//	txtWorldName;
//	GLUI_Listbox* levelList;
//	GLUI_TextBox* txtLevelName
	/*
	 GLUI_EditText  *GLUI::add_edittext_to_panel( GLUI_Panel *panel, 
	 const char *name, 
	 int data_type, void *data,
	 int id, GLUI_CB callback)
	 
*/	 
	worldList = glui->add_listbox_to_panel(worldPanel, "World", NULL, 0, UpdateSelectedWorld);
	m_worldFilenames = hdGetFileNames(FileSystem_BaseDir(), "Worlds");
	
	
	i = 0;
	it = m_worldFilenames.begin();
	while(it != m_worldFilenames.end())
	{
		if (strlen((*it).c_str()) > 0)
		{
			worldList->add_item(i++, (*it).c_str());
		}
		it++;
	}
	
	
	worldList->w = 200;
	txtWorldSaveAsName = glui->add_edittext_to_panel(worldPanel, "Filename", 0, NULL, 0, UpdateWorldFileSaveName);
	txtWorldSaveAsName->w = 200;
	
	txtWorldName = glui->add_edittext_to_panel(worldPanel, "World Name", 0, NULL, 0, UpdateCurrentWorldName);
	txtWorldName->w = 200;
	
	UpdateSelectedWorld(0);
																	  
	glui->add_button_to_panel(worldPanel, "Add Level", 0, AddLevelToSelectedWorld);
	levelList = glui->add_listbox_to_panel(worldPanel, "Level", NULL, 0, UpdateSelectedLevel);
	for (int i = 0; i < levelEditorController->GetCurrentTotemWorld()->GetLevelCount(); i++)
	{
		char levelName[256];
		snprintf(levelName, 256, "%d: %s", i, levelEditorController->GetCurrentTotemWorld()->GetLevels()[i]->GetLevelName());
		levelList->add_item((long)levelEditorController->GetCurrentTotemWorld()->GetLevels()[i], levelName);
	}
	levelList->w = 200;
	txtLevelName = glui->add_edittext_to_panel(worldPanel, "Level Name", 0, NULL, 0, UpdateCurrentLevelName);
	txtLevelName->w = 200;

	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// layer selector
	///
	///////////////////////////////////////////////////////////////////////////
	//GLUI_Panel* layerPanel = glui->add_panel("Filters");
	//toggleBackground = new GLUI_Checkbox(layerPanel, "Background Layer", NULL, 0, SetLayer_Background);
	//toggleGameLayer = new GLUI_Checkbox(layerPanel, "* Game Layer *", NULL, 0, SetLayer_GameLayer);
	//toggleMidground = new GLUI_Checkbox(layerPanel, "Midground Layer", NULL, 0, SetLayer_Midground);
	//toggleForground = new GLUI_Checkbox(layerPanel, "Foreground Layer", NULL, 0, SetLayer_Foreground);
	//toggleBackground = new GLUI_Checkbox("Background Layer", NULL, 0, SetLayer_Background);
	//toggleGameLayer = new GLUI_Checkbox("* Game Layer *", NULL, 0, SetLayer_GameLayer);
	//toggleMidground = new GLUI_Checkbox("Midground Layer", NULL, 0, SetLayer_Midground);
	//toggleForground = new GLUI_Checkbox("Foreground Layer", NULL, 0, SetLayer_Foreground);	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// filter selector
	///
	///////////////////////////////////////////////////////////////////////////
	GLUI_Panel* filterPanel = glui->add_panel("Filters");
	toggleFilterCosmetics = new GLUI_Checkbox(filterPanel, "Cosmetics", NULL, 0, SetFilters);
	toggleFilterGround = new GLUI_Checkbox(filterPanel, "Ground Objects", NULL, 0, SetFilters);
	toggleFilterPhysics = new GLUI_Checkbox(filterPanel, "Physics Objects", NULL, 0, SetFilters);
	
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// cursor palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	glui->add_separator();
	glui->add_button("Cursor", 0, SetInterfaceMode_ActiveCursor);
	glui->add_separator();

	
	///////////////////////////////////////////////////////////////////////////
	///
	/// texture palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	GLUI_Panel* texturePalettePanel = glui->add_panel("Texture Palette");
	m_textureFilenames = hdGetFileNames(FileSystem_BaseDir(), "/Textures");
	
	newPolygonTextureList = new GLUI_Listbox(texturePalettePanel, "Texture", NULL, 0, UpdateNewPolygonTexture);
	newPolygonTextureList->add_item(-1, "*** Black ***");
	
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		newPolygonTextureList->add_item(i++, (*it).c_str());
		it++;
	}
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// polygon buttons
	///
	///////////////////////////////////////////////////////////////////////////
	newPolygonPropertiesPanel = glui->add_panel("Polygon");
	glui->add_button_to_panel(newPolygonPropertiesPanel, "Actual Size Rectangle", 0, SetInterfaceMode_AddPolygonQuadActualSize);
	glui->add_button_to_panel(newPolygonPropertiesPanel, "Add Rectangle", 0, SetInterfaceMode_AddPolygonQuad);
	glui->add_button_to_panel(newPolygonPropertiesPanel, "Add Polygon", 0, SetInterfaceMode_AddPolygon);
	btnEditorFinishCurrentPolygon = glui->add_button_to_panel(newPolygonPropertiesPanel, "Done", 0, EditorFinishCurrentPolygon);
	btnEditorFinishCurrentPolygon->disable();
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// shape palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	//newShapePropertiesPanel =	glui->add_panel("Shape");
	//glui->add_button_to_panel(newShapePropertiesPanel, "Add Shape", 0, SetInterfaceMode_AddShape);
	//glui->add_button_to_panel(newShapePropertiesPanel, "Done", 0, EditorFinishCurrentShape);
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// event palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	newEventsPropertiesPanel = glui->add_panel("Events");
	//GLUI_Button* btnNewEvent;
	btnNewEvent = glui->add_button_to_panel(newEventsPropertiesPanel, "Add Event", 0, SetInterfaceMode_AddEvent);
	//GLUI_Listbox* btnEventTypesList;
	newEventTypesList = glui->add_listbox_to_panel(newEventsPropertiesPanel, "Event Type", NULL, 0, UpdateNewEventType);
	for (int i = 0; i < e_totemEventType_Count; i++)
	{
		newEventTypesList->add_item((int)totemEventTypeMenuItems[i].eventType, totemEventTypeMenuItems[i].eventTypeName);
	}
	
	/*
	newJackPropertiesPanel = glui->add_panel("Jack");
	glui->add_button_to_panel(newJackPropertiesPanel, "Add Jack", 0, SetInterfaceMode_AddJack);
	newJackTypesList = glui->add_listbox_to_panel(newJackPropertiesPanel, "Jack Type", NULL, 0, UpdateNewJackType);
	for (int i = 0; i < e_totemJackType_Count; i++)
	{
		newJackTypesList->add_item((int)totemJackTypeMenuItems[i].jackType, totemJackTypeMenuItems[i].jackTypeName);
	}*/
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// block palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	newBlockPropertiesPanel = glui->add_panel("Block");
	glui->add_button_to_panel(newBlockPropertiesPanel, "Add Totem Block", 0, SetInterfaceMode_AddTotemBlock);
	newBlockTypesList = glui->add_listbox_to_panel(newBlockPropertiesPanel, "Block Type", NULL, 0, UpdateNewBlockType);
	for (int i = 0; i < e_totemBlockType_Count; i++)
	{
		newBlockTypesList->add_item((int)totemBlockTypeMenuItems[i].blockType, totemBlockTypeMenuItems[i].blockTypeName);
	}
	
	newBlockShapesList = glui->add_listbox_to_panel(newBlockPropertiesPanel, "Shape", NULL, 0, UpdateNewBlockShapeType);
	for (int i = 0; i < e_totemShapeType_Count; i++)
	{
		newBlockShapesList->add_item((int)totemShapeTypeMenuItems[i].shapeType, totemShapeTypeMenuItems[i].shapeTypeName);
	}
	
	newMaterialList = glui->add_listbox_to_panel(newBlockPropertiesPanel, "Material", NULL, 0, UpdateNewBlockMaterial);
	for (int i = 0; i < e_totemMaterial_Count; i++)
	{
		newMaterialList->add_item((int)totemMaterialMenuItems[i].material, totemMaterialMenuItems[i].materialName);
	}
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// joint palette properties
	///
	///////////////////////////////////////////////////////////////////////////
	newJointPropertiesPanel = glui->add_panel("Joints");
	btnAddNewJoint = glui->add_button_to_panel(newJointPropertiesPanel, "Add Joint", 0, SetInterfaceMode_AddJoint);
	newJointTypesList = glui->add_listbox_to_panel(newJointPropertiesPanel, "Joint Type", NULL, 0, UpdateNewJointType);
	for (int i = 0; i < e_totemJoinType_Count; i++)
	{
		newJointTypesList->add_item((int)totemJointTypeMenuItems[i].jointType, totemJointTypeMenuItems[i].jointTypeName);
	}
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// Selected Item Panels
	///
	///////////////////////////////////////////////////////////////////////////
	gluiBlockProperties = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);
	selectedBlockPropertiesPanel =	gluiBlockProperties->add_panel("Block Properties");
	selectedBlockMaterialList = new GLUI_Listbox(selectedBlockPropertiesPanel, "Material", NULL, 0, UpdateSelectedBlockMaterial);
	for (int i = 0; i < e_totemMaterial_Count; i++)
	{
		selectedBlockMaterialList->add_item((int)totemMaterialMenuItems[i].material, totemMaterialMenuItems[i].materialName);
	}
	
	selectedBlockTypesList = new GLUI_Listbox(selectedBlockPropertiesPanel, "Block Type", NULL, 0, UpdateSelectedBlockType);
	for (int i = 0; i < e_totemBlockType_Count; i++)
	{
		selectedBlockTypesList->add_item((int)totemBlockTypeMenuItems[i].blockType, totemBlockTypeMenuItems[i].blockTypeName);
	}
	selectedBlockPropertiesPanel->disable();
	
	selectedBlockTag = gluiBlockProperties->add_edittext_to_panel(selectedBlockPropertiesPanel, "Tag", 0, NULL, 0, UpdateSelectedBlockTag);
	selectedBlockDepth = new GLUI_Listbox(selectedBlockPropertiesPanel, "Depth", NULL, 0, UpdateSelectedBlockDepth);
	selectedBlockTiling = gluiBlockProperties->add_edittext_to_panel(selectedBlockPropertiesPanel, "Tiling", 0, NULL, 0, UpdateSelectedBlockTiling);
	selectedBlockZOffset = gluiBlockProperties->add_edittext_to_panel(selectedBlockPropertiesPanel, "Z Offset", 0, NULL, 0, UpdateSelectedBlockZOffset);
	//selectedBlockTiling = new GLUI_Listbox(selectedBlockPropertiesPanel, "Tiling", NULL, 0, UpdateSelectedBlockTiling);
	
	char itemName[32];
	selectedBlockDepth->add_item(0, "0.02 units");
	for (int i = 1; i < 30; i++)
	{
		snprintf(itemName, 32, "%2.2f units", ((float)i)/10.0f);
		selectedBlockDepth->add_item(i, itemName);
	}
	
	selectedBlockTexture = new GLUI_Listbox(selectedBlockPropertiesPanel, "Texture", NULL, 0, UpdateSelectedBlockTexture);
	selectedBlockTexture->add_item(-1, "None");
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		selectedBlockTexture->add_item(i++, (*it).c_str());
		it++;
	}
	
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// Selected Polygon Panels
	///
	///////////////////////////////////////////////////////////////////////////
	selectedPolygonPropertiesPanel = gluiBlockProperties->add_panel("Polygon Properties");
	selectedTextureList = new GLUI_Listbox(selectedPolygonPropertiesPanel, "Material", NULL, 0, UpdateSelectedPolygonTexture);

	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		selectedTextureList->add_item(i++, (*it).c_str());
		it++;
	}
	
	selectedPolygonPropertiesPanel->disable();
	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// Level properties
	///
	///////////////////////////////////////////////////////////////////////////
	currentLevelProperties = gluiBlockProperties->add_panel("Level Properties");
	currentLevelExtendAABB = new GLUI_Checkbox(currentLevelProperties, "Extend Level AABB", NULL, 0, UpdateLevelExtendAABB);

	currentLevelSkyTexture = new GLUI_Listbox(currentLevelProperties, "Sky", NULL, 0, UpdateLevelBG);
	currentLevelSkyTexture->add_item(-1, "None");
	
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelSkyTexture->add_item(i++, (*it).c_str());
		it++;
	}	
	
	
	currentLevelNearBGTexture = new GLUI_Listbox(currentLevelProperties, "Near BG", NULL, 0, UpdateLevelBG);
	currentLevelNearBGTexture->add_item(-1, "None");

	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelNearBGTexture->add_item(i++, (*it).c_str());
		it++;
	}
	
	currentLevelFarBGTexture = new GLUI_Listbox(currentLevelProperties, "Far BG", NULL, 0, UpdateLevelBG);
	currentLevelFarBGTexture->add_item(-1, "None");
	
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelFarBGTexture->add_item(i++, (*it).c_str());
		it++;
	}
	
	currentLevelDistantBGTexture = new GLUI_Listbox(currentLevelProperties, "Hor. BG", NULL, 0, UpdateLevelBG);
	currentLevelDistantBGTexture->add_item(-1, "None");
	
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelDistantBGTexture->add_item(i++, (*it).c_str());
		it++;
	}
	
	/*
	currentLevelSkyTintPanel  = gluiBlockProperties->add_panel_to_panel(currentLevelProperties, "Sky Tint", 1);
	currentLevelSkyTintBottomRed = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Bottom R", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintBottomRed->w = 100;
	currentLevelSkyTintBottomGreen = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Bottom G", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintBottomGreen->w = 100;
	currentLevelSkyTintBottomBlue = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Bottom B", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintBottomBlue->w = 100;

	currentLevelSkyTintTopRed = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Top R", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintTopRed->w = 100;
	currentLevelSkyTintTopGreen = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Top G", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintTopGreen->w = 100;
	currentLevelSkyTintTopBlue = glui->add_edittext_to_panel(currentLevelSkyTintPanel, "Top B", 0, NULL, 0, UpdateLevelBG);
	currentLevelSkyTintTopBlue->w = 100;*/
	
	currentLevelHasFloor = new GLUI_Checkbox(currentLevelProperties, "Level has floor controller", NULL, 0, UpdateLevelFloor);
	currentLevelFloorTexture1 = new GLUI_Listbox(currentLevelProperties, "Tex 1", NULL, 0, UpdateLevelFloor);
	currentLevelFloorTexture1->add_item(-1, "None");
	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelFloorTexture1->add_item(i++, (*it).c_str());
		it++;
	}
	currentLevelFloorTexture1->w = 200;
	currentLevelFloorTexture1->disable();
	
	currentLevelFloorTexture2 = new GLUI_Listbox(currentLevelProperties, "Tex 2", NULL, 0, UpdateLevelFloor);
	currentLevelFloorTexture2->add_item(-1, "None");

	i = 0;
	it = m_textureFilenames.begin();
	while(it != m_textureFilenames.end())
	{
		currentLevelFloorTexture2->add_item(i++, (*it).c_str());
		it++;
	}
	currentLevelFloorTexture2->disable();
	
	currentLevelFloorLevel = glui->add_edittext_to_panel(currentLevelProperties, "Floor Level", 0, NULL, 0, UpdateLevelFloor);
	currentLevelFloorLevel->w = 200;
	currentLevelFloorLevel->disable();
	
	currentLevelFloorIsWater = new GLUI_Checkbox(currentLevelProperties, "Is Water?", NULL, 0, UpdateLevelFloor);
	currentLevelFloorIsWater->disable();
	
	currentLevelReflectiveFloor = new GLUI_Checkbox(currentLevelProperties, "Reflective floor", NULL, 0, UpdateLevelFloor);
	currentLevelReflectiveFloor->disable();
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// Joint Properties - Used for both rev and prismatic joints.
	///
	///////////////////////////////////////////////////////////////////////////
	
	currentJointPanel = gluiBlockProperties->add_panel("Joint Properties");
	currentJointPanel->disable();
	
	currentJointEnableMotor = new GLUI_Checkbox(currentJointPanel, "Motor Enabled", NULL, 0, UpdateJointProperties);
	
	currentJointMotorSpeed = glui->add_edittext_to_panel(currentJointPanel, "Motor Speed", 0, NULL, 0, UpdateJointProperties);
	currentJointMotorSpeed->w = 200;
	
	currentJointMaxForce = glui->add_edittext_to_panel(currentJointPanel, "Max Motor Force", 0, NULL, 0, UpdateJointProperties);
	currentJointMaxForce->w = 200;
	
	currentJointEnableLimit = new GLUI_Checkbox(currentJointPanel, "Limit Enabled", NULL, 0, UpdateJointProperties);
	
	currentJointLowerLimit = glui->add_edittext_to_panel(currentJointPanel, "Lower Limit", 0, NULL, 0, UpdateJointProperties);
	currentJointLowerLimit->w = 200;
	
	currentJointUpperLimit = glui->add_edittext_to_panel(currentJointPanel, "Upper Limit", 0, NULL, 0, UpdateJointProperties);
	currentJointUpperLimit->w = 200;
	
	
	// delete button
	currentLevelDeleteButton = gluiBlockProperties->add_button("Delete Level (3)", 0, DeleteCurrentLevel);
	
	currentLevelCopy = gluiBlockProperties->add_button("Copy Level", 0, CopyCurrentLevel);
	currentLevelPaste = gluiBlockProperties->add_button("Paste Level", 0, PasteCopiedLevel);
	currentLevelPaste->disable();
	


	
	
	///////////////////////////////////////////////////////////////////////////
	///
	/// Big Buttons
	///
	///////////////////////////////////////////////////////////////////////////
	glui->add_button("New World", 0, MakeNewWorld);
	glui->add_button("Test Physics", 0, TestPhysics);
	glui->add_button("Commit", 0, Commit);
	glui->add_button("Save", 0, SaveChanges);
	glui->add_button("Quit", 0,QuitClick);
	glui->set_main_gfx_window( mainWindow );
	
	


	
	
	UpdateSelectedLevel(0);
	UpdateInterfaceState();
	UpdateNewPolygonTexture(0);
	
	// Use a timer to control the frame rate.
	glutTimerFunc(framePeriod, Timer, 0);
	
	glutMainLoop();
	
	
	
    return 0;
}
