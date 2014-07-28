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

using namespace std;
#include "AppLevelController.h"

typedef struct surface_t
{
	hdPolygon *polygon;
	hdTexture *texture;
};


static int m_textureFrameBuffer = -1;

static hdTypedefList<surface_t, 128>* m_surfaces = NULL;

static hdTexture *m_floor, *m_roof, *m_wall;

AppLevelController::AppLevelController(const hdInterfaceController* parentController) : AppInterface(parentController)

{	m_camera.eye.Set(0,0,0);
	m_camera.center.Set(10,0,0);
	m_camera.up.Set(0,1,0);
	
	m_interactionState = e_interactionDisabled;
	
	/*
	m_currentLevel = NULL;
	m_testPoint.Set(0,0,0);
	m_lastRayScreenPoint.Set(0,0,0);
	m_rayQueryCount = 0;
	m_currBlock = NULL;
	 */
	m_bulletHitCount = 0;
	//m_currBlockTapDown = false;
	//m_currBlockTicksSinceTapDown = 1000;
	
	m_tapDown = false;
	
	m_pushedCameraMatrix = false;
	
	/*
	if ("Low" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY) ||
		"Medium" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
	{
		m_reflectParticles = false;
	}
	else
	{
		m_reflectParticles = true;
	}*/
	
	m_thumbPad = new hdThumbPad("Interface/mousePoint.png", "Interface/mousePoint.png",
								20.0f, 20.0f, 120.0f, 120.0f);
	
	m_trigger = new hdButton("Interface/mousePoint.png", "Interface/mousePoint.png", "Interface/mousePoint.png", NULL);
	m_trigger->SetAs2DBox(m_PixelScreenWidth - 90.0f, 20.0f, 70.0f, 70.0f);
	m_trigger->SetMouseDownListener(this, AppLevelController::btnTriggerDownCallback);
	m_trigger->SetMouseOverListener(this, AppLevelController::btnTriggerHoldCallback);
	m_trigger->SetMouseUpListener(this, AppLevelController::btnTriggerUpCallback);
	
	
	m_messageTagQueue = new hdTypedefList<hdMessage, 16>();
	//m_physicsParticleEmitter = NULL;
	
	m_minFrameDuration = strtof(hdConfig::GetValueForKey(CONFIG_MINFRAMEDURATION_KEY).c_str(), NULL);
	m_minFrameDuration = hdClamp(m_minFrameDuration, 0.016f, 0.05f);
	
	m_fixedPhysicsInterval = (true == strtol(hdConfig::GetValueForKey(CONFIG_FIXEDPHYSICSINTERVAL_KEY).c_str(), NULL, 0));
	
	m_physicsFixedFrameDuration = strtof(hdConfig::GetValueForKey(CONFIG_PHYSICSFIXEDFRAMEDURATION_KEY).c_str(), NULL);
	m_physicsFixedFrameDuration = hdClamp(m_physicsFixedFrameDuration, 0.016f, 0.05f);
	
	m_variablePhysicsRateMultiplier = strtof(hdConfig::GetValueForKey("VariablePhysicsRateMultiplier").c_str(), NULL);
	if (m_variablePhysicsRateMultiplier == 0.0f)
	{
		m_variablePhysicsRateMultiplier = 1.0f;
	}
	else
	{
		m_variablePhysicsRateMultiplier = hdClamp(m_variablePhysicsRateMultiplier, 0.2f, 10.0f);
	}
	
	m_physicsIterationsCount = strtol(hdConfig::GetValueForKey(CONFIG_PHYSICSITERATIONSCOUNT_KEY).c_str(), NULL, 0);
	m_physicsIterationsCount = hdClamp(m_physicsIterationsCount, 1, 20);
	
	
	m_floor = hdTextureManager::Instance()->FindTexture("Textures/fpsTest/t_floorpu01_256t.tga", TT_16Wall);
	m_roof = hdTextureManager::Instance()->FindTexture("Textures/fpsTest/t_mbase55a_14_256.tga", TT_16Wall);
	m_wall = hdTextureManager::Instance()->FindTexture("Textures/fpsTest/t_Concrete43_14_256.tga", TT_16Wall);
	
	m_surfaces = new hdTypedefList<surface_t, 128>();
	
	
	{
		surface_t p, p1, p2, p3, floor, roof;
		p.polygon = new hdPolygon(m_gameWorld);
		p.polygon->AddPoint(hdVec3(4, -4, -10));
		p.polygon->AddPoint(hdVec3(4, 4,  -10));
		p.polygon->AddPoint(hdVec3(4, 4,  20));
		p.polygon->AddPoint(hdVec3(4, -4, 20));
		p.polygon->ResetAABB();
		p.polygon->SetTextureRepeatX(1.0f);
		p.polygon->SetTextureRepeatY(1.0f);
		p.polygon->ResetTextureCoords2();
		p.texture = m_wall;
		m_surfaces->Add(p);
		
		p1.polygon = new hdPolygon(m_gameWorld);
		p1.polygon->AddPoint(hdVec3(4, -4, 20));
		p1.polygon->AddPoint(hdVec3(4, 4,  20));
		p1.polygon->AddPoint(hdVec3(-16, 4,  20));
		p1.polygon->AddPoint(hdVec3(-16, -4, 20));
		p1.polygon->ResetAABB();
		p1.polygon->SetTextureRepeatX(1.0f);
		p1.polygon->SetTextureRepeatY(1.0f);
		p1.polygon->ResetTextureCoords2();
		p1.texture = m_wall;
		m_surfaces->Add(p1);
		
		p2.polygon = new hdPolygon(m_gameWorld);
		p2.polygon->AddPoint(hdVec3(-16, -4, 20));
		p2.polygon->AddPoint(hdVec3(-16, 4,  20));
		p2.polygon->AddPoint(hdVec3(-16, 4,  -10));
		p2.polygon->AddPoint(hdVec3(-16, -4, -10));
		p2.polygon->ResetAABB();
		p2.polygon->SetTextureRepeatX(1.0f);
		p2.polygon->SetTextureRepeatY(1.0f);
		p2.polygon->ResetTextureCoords2();
		p2.texture = m_wall;
		m_surfaces->Add(p2);
		
		p3.polygon = new hdPolygon(m_gameWorld);
		p3.polygon->AddPoint(hdVec3(-16, -4, -10));
		p3.polygon->AddPoint(hdVec3(-16, 4,  -10));
		p3.polygon->AddPoint(hdVec3(4, 4,  -10));
		p3.polygon->AddPoint(hdVec3(4, -4, -10));
		p3.polygon->ResetAABB();
		p3.polygon->SetTextureRepeatX(1.0f);
		p3.polygon->SetTextureRepeatY(1.0f);
		p3.polygon->ResetTextureCoords2();
		p3.texture = m_wall;
		m_surfaces->Add(p3);
		
		floor.polygon = new hdPolygon(m_gameWorld);
		floor.polygon->AddPoint(hdVec3(4, -4, -10));
		floor.polygon->AddPoint(hdVec3(4, -4, 20));
		floor.polygon->AddPoint(hdVec3(-16, -4, 20));
		floor.polygon->AddPoint(hdVec3(-16, -4, -10));
		floor.polygon->SetTextureRepeatX(1.0f);
		floor.polygon->SetTextureRepeatY(1.0f);
		floor.polygon->ResetAABB();
		floor.polygon->ResetTextureCoords2();
		floor.texture = m_floor;
		m_surfaces->Add(floor);
		
		roof.polygon = new hdPolygon(m_gameWorld);
		roof.polygon->AddPoint(hdVec3(-16, 4, -10));
		roof.polygon->AddPoint(hdVec3(-16, 4, 20));
		roof.polygon->AddPoint(hdVec3(4, 4, 20));
		roof.polygon->AddPoint(hdVec3(4, 4, -10));
		roof.polygon->ResetAABB();
		roof.polygon->SetTextureRepeatX(1.0f);
		roof.polygon->SetTextureRepeatY(1.0f);
		roof.polygon->ResetTextureCoords2();
		roof.texture = m_roof;
		m_surfaces->Add(roof);
	}
	
	for (unsigned i = 0; i < m_surfaces->GetItemCount(); ++i)
	{
		m_surfaces->GetItems()[i].polygon->SetUserType(0x01);
	}
	
	this->InitPhysics();
	this->InitAnimations();
	this->InitSounds();
	this->InitEffects();
	
	this->RefreshLayout();
}



AppLevelController::~AppLevelController()
{
}


void AppLevelController::InitPhysics()
{
}


void AppLevelController::InitAnimations() {}

static hdSound *m_shotty = NULL;
void AppLevelController::InitSounds()
{
	m_shotty = hdSoundManager::FindSound("Sounds/shotty.caf", e_soundTypeFrequent);
}


void AppLevelController::InitEffects()
{
}


void AppLevelController::PhysicsStep(double interval)
{
	/*
	float proportion;
	
	if (m_currentLevel == NULL) return;
	if (m_physicsWorld == NULL) return;
	
	this->ResetContactPoints();
	
	// Paused 
	if (m_interactionState != e_interactionDisabled)
	{
		if (m_fixedPhysicsInterval)
		{
			proportion = hdClamp(interval, 0.016, 2.0 * m_physicsFixedFrameDuration);
			
			// Try it twice...
			m_physicsWorld->Step(0.5f * proportion * m_variablePhysicsRateMultiplier, //0.016, //m_physicsFixedFrameDuration, 
								 m_physicsIterationsCount, 
								 m_physicsIterationsCount);
			
			m_physicsWorld->Step(0.5f * proportion * m_variablePhysicsRateMultiplier, //0.016, //m_physicsFixedFrameDuration, 
								 m_physicsIterationsCount, 
								 m_physicsIterationsCount);
		}
		else
		{
			//
			printf("Unfixed physics interval no longer supported. \n");
			hdAssert(false);
			
			// proportion = (float)hdClamp(interval, 0.016, 0.04);
			// m_physicsWorld->Step(proportion * m_variablePhysicsRateMultiplier, 
			// m_physicsIterationsCount, 
			// m_physicsIterationsCount);
			 
		}
	}
*/
}



void AppLevelController::Step(double interval)
{
	
	PhysicsStep(interval);
	
	
	m_panMomentumEnabled = ((m_interactionState != e_panAndZoom) || !m_tapDown); //(m_interactionState == e_waitingForInput);
	if (hdFastHypotenuse(m_lastPanDirectionVector.x, m_lastPanDirectionVector.y) > 0.0f)
	{
		m_lastPanDirectionVector = hdClamp(m_lastPanDirectionVector - (0.11f * m_lastPanDirectionVector), 
										   hdVec2(-70.0f, -70.0f), hdVec2(70.0f, 70.0f));
		
		if (m_panMomentumEnabled)
		{
			this->PanProjection(m_lastPanDirectionVector.x, 
								m_lastPanDirectionVector.y, 
								0.0f, 0.0f);
		}
	}
	
	
	
	Game_Step(interval);
} 


static void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
					  GLfloat centerx, GLfloat centery, GLfloat centerz,
					  GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
	
    /* Make rotation matrix */
	
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
	
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
	
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
	
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
	
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
	
    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
	
    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
	
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        float fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (m[a]);
        glMultMatrixf(fixedM);
    }
	
    /* Translate Eye to Origin */
    glTranslatef((-eyex),
                 (-eyey),
                 (-eyez));
}


#define ROTATION_EXPERIMENT 0
void AppLevelController::PushCameraMatrix()
{
	hdAssert(!m_pushedCameraMatrix);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPushMatrix();
	
	hdMatrix matCam, mProjection;
	

	
	
#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	MatrixPerspectiveFovRH(mProjection, 90.0f*(hd_pi/180.0f), 1.0f/1.5f, 1.0f, 10000.0f, false);
#else
	MatrixPerspectiveFovRH(mProjection, 67.38f*(hd_pi/180.0f), 1.0f/m_ScreenAspectRatio, 1.0f, 10000.0f, false);
#endif
	glMultMatrixf(mProjection.f);
	
#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	if (m_orientation == e_hdInterfaceOrientationLandscape)
	{
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y, 
		//			 -((m_projection->GetAABB().upper.x - m_projection->GetAABB().lower.x)/2.0f));
	}
	else
	{
		//glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y, 
		//			 -((m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y)/2.0f));
	}
#else
	//glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y, 
	//			 -((m_projection->GetAABB().upper.x - m_projection->GetAABB().lower.x)/2.0f));
#endif
	
	//MatrixLookAtRH(hdMatrix &mOut, const hdVec3 &vEye, const hdVec3 &vAt, const hdVec3 &vUp);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	//MatrixLookAtRH(matCam, m_camera.eye, m_camera.center, m_camera.up);
	
	//glMultMatrixf(matCam.f);
	
	//glTranslatef(-m_camera.eye.x, -m_camera.eye.y, -m_camera.eye.z);
	
	gluLookAt(
			  m_camera.eye.x, m_camera.eye.y, m_camera.eye.z,
			  m_camera.center.x, m_camera.center.y, m_camera.center.z,
			  m_camera.up.x,m_camera.up.y, m_camera.up.z
			);
	
	
	m_pushedCameraMatrix = true;
}


void AppLevelController::PopCameraMatrix()
{
	hdAssert(m_pushedCameraMatrix);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	
	glPopMatrix();
	m_pushedCameraMatrix = false;
}


void AppLevelController::btnTriggerDownCallback(void *handler, void *sender)
{
	AppLevelController *self = (AppLevelController *)handler;
	self->HandleTriggerDown();
}


void AppLevelController::HandleTriggerDown()
{
	hdSoundManager::PlaySound(m_shotty);
	
	if (BulletRayTest())
	{
		hdPrintf("IMPACT!");
	}
}


void AppLevelController::btnTriggerHoldCallback(void *handler, void *sender)
{
	AppLevelController *self = (AppLevelController *)handler;
	self->HandleTriggerHold();
}


void AppLevelController::HandleTriggerHold()
{
	hdSoundManager::PlaySound(m_shotty);
	BulletRayTest();
}


void AppLevelController::btnTriggerUpCallback(void *handler, void *sender)
{
	AppLevelController *self = (AppLevelController *)handler;
	self->HandleTriggerUp();
}


void AppLevelController::HandleTriggerUp()
{
	hdPrintf("[TRIGGER UP]\n");
	BulletRayTest();
}



void AppLevelController::DrawInternal()
{
	PushCameraMatrix();
	
	hdglBindTexture(NULL);

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_FRONT);
	
	surface_t surf;
	float f[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	for (unsigned i = 0; i < m_surfaces->GetItemCount(); ++i)
	{
		surf = m_surfaces->GetItems()[i];
		
		DrawPolygonWithTint(surf.polygon, surf.texture, f, 1.0f);
	}
	glDisable(GL_TEXTURE_2D);
	
	if (m_bulletHitCount > 0)
	{
		hdglBegin(GL_LINES);
		hdglColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		hdglVertex3f(m_camera.eye.x - 1.0f, m_camera.eye.y - 1.0f, m_camera.eye.z);
		hdglVertex3f(m_bulletImpactPoint.x, m_bulletImpactPoint.y, m_bulletImpactPoint.z);
		hdglEnd();
		
		m_bulletHitCount = 0;
	}
	
	
	hdglBegin(GL_LINES);
	hdglColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	hdglVertex3f(0.0f, 0.0f, 0.0f);
	hdglVertex3f(10.0f, 0.0f, 0.0f);
	hdglEnd();
	
	hdglBegin(GL_LINES);
	hdglColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	hdglVertex3f(0.0f, 0.0f, 0.0f);
	hdglVertex3f(0.0f, 10.0f, 0.0f);
	hdglEnd();
	
	hdglBegin(GL_LINES);
	hdglColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	hdglVertex3f(0.0f, 0.0f, 0.0f);
	hdglVertex3f(0.0f, 0.0f, 10.0f);
	hdglEnd();
	
	
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	
	// particles
	
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	//glDisable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);
	PopCameraMatrix();
}


void AppLevelController::DrawInternalInterface()
{
	m_thumbPad->Draw();
	m_trigger->Draw();
	m_uiMessageQueueContainer->Draw();
	
	if (GetOrientation() == e_hdInterfaceOrientationLandscape)
	{
		m_uiLandscapeMessageContainer->Draw();
		m_uiLandscapeMessageButtonContainer->Draw();
	}
	else
	{
		m_uiPortraitMessageContainer->Draw();
		m_uiPortraitMessageButtonContainer->Draw();
	}
}


void AppLevelController::HandleTapUp(float x, float y, int tapCount)
{
	Game_HandleTapUp(x, y, tapCount);
}


void AppLevelController::HandleTapDown(float x, float y, int tapCount)
{
	Game_HandleTapDown(x, y, tapCount);
}


void AppLevelController::HandleSingleTap(float x, float y) 
{
	Game_HandleSingleTap(x, y);
}


void AppLevelController::HandleDoubleTap(float x, float y) 
{
	Game_HandleDoubleTap(x, y);
}


void AppLevelController::HandleTapMovedSingle(const float previousX, const float previousY, 
												const float currentX, const float currentY)
{
	//Game_HandleTapMovedSingle(previousX, previousY, currentX, currentY);
	hdVec2 previous, current;
	hdVec2 screenDiff, trans;
	float yRot;
	hdVec3 tmp;
	{
		if (m_thumbPad->IsMoving()) 
		{
			// - trans has direction and magnitude
			// - direction to move in: angle of pad relative to current
			//   eye->center vector.
			trans = m_thumbPad->GetPadDirection();
			
			tmp.Set(-trans.x, 0, trans.y);
			
			hdPrintf("[THUMB PAD: %3.3f, %3.3f]\n", trans.x, trans.y);
			
			// y rotation:
			yRot = hdGetAngle(hdVec2(m_camera.center.x, m_camera.center.z), 
					   hdVec2(m_camera.eye.x, m_camera.eye.z + 10.0f), 
					   hdVec2(m_camera.eye.x, m_camera.eye.z));
			
			hdRotateVertices(&tmp, 1, hdVec3(0, yRot, 0));
			
			hdTranslateVertices(&m_camera.eye, 1, tmp);
			hdTranslateVertices(&m_camera.center, 1, tmp);
		}
		else
		{
			ConvertRawToScreen(previous, previousY, previousX);
			ConvertRawToScreen(current, currentY, currentX);
			
			// Screen distance travelled.
			screenDiff = previous - current;
			
			// no movement - we only want to stop if the user
			// has stopped moving, but not removed, their finger.
			if (screenDiff.x == 0 && screenDiff.y == 0) 
			{
				return;
			}
			
			// Rotate camera center by screenDiff.
			hdRotateVertices(&m_camera.center, 1, hdVec3(screenDiff.x * (0.25 * (hd_pi/90.0f)), screenDiff.y * (0.25 * (hd_pi/90.0f)), 0));
		}
	}
}


void AppLevelController::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
												const float aCurrentX, const float aCurrentY,
												const float bPreviousX, const float bPreviousY,	
												const float bCurrentX, const float bCurrentY)
{
	this->ZoomProjection(aPreviousX, aPreviousY, aCurrentX, aCurrentY, 
						 bPreviousX, bPreviousY, bCurrentX, bCurrentY);
}


void AppLevelController::HandleOrientationChanged()
{
	this->RefreshLayout();
	Game_HandleOrientationChanged();
	PanProjection(0, 0, 0.1f, 0.1f);
}


bool AppLevelController::BulletRayTest()
{
	hdVec3 ray[2];
	const int k_maxCount = 1;
	hdGameObject* objs[k_maxCount];
	
	//this->ConvertScreenToInterface(pp, x, y);
	ray[0].Set(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z);
	ray[1].Set(m_camera.center.x, m_camera.center.y, m_camera.center.z);
	
	m_bulletHitCount = m_gameWorld->RayQuery___2((unsigned int)0x01, 
												ray, objs, k_maxCount, m_bulletImpactPoint);

	return m_bulletHitCount > 0;
}


void AppLevelController::Game_Step(double interval) {}


const bool AppLevelController::Game_HandleTapUp(float x, float y, int tapCount)
{
	return false;
}


const bool AppLevelController::Game_HandleTapDown(float x, float y, int tapCount)
{
	return false;
}


const bool AppLevelController::Game_HandleSingleTap(float x, float y)
{
	return false;
}


const bool AppLevelController::Game_HandleDoubleTap(float x, float y)
{
	return false;
}


const bool AppLevelController::Game_HandleTapMovedSingle(const float previousX, const float previousY, 
														   const float currentX, const float currentY)
{
	return false;
}


const bool AppLevelController::Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
														   const float aCurrentX, const float aCurrentY,
														   const float bPreviousX, const float bPreviousY,	
														   const float bCurrentX, const float bCurrentY)
{
	return false;
}void AppLevelController::Game_HandleOrientationChanged() {}
