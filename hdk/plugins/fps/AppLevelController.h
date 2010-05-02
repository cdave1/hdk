/*
 *  AppLevelController.h
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

/*
 *
 * Controller for:
 * - Levels - which one player is on.
 * - Currently selected world.
 * - Level picker menu.
 * - 
 *
 */



#ifndef APP_LEVEL_CONTROLLER_H
#define APP_LEVEL_CONTROLLER_H


#include "hdk.h"

#include "AppInterface.h"

#include "hdGraphicsLibrary/hdCamera.h"
#include "hdInterface/hdThumbPad.h"


#define kGravityMultiplier 10.0f

class AppLevelController : public AppInterface
{
public:	
	AppLevelController(const hdInterfaceController* parentController);
	
	~AppLevelController();
	
	void Step(double interval);
	
	
	void HandleTapUp(float x, float y, int tapCount);
	
	void HandleTapDown(float x, float y, int tapCount);
	
	void HandleSingleTap(const float x, const float y);
	
	void HandleDoubleTap(float x, float y);
	
	void HandleTapMovedSingle(const float previousX, const float previousY, 
							  const float currentX, const float currentY);
	
	void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
							  const float aCurrentX, const float aCurrentY,
							  const float bPreviousX, const float bPreviousY,
							  const float bCurrentX, const float bCurrentY);
	
	void HandleOrientationVectorUpdate(const float x, const float y, const float z) {}
	
	void HandleOrientationChanged();
	
	
protected:
	void DrawInternal();
	
	void PushCameraMatrix();
	
	void PopCameraMatrix();
	
	void DrawInternalInterface();
	
	
	static void btnTriggerDownCallback(void *handler, void *sender);
	void HandleTriggerDown();
	
	static void btnTriggerHoldCallback(void *handler, void *sender);
	void HandleTriggerHold();
	
	static void btnTriggerUpCallback(void *handler, void *sender);
	void HandleTriggerUp();
	
	
	bool BulletRayTest();
	
	
	
	/**********************************************************************/
	/* Virtual Functions - all optional */
	/**********************************************************************/
	virtual void Game_Step(double interval);
	
	virtual const bool Game_HandleTapUp(float x, float y, int tapCount);
	
	virtual const bool Game_HandleTapDown(float x, float y, int tapCount);
	
	virtual const bool Game_HandleSingleTap(float x, float y);
	
	virtual const bool Game_HandleDoubleTap(float x, float y);
	
	virtual const bool Game_HandleTapMovedSingle(const float previousX, const float previousY, 
												 const float currentX, const float currentY);
	
	virtual const bool Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
												 const float aCurrentX, const float aCurrentY,
												 const float bPreviousX, const float bPreviousY,	
												 const float bCurrentX, const float bCurrentY);
	
	virtual void Game_HandleOrientationChanged();
	
	
private:
	
	void InitPhysics();
	
	void InitEffects();
	
	void InitAnimations();
	
	void InitSounds();
	
	
	
	
	void PhysicsStep(double interval);

	/**********************************************************************/
	/* Camera/Thumbpad */
	/**********************************************************************/ 	
	hdCamera m_camera;
	
	bool m_pushedCameraMatrix;
	
	hdThumbPad *m_thumbPad;
	
	hdButton *m_trigger;
	
	int m_bulletHitCount;
	
	hdVec3 m_bulletImpactPoint;
	
	
	
	
	/**********************************************************************/
	/* Sound */
	/**********************************************************************/
	
	
	
	/**********************************************************************/
	/* Misc */
	/**********************************************************************/
	float m_minFrameDuration;
	
	float m_physicsFixedFrameDuration;
	
	bool m_fixedPhysicsInterval;
	
	float m_variablePhysicsRateMultiplier;
	
	int m_physicsIterationsCount;
	
	
	int m_rayQueryCount;
	
	bool m_tapDown;
};


#endif