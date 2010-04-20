/*
 *  hdUIImage.h
 *  TotemGame
 *
 *  Created by david on 27/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_UI_IMAGE_H_
#define _HD_UI_IMAGE_H_

#include "hdInterface/hdReceiver.h"
#include "hdCollections.h"


class hdUIImage : public hdReceiver
{
public:
	hdUIImage() {} 
	
	hdUIImage(const char* texture,  
			  hdGameWorld* gameWorld); 
	
	
	hdUIImage(const char* texture,  
			 hdGameWorld* gameWorld, 
			 const hdAABB& aabb);
	
	hdUIImage(const char* texture,
			 hdGameWorld* gameWorld, 
			 const float x, const float y, 
			 const float w, const float h);
	
	~hdUIImage();
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	void Draw() const;

	const hdTexture* GetTexture() const;
	
	void SetTexture(const hdTexture *texture);
	
	void SetTint(float r, float g, float b, float a);
	
private:
	hdTexture* m_texture;
	
	float* m_tint;
	
};


inline const hdTexture* hdUIImage::GetTexture() const
{
	return m_texture;
}


inline void hdUIImage::SetTexture(const hdTexture *texture)
{
	m_texture = (hdTexture *)texture;
}


inline void hdUIImage::SetTint(float r, float g, float b, float a)
{
	m_tint[0] = r;
	m_tint[1] = g;
	m_tint[2] = b;
	m_tint[3] = a;
}



#endif