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

#include "StarEmitter.h"

StarEmitter::StarEmitter(int maxParticles,
                                   const char * texture,
                                   float gravity,
                                   float initialParticleSpread,
                                   float particlePointSize,
                                   float duration) : hdParticleEmitter(maxParticles, texture, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;
    m_onStopResume = false;
    m_ySpreadLower = -initialParticleSpread;
    m_ySpreadUpper = initialParticleSpread;
    m_drawAsQuad = true;
}


StarEmitter::~StarEmitter() {}


void StarEmitter::Step(hdTimeInterval elapsed)
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

            // translate, rotate
            hdTranslateVertices(p->vertices, 4, (elapsed * p->direction));
            hdTranslateVertices(p->vertices, 4, -p->position);
            hdRotateVertices(p->vertices, 4, (elapsed * p->rotationDirection));
            hdTranslateVertices(p->vertices, 4, p->position);

            p->position = hdVec3(xPos, yPos, zPos);
            p->life = hdMax(p->life - elapsed, 0.0f);
        }
        else
        {
            if (GetRemainingTime() > (m_duration / 2.0))
            {
                if ((rand() % 100) < 33)
                    MakeParticle(i);
            }
        }
    }
}


void StarEmitter::MakeParticle(int pos)
{
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = hdVec3(hdRandom(m_aabb.lower.x, m_aabb.upper.x),
                               hdRandom(m_aabb.lower.y, m_aabb.upper.y),
                               0.75f);
    particle.color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
    particle.life = m_duration;
    particle.damping = hdRandom(0.7f, 1.1f);

    // Calculate X and Z vectors
    particle.direction.x = hdRandom(-(m_initialParticleSpread/2.0f),
                                    (m_initialParticleSpread/2.0f));
    particle.direction.z = 0.0f;
    particle.direction.y = hdRandom(m_ySpreadLower, m_ySpreadUpper);

    float halfWidth = 0.5f * m_particlePointSize;
    particle.vertices[0].Set(particle.position.x - halfWidth, particle.position.y - halfWidth, particle.position.z);
    particle.vertices[1].Set(particle.position.x - halfWidth, particle.position.y + halfWidth, particle.position.z);
    particle.vertices[2].Set(particle.position.x + halfWidth, particle.position.y + halfWidth, particle.position.z);
    particle.vertices[3].Set(particle.position.x + halfWidth, particle.position.y - halfWidth, particle.position.z);	
    
    particle.rotationDirection.Set(0, 0, hdRandom(-1.0f, 1.0f));
    
    if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
}


void StarEmitter::ParticleAnimationFinished()
{
    if (m_onStopResume) this->Start(m_aabb);
}
