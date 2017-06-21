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

#include "quadEmitter.h"

quadEmitter::quadEmitter(int maxParticles,
                                   const char * textureName,
                                   float particlePointSize,
                                   float duration,
                                   float gravity,
                                   float initialParticleSpread) : hdParticleEmitter(maxParticles, textureName, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;

    m_drawAsQuad = true;
    m_level = NULL;
}


quadEmitter::~quadEmitter() {}


// make one particle per "start" call.
void quadEmitter::DoStartingTasks()
{
    m_nextParticlePos = (m_nextParticlePos + 1) % m_maxParticles;
    MakeParticle(m_nextParticlePos);
    ExtendTimeStampForStop();
}


void quadEmitter::Step(hdTimeInterval elapsed)
{
    hdParticle p;
    hdVec3 scale;

    for (int i = 0; i < m_maxParticles; ++i)
    {
        if (m_particles->GetItems()[i].life > 0.0)
        {
            hdParticle* p = (hdParticle*)&m_particles->GetItems()[i];

            p->position += (elapsed * p->direction);

            if (m_level != NULL && m_level->GetFloorInfo() != NULL)
            {
                p->position.y = m_level->GetFloorDrawingLevel()+ 0.01f;
            }

            // translate, rotate, scale
            hdTranslateVertices(p->vertices, 4, (elapsed * p->direction));

            // move to center
            hdTranslateVertices(p->vertices, 4, -p->position);

            // rotate around center
            hdRotateVertices(p->vertices, 4, (elapsed * p->rotationDirection));

            // scale - use the life remaining to slow down the scaling as the life runs out
            scale = hdVec3(1.0f, 1.0f, 1.0f) + (((p->life/m_duration) * elapsed) * p->scaleDirection);
            scale.Set(scale.x, 1.0f, scale.z);
            hdScaleVertices(p->vertices, 4, scale);

            // Back
            hdTranslateVertices(p->vertices, 4, p->position);

            // Set the level to the level's water level:
            if (m_level != NULL && m_level->GetFloorInfo() != NULL)
            {
                p->vertices[0].y = m_level->GetFloorDrawingLevel() + 0.025f;
                p->vertices[1].y = m_level->GetFloorDrawingLevel() + 0.025f;
                p->vertices[2].y = m_level->GetFloorDrawingLevel() + 0.025f;
                p->vertices[3].y = m_level->GetFloorDrawingLevel() + 0.025f;
            }

            p->life = hdMax(p->life - elapsed, 0.0f);
        }
    }
}


void quadEmitter::MakeParticle(int pos)
{
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = m_position;

    // Flat, parallel with z and x axes.
    float halfWidth = 0.5f * m_particlePointSize;
    particle.vertices[0].Set(particle.position.x - halfWidth, particle.position.y, particle.position.z + halfWidth);
    particle.vertices[1].Set(particle.position.x - halfWidth, particle.position.y, particle.position.z - halfWidth);
    particle.vertices[2].Set(particle.position.x + halfWidth, particle.position.y, particle.position.z - halfWidth);
    particle.vertices[3].Set(particle.position.x + halfWidth, particle.position.y, particle.position.z + halfWidth);	
    
    particle.color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
    particle.life = m_duration; 
    particle.damping = hdRandom(0.7f, 1.1f);
    
    // Calculate X and Z vectors
    particle.direction.Set(0,0,0);
    
    particle.rotationDirection.Set(0, hdRandom(-1.0f, 1.0f), 0);
    
    float randScale = hdRandom(1.00025f, 1.0005f);
    particle.scaleDirection.Set(randScale, randScale, randScale);
    
    if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
}


const bool quadEmitter::IsFixedDuration() const
{
    return false;
}


void quadEmitter::ParticleAnimationFinished()
{
}
