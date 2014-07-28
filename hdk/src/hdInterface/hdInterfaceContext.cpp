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

#include "hdInterfaceContext.h"

static hdTypedefList<hdReceiver*, 256> *m_receivers = NULL;

static hdInterfaceController *m_controller = NULL;

static bool loggingEnabled = false;


void hdInterfaceContext::Init(hdInterfaceController *context)
{
	m_controller = context;
	
	if (m_receivers != NULL)
	{
		delete m_receivers;
	}
	m_receivers = new hdTypedefList<hdReceiver*, 256>();
}


void hdInterfaceContext::TearDown(hdInterfaceController *context)
{
	if (m_controller != NULL
		&& m_controller != context)
		return;
	
	if (m_receivers)
	{
		delete m_receivers;
	}
	m_receivers = NULL;
	m_controller = NULL;
}


void hdInterfaceContext::EnableLogging()
{
	loggingEnabled = true;
}


void hdInterfaceContext::DisableLogging()
{
	loggingEnabled = false;
}


bool hdInterfaceContext::AddReceiver(const hdReceiver *receiver)
{
	int res;
	if (!m_receivers) return false;
	
	res = m_receivers->Add((hdReceiver *)receiver);
	
	if (HD_COLLECTIONS_ERROR_FULL == res ||
		HD_COLLECTIONS_ERROR_INVALID_INDEX == res)
	{
		hdError(-1, "Collection was full. Could not add receiver.");
		return false;
	}
	
	return true;
}


bool hdInterfaceContext::RemoveReceiver(const hdReceiver *receiver)
{
	return true;
}


void hdInterfaceContext::HandleTapUp(float x, float y, int tapCount)
{
	uint32 i;
	hdVec2 screen;
	
	if (m_controller == NULL) return;
	
	m_controller->HandleTapUp(x, y, tapCount);
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;
	m_controller->ConvertRawToScreen(screen, x, y);
	
	if (loggingEnabled)
		hdPrintf("%3.3f, %3.3f, %3.3f, %3.3f\n", x, y, screen.x, screen.y);
	
	for (i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->MouseUp(screen.x, screen.y);
	}
}


void hdInterfaceContext::HandleTapDown(float x, float y, int tapCount)
{
	uint32 i;
	hdVec2 screen;
	
	if (m_controller == NULL) return;
	
	m_controller->HandleTapDown(x, y, tapCount);
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;
	m_controller->ConvertRawToScreen(screen, x, y);
	
	if (loggingEnabled)
		hdPrintf("%3.3f, %3.3f\n", screen.x, screen.y);
	
	for (i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->MouseDown(screen.x, screen.y);
	}
}


void hdInterfaceContext::HandleSingleTap(const float x, const float y)
{
	hdVec2 screen;
	
	if (m_controller == NULL) return;
	
	m_controller->HandleSingleTap(x, y);
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;

	m_controller->ConvertRawToScreen(screen, x, y);
}
	 


void hdInterfaceContext::HandleDoubleTap(float x, float y)
{
	uint32 i;
	hdVec2 screen;
	
	if (m_controller == NULL) return;
	
	m_controller->HandleDoubleTap(x, y);
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;
	m_controller->ConvertRawToScreen(screen, x, y);
	
	for ( i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->MouseDoubleClick(screen.x, screen.y);
	}
}


void hdInterfaceContext::HandleTapMovedSingle(const float previousX, const float previousY, 
						  const float currentX, const float currentY)
{
	uint32 i;
	hdVec2 screen;
	
	if (m_controller == NULL) return;
	
	m_controller->HandleTapMovedSingle(previousX, previousY, currentX, currentY);
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;
	m_controller->ConvertRawToScreen(screen, currentX, currentY);
	
	if (loggingEnabled)
		hdPrintf("%3.3f, %3.3f\n", screen.x, screen.y);
	
	for ( i = 0; i < m_receivers->GetItemCount(); ++i)
	{
		m_receivers->GetItems()[i]->MouseOver(screen.x, screen.y);
	}
}


void hdInterfaceContext::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
						  const float aCurrentX, const float aCurrentY,
						  const float bPreviousX, const float bPreviousY,
						  const float bCurrentX, const float bCurrentY)
{
	if (m_controller == NULL) return;
	
	if (m_receivers == NULL) return;
	if (m_receivers->GetItemCount() == 0) return;
	
	m_controller->HandleTapMovedDouble(aPreviousX, aPreviousY,
									   aCurrentX, aCurrentY,
									   bPreviousX, bPreviousY,
									   bCurrentX, bCurrentY);
}
