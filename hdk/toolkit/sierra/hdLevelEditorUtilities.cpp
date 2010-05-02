/*
 *  hdLevelEditorUtilities.cpp
 *  LevelEditor
 *
 *  Created by david on 31/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */


#include "hdLevelEditorUtilities.h"

void hdDrawGameObjectHandles(hdGameObject* gameObject)
{
	if (gameObject == NULL) return;
	
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.0f, 0.0f, 0.0f);
	
	glPushMatrix();
		
	glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().lower.y, gameObject->GetAABB().lower.z);
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().upper.y, gameObject->GetAABB().lower.z);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().upper.y, gameObject->GetAABB().lower.z);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().lower.y, gameObject->GetAABB().lower.z);
	glVertex3f(gameObject->GetWorldCenter().x, gameObject->GetWorldCenter().y, gameObject->GetAABB().lower.z);
	
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().lower.y, 0.0f);
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().upper.y,0.0f);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().upper.y, 0.0f);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().lower.y, 0.0f);
	glVertex3f(gameObject->GetWorldCenter().x, gameObject->GetWorldCenter().y, 0.0f);
	
	
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().lower.y, gameObject->GetAABB().upper.z);
	glVertex3f(gameObject->GetAABB().lower.x, gameObject->GetAABB().upper.y, gameObject->GetAABB().upper.z);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().upper.y, gameObject->GetAABB().upper.z);
	glVertex3f(gameObject->GetAABB().upper.x, gameObject->GetAABB().lower.y, gameObject->GetAABB().upper.z);
	glVertex3f(gameObject->GetWorldCenter().x, gameObject->GetWorldCenter().y, gameObject->GetAABB().upper.z);
	
	//glVertex2f(m_currentMouseDragPoint.x, m_currentMouseDragPoint.y);
	glEnd();
	
	glPopMatrix();
}
