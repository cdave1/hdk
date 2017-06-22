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

#ifndef _HDK_SPLINTER_EMITTER_H
#define _HDK_SPLINTER_EMITTER_H

#include <hdk/game.h>
#include <hdk/particles.h>

struct hdParticle;

class SplinterEmitter : public hdParticleEmitter
{
public:
    SplinterEmitter(int maxParticles,
                         float gravity,
                         float initialParticleSpread,
                         float particlePointSize,
                         float duration);

    ~SplinterEmitter();

    void Step(hdTimeInterval elapsed);

    void SetParticleColor(const hdColor4& color);


protected:
    void ParticleAnimationFinished();

    void DoStartingTasks();

    const bool IsFixedDuration() const;


private:
    void MakeParticle(int pos);

    float m_gravity;

    float m_initialParticleSpread;

    int m_nextParticlePos;

    hdColor4 m_particleColor;
};


inline void SplinterEmitter::SetParticleColor(const hdColor4& color)
{
    m_particleColor.Set(color.r, color.g, color.b, color.a);
}

#endif
