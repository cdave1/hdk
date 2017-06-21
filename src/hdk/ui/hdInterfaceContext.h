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

#ifndef HD_INTERFACE_CONTEXT_H
#define HD_INTERFACE_CONTEXT_H

#include <hdk/hdGame.h>
#include <hdk/hdGraphicsLibrary.h>
#include <hdk/hdMath.h>
#include <hdk/hdCollections.h>
#include <hdk/hdInterface/hdReceiver.h>
#include <hdk/hdInterface/hdInterfaceController.h>

/*
 * Retains a list of receivers. Receivers add themselves
 * to this context when they are created.
 *
 * The context dispatches events to each of the receivers.
 *
 * A context exists for the duration of an interface
 * controller, but is not tied to the interface controller.
 *
 * Events are sent via app controller; if there is no context
 * then just do nothing.
 */

class hdInterfaceController;

class hdInterfaceContext
{
public:
    static void Init(hdInterfaceController *context);

    static void TearDown(hdInterfaceController *context);

    static void EnableLogging();

    static void DisableLogging();

    static bool AddReceiver(const hdReceiver *receiver);

    static bool RemoveReceiver(const hdReceiver *receiver);

    static void HandleTapUp(float x, float y, int tapCount);

    static void HandleTapDown(float x, float y, int tapCount);

    static void HandleSingleTap(const float x, const float y);

    static void HandleDoubleTap(float x, float y);

    static void HandleTapMovedSingle(const float previousX, const float previousY,
                                     const float currentX, const float currentY);

    static void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
                                     const float aCurrentX, const float aCurrentY,
                                     const float bPreviousX, const float bPreviousY,
                                     const float bCurrentX, const float bCurrentY);
};

#endif
