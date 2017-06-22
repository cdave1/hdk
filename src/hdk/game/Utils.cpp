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

#include "Utils.h"

void b2Mat22ToMatrix(hdMatrix &mOut, const b2Mat22& mIn)
{
    MatrixIdentity(mOut);

    mOut.f[__11] = mIn.col1.x;	mOut.f[__12] = mIn.col1.y;
    mOut.f[__21] = mIn.col2.x;	mOut.f[__22] = mIn.col2.y;
}


void UtilDrawAABB(const hdAABB &aabb)
{
    hdglBegin(GL_QUADS);
    hdglColor4f(1.0f, 1.0f, 1.0f, 0.25f);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.lower.z);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.lower.z);
    hdglColor4f(0.0f, 1.0f, 1.0f, 0.25f);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.upper.z);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.upper.z);
    hdglColor4f(0.0f, 0.0f, 1.0f, 0.25f);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    hdglColor4f(0.0f, 0.0f, 0.0f, 0.25f);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.upper.z);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    hdglColor4f(1.0f, 1.0f, 0.0f, 0.25f);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.upper.z);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    hdglVertex3f(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    hdglColor4f(1.0f, 0.0f, 1.0f, 0.25f);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.upper.z);
    hdglVertex3f(aabb.upper.x, aabb.upper.y, aabb.lower.z);
    hdglVertex3f(aabb.upper.x, aabb.lower.y, aabb.lower.z);
    hdglEnd();
}


void UtilGenerateColor(float* out, float pos, float end)
{
    float r, g, b;

    float den = (1.0f/6.0f);

    r = g = b = 0.0f;

    float norm = hdMax(0.0f,pos-1)/end;

    if (0.0f <= norm && norm <= 1.0f/6.0f)
    {
        r = 1.0f;
        g = norm/den;
        b = 0.0f;
    }
    else if (1.0f/6.0f <= norm && norm <= 2.0f/6.0f)
    {
        r = (den - (norm - (1.0f/6.0f)))/den;
        g = 1.0f;
        b = 0.0f;
    }
    else 	if (2.0f/6.0f <= norm && norm <= 3.0f/6.0f)
    {
        r = 0.0f;
        g = 1.0f;
        b = (norm - (2.0f/6.0f))/den;
    }
    else 	if (3.0f/6.0f <= norm && norm <= 4.0f/6.0f)
    {
        r = 0.0f;
        g = (den - (norm - (3.0f/6.0f)))/den;
        b = 1.0f;
    }
    else 	if (4.0f/6.0f <= norm && norm <= 5.0f/6.0f)
    {
        r = (norm - (4.0f/6.0f))/den;
        g = 0.0f;
        b = 1.0f;
    }
    else 	if (5.0f/6.0f <= norm && norm <= 1.0f)
    {
        r = 1.0f;
        g = 0.0f;
        b = (den - (norm - (5.0f/6.0f)))/den;
    }
    
    out[0] = r;
    out[1] = g;
    out[2] = b;
    out[3] = 1.0f;
}
