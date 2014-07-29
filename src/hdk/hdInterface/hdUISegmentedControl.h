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

#ifndef HD_UI_SEGMENT_CONTROL
#define HD_UI_SEGMENT_CONTROL

#include <hdk/hdInterface/hdReceiver.h>
#include <hdk/hdInterface/hdButton.h>
#include <hdk/hdInterface/hdUIImage.h>

typedef struct
{
	char text[64];
	
	int value;
	
} ui_segmentdef_t;


typedef struct
{
	hdTexture *imgNormal;
	
	hdTexture *imgOver;
	
	hdTexture *imgUp;
	
	hdTexture *imgSelected;
	
	hdButton *button;
	
	ui_segmentdef_t def;
	
} ui_segmentcomponent_t;


class hdUISegmentedControl  : public hdReceiver
{
public:
	
	/*
	 * From aabb lower.x, draw:
	 * left edge
	 * -	buttons, with separator in between.
	 *
	 * right edge
	 */
	int AddSegment(const ui_segmentdef_t& segment);
	
	bool RemoveSegment(const int index);
	
	void Draw() const;
	
	const int GetValue(void) const;
	
	void AddValueChangedListener(const void *obj, void (*func)(void *, void *));
	
private:
	hdUIImage *m_imgSeparator;
	
	hdUIImage *m_imgLeftEdgeNormal, *m_imgLeftEdgeOver, *m_imgLeftEdgeUp, *m_imgLeftEdgeSelected;
	
	hdUIImage *m_imgRightEdgeNormal, *m_imgRightEdgeOver, *m_imgRightEdgeUp, *m_imgRightEdgeSelected;
	
	hdUIImage *m_imgButtonNormal, *m_imgButtonOver, *m_imgButtonUp, *m_imgButtonSelected;
	
	// list of buttons
	hdTypedefList<ui_segmentcomponent_t, 64> m_buttons;
	
	// currently selected segement
	int m_value; 

	void (*m_valueChangedCallback)(void *, void *);
	
	void *m_callbackObject;
};

#endif
