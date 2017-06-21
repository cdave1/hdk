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

#ifndef HD_ANIMATION_H
#define HD_ANIMATION_H

#include <hdk/animation/hdAction.h>
#include <hdk/collections.h>

class hdAnimationController;
class hdGameObject;


typedef enum
{
    e_animationStopped = 0,
    e_animationPaused,
    e_animationRunning,
    e_animationFinished,
    e_animationStatusCount
} e_AnimationStatus;


class hdAnimation
{
public:

    int StartAnimation();

    void PauseAnimation();

    void ResumeAnimation();

    void StopAnimation(bool completeAnimation);

    void StopAnimation(bool completeAnimation, bool doCallback);

    void StopAnimationDead();

    void StopAnimationWithCompletion();

    void Step(hdTimeInterval elapsed);

    /*
     * Function to call on an object when the animation finishes successfully.
     */
    void SetFinishedCallback(const void* obj, void (*func)(void *, hdAnimation *));

    void RemoveCallbackFunctions();

    const hdPointerList<hdAction, 16>* GetActionList() const;

    const hdTypedefList<hdGameObject*, 128>* GetGameObjects() const;

    hdGameObject* GetGameObject(int index) const;

    const int GetGameObjectCount();

    const int AddAction(const hdAction *action);

    const bool RemoveAction(const hdAction* action);

    const int AddGameObject(const hdGameObject *object);

    const bool RemoveGameObject(const hdGameObject *object);

    const bool RemoveAllGameObjects();

    const bool RemoveAllActions();

    const e_AnimationStatus GetStatus() const;


private:
    friend class hdAnimationController;
    friend class purgeAnimsPredicate;

    hdAnimation();

    ~hdAnimation();

    hdTypedefList<hdGameObject*, 128>* m_gameObjects;

    hdPointerList<hdAction, 16>* m_actions;

    int m_actionCounter;

    int m_id; // internal animation controller identifier.

    hdTimeInterval m_duration;

    hdTimeInterval m_progress;

    e_AnimationStatus m_status;

    void *m_parent;

    void* m_callbackObj;

    void (*m_callbackFunc)(void *, hdAnimation *);

    void DoFinishedCallback();

    bool m_isLocked;
};


inline const hdPointerList<hdAction, 16>* hdAnimation::GetActionList() const
{
    return m_actions;
}


inline const hdTypedefList<hdGameObject*, 128>* hdAnimation::GetGameObjects() const
{
    return m_gameObjects;
}


inline hdGameObject* hdAnimation::GetGameObject(int index) const
{
    return m_gameObjects->GetItems()[index];
}


inline const int hdAnimation::GetGameObjectCount()
{
    return m_gameObjects->GetItemCount();
}


inline const int hdAnimation::AddAction(const hdAction *action)
{
    if (m_status == e_animationRunning) return -1;
    return m_actions->Add(action);
}


inline const bool hdAnimation::RemoveAction(const hdAction* action)
{
    if (m_status == e_animationRunning) return -1;
    return m_actions->Remove(action);
}


inline const int hdAnimation::AddGameObject(const hdGameObject *object)
{
    return m_gameObjects->Add((hdGameObject *)object);
}


inline const bool hdAnimation::RemoveGameObject(const hdGameObject *object)
{
    return false; //m_gameObjects->Remove(object);
}


inline const bool hdAnimation::RemoveAllGameObjects()
{
    return m_gameObjects->RemoveAll();
}


inline const bool hdAnimation::RemoveAllActions()
{
    return m_actions->RemoveAll();
}


inline const e_AnimationStatus hdAnimation::GetStatus() const
{
    return m_status;
}


inline void hdAnimation::SetFinishedCallback(const void* obj, void (*func)(void *, hdAnimation *))
{
    m_callbackObj = (void *)obj;
    m_callbackFunc = func;
}


inline void hdAnimation::RemoveCallbackFunctions()
{	
    m_callbackObj = NULL;
    m_callbackFunc = NULL;
}


inline void hdAnimation::DoFinishedCallback()
{
    if (m_callbackObj == NULL) return;
    
    (*m_callbackFunc)(m_callbackObj, this);
}

#endif
