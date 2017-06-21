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

#include "SparkEmitter.h"

SparkEmitter::SparkEmitter(int maxParticles,
                                     const char * textureName,
                                     float particlePointSize,
                                     float duration,
                                     float gravity,
                                     float initialParticleSpread) : hdParticleEmitter(maxParticles, textureName, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;
    m_drawAsQuad = true;
}


SparkEmitter::~SparkEmitter() {}


void SparkEmitter::Step(hdTimeInterval elapsed)
{
    for (int i = 0; i < m_maxParticles; ++i)
    {
        if (m_particles->GetItems()[i].life > 0.0)
        {
            hdParticle* p = (hdParticle*)&m_particles->GetItems()[i];

            float xPos = (p->position.x + p->direction.x * elapsed);
            float yPos = (p->position.y + p->direction.y * elapsed);
            float zPos = (p->position.z + p->direction.z * elapsed);

            p->direction.y += ((m_gravity * elapsed) * p->damping);

            p->position = hdVec3(xPos, yPos, zPos);
            p->life = hdMax(p->life - elapsed, 0.0f);
        }
        else
        {
            if (GetRemainingTime() > (m_duration / 2.0))
            {
                MakeParticle(i);
            }
        }
    }
}


void SparkEmitter::MakeParticle(int pos)
{
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = hdVec3(hdRandom(m_aabb.lower.x, m_aabb.upper.x),
                               hdRandom(m_aabb.lower.y, m_aabb.upper.y),
                               hdRandom(m_aabb.lower.z-0.01f, m_aabb.upper.z+0.01f));
    particle.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    particle.life = m_duration;
    particle.damping = hdRandom(0.7f, 1.1f);

    // Calculate X and Z vectors
    particle.direction.x = m_direction.x + hdRandom(-(m_initialParticleSpread/2.0f), (m_initialParticleSpread/2.0f));
    particle.direction.z = m_direction.y + hdRandom(-(m_initialParticleSpread/2.0f), (m_initialParticleSpread/2.0f));
    particle.direction.y = hdRandom(2.0f, 5.0f);
    
    particle.rotationDirection.x = 0.0f;
    particle.rotationDirection.y = 0.0f;	
    particle.rotationDirection.z = hdRandom(-30.0f, 30.0f);
    
    if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
}


void SparkEmitter::SetDirection(const float x, const float y, const float z)
{
    m_direction.Set(x, y, z);
}


void SparkEmitter::ParticleAnimationFinished()
{
}
