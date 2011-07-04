/*
 *  hdInterfaceController.cpp
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdInterfaceController.h"

#define kDefaultScreenDepth 50.0f


hdInterfaceController::hdInterfaceController()
{
	m_parentController = NULL;
	float width, height;
	ScreenSettings_GetScreenRect(&width, &height);
	this->Init(e_hdInterfaceOrientationPortrait, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdInterfaceController::hdInterfaceController(const hdInterfaceController* parentController)
{
	m_parentController = (hdInterfaceController *)parentController;
	float width, height;
	ScreenSettings_GetScreenRect(&width, &height);
	this->Init(e_hdInterfaceOrientationPortrait, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdInterfaceController::hdInterfaceController(const e_hdInterfaceOrientation orientation, const hdInterfaceController* parentController)
{
	m_parentController = (hdInterfaceController *)parentController;
	float width, height;
	ScreenSettings_GetScreenRect(&width, &height);
	this->Init(orientation, 
			   width / 2.0f, 
			   height / 2.0f, 
			   kDefaultScreenDepth);
}


hdInterfaceController::hdInterfaceController(const float screenWidth, const float screenHeight, const float screenDepth)
{
	m_parentController = NULL;
	this->Init(e_hdInterfaceOrientationPortrait, screenWidth, screenHeight, screenDepth);
}


void hdInterfaceController::Init(const e_hdInterfaceOrientation orientation, const float screenWidth, const float screenHeight, const float screenDepth)
{
	hdAABB worldAABB;
	hdAABB projectionAABB;
	
	m_projection = NULL;
	
	m_momentumCancelCountdown = -1;
	
	worldAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	worldAABB.upper = hdVec3(m_screenWidth, m_screenHeight, -m_screenDepth);
	m_gameWorld = new hdGameWorld(worldAABB);
	
	SetOrientation(e_hdInterfaceOrientationLandscapeLeft);

	m_followObjectAnimation = NULL;
	
	m_followingObject = NULL;
	m_centerZoomAnimation = NULL;
	m_centerZoomAction = NULL;
	m_duration = 0.0f;
	m_fracn = 0.0f;
	
	m_moveVector.Set(0,0,0);
	
	m_panMomentumEnabled = false;
	m_lastPanDirectionVector.Set(0.0f, 0.0f);
}


hdInterfaceController::~hdInterfaceController()
{
	hdAnimationController::Instance()->StopAnimations(this);
	hdAnimationController::Instance()->PurgeAnimations(this);
	
	if (m_projection != NULL)
	{
		delete m_projection;
	}
	
	if (m_gameWorld != NULL)
	{
		delete m_gameWorld;
	}
}


void hdInterfaceController::ResetGameWorld()
{
	delete m_gameWorld;
	hdAABB worldAABB;
	worldAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	worldAABB.upper = hdVec3(m_screenWidth, m_screenHeight, -m_screenDepth);
	m_gameWorld = new hdGameWorld(worldAABB);
}


/*
 * Call this to kick off an animation that centres the projection on the given point,
 * at the given zoom level, over a specific duration.
 *
 * If duration 0, effect is instant.
 *
 * If damping, the animation effect will "slow" as it gets within a certain threshold distance
 * of the destination (without curves to define this stuff, it may look faked).
 */
void hdInterfaceController::CentreAndZoomProjection(const hdVec3& dest, const hdVec3& zoomDim, const float duration)
{
	hdVec3 lo = m_projection->GetAABB().lower;
	hdVec3 hi = m_projection->GetAABB().upper;
	m_startingAABB = m_projection->GetAABB();
	
	m_moveVector = dest - m_projection->GetWorldCenter();
	
	if (duration <= 0.0f)
	{
		lo -= m_moveVector;
		hi -= m_moveVector;
	
		m_projection->SetAABB(lo, hi);
	} 
	else
	{
		if (m_centerZoomAnimation == NULL)
		{
			m_centerZoomAnimation = hdAnimationController::CreateAnimation(this, false);
		}
		
		if (m_centerZoomAction == NULL)
		{
			m_centerZoomAction = new hdFunctionAction<hdInterfaceController>();
		}
		m_fracn = 0.0f;
		
		// Function action calls the step function below until times out.
		m_centerZoomAction->SetDuration(m_duration);
		m_centerZoomAction->SetFunctionObject(this);
		m_centerZoomAction->SetFunction(&hdInterfaceController::CenterAndZoomProjectionStep);
				
		m_centerZoomAnimation->AddGameObject(m_projection);
		m_centerZoomAnimation->AddAction(m_centerZoomAction);
		m_centerZoomAnimation->StartAnimation();
	}
}


void hdInterfaceController::CenterAndZoomProjectionStep(hdTimeInterval interval)
{
	// Formula: 
	//
	// n_0 = 0
	// n = n_-1 + ((1 - elapsedPercent) * factor)
	
	hdVec3 lo = m_startingAABB.lower;
	hdVec3 hi = m_startingAABB.upper;
	
	m_fracn += (1.0f - (m_centerZoomAction->GetProgress() / m_centerZoomAction->GetDuration())) * (0.2f); // * interval);
	
	hdVec3 diffCenter = m_fracn * m_moveVector;
		
	lo += diffCenter;
	hi += diffCenter;

	m_projection->SetAABB(lo, hi);
}





void hdInterfaceController::FollowGameObject(const hdGameObject *object, const float duration)
{
	if (object == NULL) return;
	
	m_followingObject = (hdGameObject *)object;
	
	if (m_followObjectAnimation == NULL)
	{
		m_followObjectAnimation = hdAnimationController::CreateAnimation(this, false);
	} 
	else
	{
		m_followObjectAnimation->StopAnimation(false, false);
	}
	
	//if (m_followObjectAction == NULL)
	//{
		
	//}
	//fracn = 0.0f;
	
	m_followObjectAnimation->RemoveAllGameObjects();
	m_followObjectAnimation->RemoveAllActions();
	
	hdFunctionAction<hdInterfaceController> *followObjectAction = new hdFunctionAction<hdInterfaceController>();
	
	// Function action calls the step function below until times out.
	followObjectAction->SetDuration(duration);
	followObjectAction->SetFunctionObject(this);
	followObjectAction->SetFunction(&hdInterfaceController::FollowGameObjectStep);
	
	m_followObjectAnimation->AddGameObject(m_followingObject);
	m_followObjectAnimation->AddAction(followObjectAction);
	m_followObjectAnimation->StartAnimation();
}


void hdInterfaceController::FollowGameObjectStep(hdTimeInterval interval)
{
	if (m_followingObject == NULL) return;

	ZoomProjection( 0.0f, 0.0f,
				   0.5f, 0.75f,
				    0.0f, 0.0f,
				   -0.75f, -0.75f);
}


void hdInterfaceController::TranslateProjection(const hdVec3& trans)
{
	hdVec3 lo, hi, box;
	
	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	lo += trans;
	hi += trans;
	
	lo.x = hdClamp(lo.x, m_worldAABB.lower.x, (m_worldAABB.upper.x - box.x));
	lo.y = hdClamp(lo.y, m_worldAABB.lower.y, (m_worldAABB.upper.y - box.y));
	
	hi.x = hdClamp(hi.x, m_worldAABB.lower.x + box.x, m_worldAABB.upper.x);
	hi.y = hdClamp(hi.y, m_worldAABB.lower.y + box.y, m_worldAABB.upper.y);
	
	m_projection->SetAABB(lo, hi);
}


void hdInterfaceController::PanProjection(const float aPreviousX, const float aPreviousY,
										const float aCurrentX, const float aCurrentY)
{
	hdVec2 previous, current;
	hdVec3 lo, hi, box, diff;
	hdVec2 screenDiff;

	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	ConvertRawToScreen(previous, aPreviousX, aPreviousY);
	ConvertRawToScreen(current, aCurrentX, aCurrentY);
	
	// Screen distance travelled.
	screenDiff = previous - current;
	
	// no movement - we only want to stop if the user
	// has stopped moving, but not removed, their finger.
	if (screenDiff.x == 0 && screenDiff.y == 0) 
	{	
		
		if (m_momentumCancelCountdown == -1)
		{
			m_momentumCancelCountdown = 10;
		}
		else
		{
			--m_momentumCancelCountdown;
			
			if (m_momentumCancelCountdown == 0)
			{
				m_lastPanDirectionVector.Set(0, 0);
				m_momentumCancelCountdown = -1;
			}
		}
		
		//if (hdFastHypotenuse(m_lastPanDirectionVector.x, m_lastPanDirectionVector.y) < 0.001f)
		//{
		//	m_lastPanDirectionVector.Set(0, 0);
		//}
		return;
	}	
	m_momentumCancelCountdown = -1;
	
	// Game units per pixel to find the actual quantity of game units used.
	float unitsPerPixelX = box.x / m_PixelScreenWidth;
	float unitsPerPixelY = box.y / m_PixelScreenHeight;
	
	diff.Set(screenDiff.x * unitsPerPixelX, screenDiff.y * unitsPerPixelY, 0.0f);
	
	lo += diff;
	hi += diff;
	
	// Clamp lo and hi to the boundaries of the world.
	lo.x = hdClamp(lo.x, m_worldAABB.lower.x, (m_worldAABB.upper.x - box.x));
	lo.y = hdClamp(lo.y, m_worldAABB.lower.y, (m_worldAABB.upper.y - box.y));
	
	hi.x = hdClamp(hi.x, m_worldAABB.lower.x + box.x, m_worldAABB.upper.x);
	hi.y = hdClamp(hi.y, m_worldAABB.lower.y + box.y, m_worldAABB.upper.y);
	
	m_projection->SetAABB(lo, hi);
	
	m_lastPanDirectionVector.Set(aPreviousX - aCurrentX, aPreviousY - aCurrentY);
}



// HACK: why is there so much code here? surely this should be simpler...
void hdInterfaceController::ZoomProjection(const float aPreviousX, const float aPreviousY,
										 const float aCurrentX, const float aCurrentY,
										 const float bPreviousX, const float bPreviousY,	
										 const float bCurrentX, const float bCurrentY)
{
	float prevDist, currDist, unitsPerPixelX;
	hdVec3 lo, hi, box;
	
	lo = m_projection->GetAABB().lower;
	hi = m_projection->GetAABB().upper;
	box = hi - lo;
	
	unitsPerPixelX = box.x / m_PixelScreenWidth;
	
	// Just compare the hypotenuse of previous with current.
	prevDist = hdFastHypotenuse(aPreviousX - bPreviousX, aPreviousY - bPreviousY);
	currDist = hdFastHypotenuse(aCurrentX - bCurrentX, aCurrentY - bCurrentY);
	
	if (prevDist > currDist)
	{
		// Can't get any bigger, so bail out.
		if (box.x >= m_GameMaxScreenWidth) return;
		
		// f is the absolute zoom distance in game area units
		float f = (prevDist - currDist)* unitsPerPixelX;
		
		// Keep the projection centered on the middle of the screen
		lo -= hdVec3(f * m_lowerZoomRatioX, f * m_lowerZoomRatioY, 0.0f);
		hi += hdVec3(f * m_upperZoomRatioX, f * m_upperZoomRatioY, 0.0f);
	} 
	else if (prevDist < currDist)
	{
		// Can't get any smaller, so bail out.
		if (box.x <= m_GameMinScreenWidth) return;
		
		float f = (currDist - prevDist) * unitsPerPixelX;
		
		lo += hdVec3(f * m_lowerZoomRatioX, f * m_lowerZoomRatioY, 0.0f);
		hi -= hdVec3(f * m_upperZoomRatioX, f * m_upperZoomRatioY, 0.0f);
	} 
	
	// Clamp lo and hi to the boundaries of the world.
	lo.x = hdClamp(lo.x, m_worldAABB.lower.x, (m_worldAABB.upper.x - box.x));
	lo.y = hdClamp(lo.y, m_worldAABB.lower.y, (m_worldAABB.upper.y - box.y));
	
	hi.x = hdClamp(hi.x, m_worldAABB.lower.x + box.x, m_worldAABB.upper.x);
	hi.y = hdClamp(hi.y, m_worldAABB.lower.y + box.y, m_worldAABB.upper.y);
	
	// ensure the zoom aabb does not get smaller or greater than
	// the min/max projection size.
	if ((hi.x - lo.x) <= m_GameMinScreenWidth)
	{
		hi.x = lo.x + m_GameMinScreenWidth;
		hi.y = lo.y + m_GameMinScreenHeight;
	}
	
	if ((hi.x - lo.x) >= m_GameMaxScreenWidth)
	{
		hi.x = lo.x + m_GameMaxScreenWidth;
		hi.y = lo.y + m_GameMaxScreenHeight;
	}
	
	// ensure the aabb is the same ratio as the screen
	if ((hi.x - lo.x) / (hi.y - lo.y) != m_ScreenAspectRatio)
	{
		// Just set the bb upper value
//		hi.y = lo.y + (hi.x - lo.x) * m_ScreenAspectRatio;
        hi.x = lo.x + (hi.y - lo.y) * m_ScreenAspectRatio;
	}
	
	m_projection->SetAABB(lo, hi);
}


/*
 * NOTE: Will not work properly with Raw input coordinates - call
 * ConvertRawToScreen on such coordinates before calling this function.
 *
 * TODO: look for a way to enforce this rule.
 */
void hdInterfaceController::ConvertScreenToInterface(hdVec2& vOut, float x, float y)
{
	hdVec3 projectionBoundingBox = m_projection->GetAABB().upper - m_projection->GetAABB().lower;
	hdVec3 projectionPosition = m_projection->GetAABB().lower;
	
	float unitsPerPixelX = projectionBoundingBox.x / m_PixelScreenWidth;
	float unitsPerPixelY = projectionBoundingBox.y / m_PixelScreenHeight;
	
	vOut.Set((x * unitsPerPixelX) + projectionPosition.x,
			 (y * unitsPerPixelY) + projectionPosition.y);
}


void hdInterfaceController::ConvertInterfaceToScreen(hdVec2& vOut, float x, float y)
{
	hdVec3 projectionBoundingBox = m_projection->GetAABB().upper - m_projection->GetAABB().lower;
	hdVec3 projectionPosition = m_projection->GetAABB().lower;
	
	float pixelsPerUnitX = m_PixelScreenWidth / projectionBoundingBox.x;
	float pixelsPerUnitY = m_PixelScreenHeight / projectionBoundingBox.y;
	
	vOut.Set(pixelsPerUnitX * (x - projectionPosition.x), 
				 pixelsPerUnitY * (y - projectionPosition.y));
}


/*
 * HACK HACK HACK - I don't have a good understanding of how the iPhone device uses
 * the orientation to determine raw screen coordinates.
 * 
 * PixelScreenWidth and PixelScreenHeight are also device specific.
 */
void hdInterfaceController::ConvertRawToScreen(hdVec2& vOut, float x, float y)
{
#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
	vOut.Set(x, m_PixelScreenHeight - y);
#else
	vOut.Set(x, y);
#endif
}



void hdInterfaceController::HandleResize(const float newWidth, const float newHeight)
{
	hdAABB boxAABB, projectionAABB;
	
	m_PixelScreenWidth = newWidth;
	m_PixelScreenHeight = newHeight;
	m_ScreenAspectRatio = newHeight / newWidth;
	
	m_GameMinScreenWidth = newWidth / m_pixelsPerGameUnit; 
	m_GameMinScreenHeight = newHeight / m_pixelsPerGameUnit;
	m_GameMaxScreenWidth = 4 * m_GameMinScreenWidth; 
	m_GameMaxScreenHeight = 4 * m_GameMaxScreenHeight; 
	
	m_screenWidth = newWidth / 100.0f;
	m_screenHeight = newHeight / 100.0f;
	m_screenDepth = kDefaultScreenDepth;
	
	hdVec3 center = m_projection->GetWorldCenter();
	
	boxAABB.lower = hdVec3(0,0,0);
	boxAABB.upper = hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
	
	projectionAABB.lower = center - hdVec3(boxAABB.Width(), boxAABB.Height(), -m_screenDepth);
	projectionAABB.upper = center + hdVec3(boxAABB.Width(), boxAABB.Height(), m_screenDepth);
	
	m_projection->SetAABB(projectionAABB.lower, 
						  projectionAABB.upper);
}


void hdInterfaceController::ResetOrientation()
{
	this->SetOrientation(m_orientation);
}


void hdInterfaceController::SetOrientation(const e_hdInterfaceOrientation orientation)
{
	hdAABB projectionAABB;
    ScreenSettings_SetLandscape();
    /*
	e_hdInterfaceOrientation oldOrientation = m_orientation;

	if (orientation == e_hdInterfaceOrientationPortrait ||
		orientation == e_hdInterfaceOrientationPortraitUpsideDown)
	{
		ScreenSettings_SetPortrait();
	}
	else if (orientation == e_hdInterfaceOrientationLandscapeLeft ||
			 orientation == e_hdInterfaceOrientationLandscapeRight)
	{
		ScreenSettings_SetLandscape();
	}*/
    m_orientation = orientation;
	
	ScreenSettings_GetAspectRatio(&m_ScreenAspectRatio);
	ScreenSettings_GetScreenRect(&m_PixelScreenWidth, &m_PixelScreenHeight);
	ScreenSettings_GetMinCameraRect(&m_GameMinScreenWidth, &m_GameMinScreenHeight);
	ScreenSettings_GetMaxCameraRect(&m_GameMaxScreenWidth, &m_GameMaxScreenHeight);
	ScreenSettings_GetZoomRatio(&m_lowerZoomRatioX, &m_lowerZoomRatioY,
								&m_upperZoomRatioX, &m_upperZoomRatioY);
	ScreenSettings_GetPixelsPerGameUnit(&m_pixelsPerGameUnit);

	
	if (m_projection == NULL)
	{
		ScreenSettings_GetDefaultCameraRect(&m_screenWidth, &m_screenHeight);
		m_screenDepth = kDefaultScreenDepth;

		projectionAABB.lower = hdVec3(0,0,0) - hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
		projectionAABB.upper = hdVec3(m_screenWidth, m_screenHeight, m_screenDepth);
		m_projection = new hdOrthographicProjection(m_gameWorld, projectionAABB);
	}
	m_landscapeRotationZValue = 0.0f;
}



void hdInterfaceController::AnimateShow() {}

void hdInterfaceController::HandleTapUp(float x, float y, int tapCount) {}

void hdInterfaceController::HandleTapDown(float x, float y, int tapCount) {}

void hdInterfaceController::HandleSingleTap(const float x, const float y) {}

void hdInterfaceController::HandleDoubleTap(float x, float y) {}

void hdInterfaceController::HandleTapMovedSingle(const float previousX, const float previousY, 
											   const float currentX, const float currentY) {}

void hdInterfaceController::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
											   const float aCurrentX, const float aCurrentY,
											   const float bPreviousX, const float bPreviousY,	
											   const float bCurrentX, const float bCurrentY) {}