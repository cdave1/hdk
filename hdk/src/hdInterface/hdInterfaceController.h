/*
 *  hdInterfaceController.h
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_INTERFACE_CONTROLLER_H_
#define _HD_INTERFACE_CONTROLLER_H_

#include "hdCore.h"
#include "hdGraphicsLibrary.h"
#include "hdAnimationEngine.h"

#include "hdInterface/hdInterfaceEnums.h"
#include "hdInterface/hdInterfaceContext.h"

class hdInterfaceController 
{
public:	
	hdInterfaceController(); 
	
	hdInterfaceController(const hdInterfaceController* parentController);

	hdInterfaceController(const e_hdInterfaceOrientation orientation, 
						  const hdInterfaceController* parentController);
	
	hdInterfaceController(const float screenWidth, 
						  const float screenHeight, 
						  const float screenDepth);
		
	~hdInterfaceController();
	
	virtual void Step(double interval) = 0;
		
	virtual void Draw() = 0;
	
	virtual void AnimateShow();
	
	virtual void HandleTapUp(float x, float y, int tapCount);
	
	virtual void HandleTapDown(float x, float y, int tapCount);
	
	virtual void HandleSingleTap(const float x, const float y);
	
	virtual void HandleDoubleTap(float x, float y);
	
	virtual void HandleTapMovedSingle(const float previousX, const float previousY, 
									  const float currentX, const float currentY);
	
	virtual void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
									  const float aCurrentX, const float aCurrentY,
									  const float bPreviousX, const float bPreviousY,	
									  const float bCurrentX, const float bCurrentY);
	
	virtual void HandleResize(const float newWidth, const float newHeight);
	
	const e_hdInterfaceOrientation GetOrientation() const;
	
	const bool IsLandscapeOrientation() const;
	
	void SetOrientation(const e_hdInterfaceOrientation orientation);
	
	void ResetOrientation();
	
	//virtual void AnimateHide() = 0;
	
	const hdAABB GetWorldAABB() const;
	
	void SetWorldAABB(const hdAABB& aabb);
	
	void SetParentController(const hdInterfaceController* parentController);
	
protected:
	friend class hdInterfaceContext;

	void ResetGameWorld();
	
	//virtual void FinishedController(hdInterfaceController * controller);
	
	void ConvertScreenToInterface(hdVec2& vOut, float x, float y);
	
	void ConvertInterfaceToScreen(hdVec2& vOut, float x, float y);
	
	void ConvertRawToScreen(hdVec2& vOut, float x, float y);
	
	void TranslateProjection(const hdVec3& trans);
	
	void PanProjection(const float aPreviousX, const float aPreviousY,
					   const float aCurrentX, const float aCurrentY);
	
	void ZoomProjection(const float aPreviousX, const float aPreviousY,
						const float aCurrentX, const float aCurrentY,
						const float bPreviousX, const float bPreviousY,	
						const float bCurrentX, const float bCurrentY);
	
	
	void CentreAndZoomProjection(const hdVec3& center, const hdVec3& zoomDim, const float duration);
	
	void CenterAndZoomProjectionStep(hdTimeInterval interval);
	
	void FollowGameObject(const hdGameObject *object, const float duration);
	
	void FollowGameObjectStep(hdTimeInterval interval);
	
	
	
	// gameworld stuff
	hdInterfaceController* m_parentController;
	
	hdGameWorld *m_gameWorld;
	
	float m_screenHeight;
	
	float m_screenWidth;
	
	float m_screenDepth;
	
	e_hdInteractionState m_interactionState;
	
	float m_PixelScreenWidth;
	float m_PixelScreenHeight;
	float m_ScreenAspectRatio;
	
	float m_GameMinScreenWidth;	
	float m_GameMinScreenHeight;
	float m_GameMaxScreenWidth;	
	float m_GameMaxScreenHeight;
	
	float m_pixelsPerGameUnit;
	float m_lowerZoomRatioX;
	float m_lowerZoomRatioY;
	float m_upperZoomRatioX;
	float m_upperZoomRatioY;
	
	float m_landscapeRotationZValue;
	
	e_hdInterfaceOrientation m_orientation;
	
	hdAABB m_worldAABB;

	hdOrthographicProjection *m_projection;
	
	bool m_panMomentumEnabled;
	
	hdVec2 m_lastPanDirectionVector;

private:
	void Init(const e_hdInterfaceOrientation orientation, const float screenWidth, const float screenHeight, const float screenDepth);
	
	// Camera centre zooms to the game object position, and then follows it around.
	hdAnimation *m_followObjectAnimation;
	//static hdFunctionAction<hdInterfaceController> *m_followObjectAction = NULL;
	hdGameObject* m_followingObject;
	
	hdAnimation *m_centerZoomAnimation;
	hdFunctionAction<hdInterfaceController> *m_centerZoomAction;
	float m_duration;
	float m_fracn;
	
	hdAABB m_startingAABB;
	hdVec3 m_moveVector;
	
	int16 m_momentumCancelCountdown;
};


inline void hdInterfaceController::SetParentController(const hdInterfaceController* parentController)
{
	m_parentController = (hdInterfaceController*)parentController;
}


inline const hdAABB hdInterfaceController::GetWorldAABB() const
{
	return m_gameWorld->GetAABB();
}


inline void hdInterfaceController::SetWorldAABB(const hdAABB& aabb)
{
	assert(m_gameWorld != NULL);
	m_gameWorld->SetAABB(aabb);
	
	
}


inline const e_hdInterfaceOrientation hdInterfaceController::GetOrientation() const
{
	return m_orientation;
}


inline const bool hdInterfaceController::IsLandscapeOrientation() const
{
	return (m_orientation == e_hdInterfaceOrientationLandscapeLeft ||
			m_orientation == e_hdInterfaceOrientationLandscapeRight);
}



#endif