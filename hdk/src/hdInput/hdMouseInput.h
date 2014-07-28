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

#ifndef HD_MOUSE_INPUT
#define HD_MOUSE_INPUT

typedef enum clickstate
{
    e_clickstate_null = 0,
    e_clickstate_down = 1,
    e_clickstate_moving = 2,
    e_clickstate_up = 3,
    e_clickstate_scrollup = 4,
    e_clickstate_scrolldown = 5,
}
e_clickstate_t;


typedef struct clickinfo
{
    clickinfo() :
        xClickDown(0.0f),
        yClickDown(0.0f),
        xClickUp(0.0f),
        yClickUp(0.0f),
        clickDownTimeStamp(0),
        clickUpTimeStamp(0),
        isDown(false),
        isMoving(false),
        isUp(false),
        clickCount(0)
    {}

    float xClickDown;
    float yClickDown;
    float xClickUp;
    float yClickUp;
    double clickDownTimeStamp;
    double clickUpTimeStamp;
    bool isDown;
    bool isMoving;
    bool isUp;
    uint16 clickCount;
}
clickinfo_t;


typedef struct mousevalues
{
    mousevalues() :
        xCurrent(0.0f),
        yCurrent(0.0f),
        xPrevious(0.0f),
        yPrevious(0.0f) {}

    float xCurrent;    // x and y coordinates where the touch began
    float yCurrent;

    float xPrevious;
    float yPrevious;
    
    clickinfo_t leftButton;
    
    clickinfo_t rightButton;
    
    clickinfo_t scrollWheel;
} 
mousevalues_t;

extern mousevalues_t GetMouseValues();

#endif
