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

#ifndef _HDK_QUAD_EMITTER_H_
#define _HDK_QUAD_EMITTER_H_

#include <hdk/game.h>

struct hdParticle;

class QuadEmitter : public hdParticleEmitter
{
public:
    QuadEmitter(int maxParticles,
                     const char * texture,
                     float particlePointSize,
                     float duration,
                     float gravity,
                     float initialParticleSpread);

    ~QuadEmitter();

    void Step(hdTimeInterval elapsed);

    void SetLevel(const Level* level);

protected:
    void ParticleAnimationFinished();

    void DoStartingTasks();

    const bool IsFixedDuration() const;

private:
    void MakeParticle(int pos);

    float m_gravity;

    float m_initialParticleSpread;

    int m_nextParticlePos;
    
    Level *m_level;
};


inline void QuadEmitter::SetLevel(const Level* level)
{
    m_level = (Level *)level;
}

#endif
