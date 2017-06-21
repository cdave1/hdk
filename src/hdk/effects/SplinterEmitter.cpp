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

#include "SplinterEmitter.h"

SplinterEmitter::SplinterEmitter(int maxParticles,
                                           float particlePointSize,
                                           float duration,
                                           float gravity,
                                           float initialParticleSpread) : hdParticleEmitter(maxParticles, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;
    m_nextParticlePos = 0;

    m_drawAsQuad = true;
    m_nextParticleCount = 12;
}


SplinterEmitter::~SplinterEmitter() {}


void SplinterEmitter::DoStartingTasks()
{
    for (int i = 0; i < m_nextParticleCount; ++i)
    {
        m_nextParticlePos = (m_nextParticlePos + 1) % m_maxParticles;
        MakeParticle(m_nextParticlePos);
    }
    ExtendTimeStampForStop();
}


void SplinterEmitter::Step(hdTimeInterval elapsed)
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

            // translate, rotate
            hdTranslateVertices(p->vertices, 4, (elapsed * p->direction));
            hdTranslateVertices(p->vertices, 4, -p->position);
            hdRotateVertices(p->vertices, 4, (elapsed * p->rotationDirection));
            hdTranslateVertices(p->vertices, 4, p->position);

            p->life = hdMax(p->life - elapsed, 0.0f);
        }
    }
}


void SplinterEmitter::MakeParticle(int pos)
{
    float Angle;
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = hdVec3(hdRandom(m_aabb.lower.x, m_aabb.upper.x),
                               hdRandom(m_aabb.lower.y, m_aabb.upper.y),
                               hdRandom(m_aabb.lower.z, m_aabb.upper.z));
    particle.color.Set(m_particleColor.r, m_particleColor.g, m_particleColor.b, 255);
    particle.life = m_duration;
    particle.damping = hdRandom(0.75f, 1.0f);

    float halfWidth = m_particlePointSize;
    particle.vertices[0].Set(particle.position.x - hdRandom(0.025f, 0.12f), particle.position.y, particle.position.z + halfWidth);
    particle.vertices[1].Set(particle.position.x - hdRandom(0.025f, 0.12f), particle.position.y, particle.position.z - halfWidth);

    particle.vertices[2].Set(particle.vertices[1].x + 0.001f, particle.position.y, particle.position.z - halfWidth);
    particle.vertices[3].Set(particle.position.x + hdRandom(0.025f, 0.12f), particle.position.y, particle.position.z + halfWidth);

    Angle = (float)(rand() % 360);

    // Calculate X and Z vectors
    particle.direction.x = hdCosf(Angle)*(hdRandom(0.0f, m_initialParticleSpread));
    particle.direction.z = 0.0f; //hdSinf(Angle)*(hdRandom(0.0f, m_initialParticleSpread));
    particle.direction.y = hdSinf(Angle)*(hdRandom(0.0f, m_initialParticleSpread));
    
    particle.rotationDirection.Set(hdRandom(-1.0f, 1.0f), hdRandom(-1.0f, 1.0f), hdRandom(-3.0f, 3.0f));
    
    if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
}


const bool SplinterEmitter::IsFixedDuration() const
{
    return false;
}


void SplinterEmitter::ParticleAnimationFinished()
{
}
