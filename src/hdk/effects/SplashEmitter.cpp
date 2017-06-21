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

#include "SplashEmitter.h"

SplashEmitter::SplashEmitter(int maxParticles,
                                       const char * textureName,
                                       float particlePointSize,
                                       float duration,
                                       float gravity,
                                       float initialParticleSpread) : hdParticleEmitter(maxParticles, textureName, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;
    m_nextParticlePos = 0;
    m_nextSplashIsSmall = false;
    m_drawAsQuad = true;
}


SplashEmitter::~SplashEmitter() {}


// Each splash adds new particles to the particle emitter.
void SplashEmitter::DoStartingTasks()
{
    int splashes;

    if (m_nextSplashIsSmall) splashes = 6;
    else splashes = 12;

    for (int i = 0; i < splashes; ++i)
    {
        m_nextParticlePos = (m_nextParticlePos + 1) % m_maxParticles;
        MakeParticle(m_nextParticlePos);
    }
    ExtendTimeStampForStop();
}


void SplashEmitter::Step(hdTimeInterval elapsed)
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


void SplashEmitter::MakeParticle(int pos)
{
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = hdVec3(hdRandom(m_aabb.lower.x, m_aabb.upper.x),
                               hdRandom(m_aabb.lower.y, m_aabb.upper.y),
                               hdRandom(m_aabb.lower.z-0.75f, m_aabb.upper.z+0.75f));
    particle.color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
    particle.life = m_duration;
    particle.damping = hdRandom(0.8f, 1.3f);

    // Calculate X and Z vectors
    particle.direction.x = hdRandom(-(m_initialParticleSpread/2.0f), (m_initialParticleSpread/2.0f));
    particle.direction.z = hdRandom(-(m_initialParticleSpread/2.0f), (m_initialParticleSpread/2.0f));
    particle.direction.y = hdRandom(2.0f, 5.0f);

    float halfWidth = 0.5f * m_particlePointSize;
    particle.vertices[0].Set(particle.position.x - halfWidth, particle.position.y - halfWidth, particle.position.z);
    particle.vertices[1].Set(particle.position.x - halfWidth, particle.position.y + halfWidth, particle.position.z);
    particle.vertices[2].Set(particle.position.x + halfWidth, particle.position.y + halfWidth, particle.position.z);
    particle.vertices[3].Set(particle.position.x + halfWidth, particle.position.y - halfWidth, particle.position.z);	
    
    particle.rotationDirection.Set(0, 0, hdRandom(-1.0f, 1.0f));
    
    if (m_particles->GetItems()[pos].life <= 0.9f)
    {
        if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
    }
}


const bool SplashEmitter::IsFixedDuration() const
{
    return false;
}


void SplashEmitter::ParticleAnimationFinished()
{
}
