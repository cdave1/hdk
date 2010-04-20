/*
 *  hdFluidFont.cpp
 *  hdGameEngine
 *
 *  Created by david on 14/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdFont.h"

hdFont::hdFont()
{
	m_texture = NULL;
	memset(fileChars, 0, sizeof(fileChars));
}


