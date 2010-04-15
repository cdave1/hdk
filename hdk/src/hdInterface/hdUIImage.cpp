/*
 *  hdUIImage.cpp
 *  TotemGame
 *
 *  Created by david on 27/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdUIImage.h"

hdUIImage::hdUIImage(const char* texture, 
					 hdGameWorld* gameWorld) : hdReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(hdVec3(0, 0, 0), hdVec3(1.0f, 1.0f, 1.0f));
	
	m_tint = new float[4];
	m_tint[0] = 1.0f;
	m_tint[1] = 1.0f;
	m_tint[2] = 1.0f;
	m_tint[3] = 1.0f;
}


hdUIImage::hdUIImage(const char* texture, 
				   hdGameWorld* gameWorld, 
				   const hdAABB& aabb) : hdReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(aabb.lower, aabb.upper);
	
	m_tint = new float[4];
	m_tint[0] = 1.0f;
	m_tint[1] = 1.0f;
	m_tint[2] = 1.0f;
	m_tint[3] = 1.0f;
}


hdUIImage::hdUIImage(const char* texture,
				   hdGameWorld* gameWorld, 
				   const float x, const float y, 
				   const float w, const float h) : hdReceiver(gameWorld)
{
	m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
	this->SetAs2DBox(hdVec3(x,y,0.0f), hdVec3(x+w,y+h,0.0f));
	
	m_tint = new float[4];
	m_tint[0] = 1.0f;
	m_tint[1] = 1.0f;
	m_tint[2] = 1.0f;
	m_tint[3] = 1.0f;
}


hdUIImage::~hdUIImage() 
{
	delete [] m_tint;
}


bool hdUIImage::MouseDown(float x, float y)
{	
	if (this->IsHidden()) return false;
	return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


// If we did not click on the item, then state can not go into mouse over.
bool hdUIImage::MouseOver(float x, float y)
{
	if (this->IsHidden()) return false;
	return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


bool hdUIImage::MouseUp(float x, float y)
{
	if (this->IsHidden()) return false;
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		return true;
	}
	else
	{
		return false;
	}
}


void hdUIImage::Draw() const
{
	if (this->IsHidden()) return;
	DrawVerticesWithTint(((hdReceiver*)this)->GetVertices(), 
						 ((hdReceiver*)this)->GetTextureCoordinates(), 
						 4, m_aabb, m_texture, m_tint, m_alpha);
}

