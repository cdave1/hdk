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
    
    glEnd();
    
    glPopMatrix();
}
