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

#ifndef _HD_2D_GAME_CONTROLLER_
#define _HD_2D_GAME_CONTROLLER_

#include <hdk/math/hdMath.h>

class hdGameWorld;

class hd2DGameController
{
private:
    hdVec2 m_targetScreenArea; // we are drawing for the iPhone.

    hdVec2 m_targetScreenPosition;

    float32 m_timeStep;

    int32 m_iterations;

    // gameworld stuff
    hdGameWorld *m_gameWorld;

    int32 m_pointCount;

public:
    hd2DGameController(const char* path);

    ~hd2DGameController();

    void Step(double interval);

    void Draw();

    void HandleDoubleTap(float x, float y);

    void PanProjection(const float aPreviousX, const float aPreviousY,
                       const float aCurrentX, const float aCurrentY);

    void ZoomProjection(const float aPreviousX, const float aPreviousY,
                        const float aCurrentX, const float aCurrentY,
                        const float bPreviousX, const float bPreviousY,
                        const float bCurrentX, const float bCurrentY);

};

#endif
