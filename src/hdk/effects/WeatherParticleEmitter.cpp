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

#include "WeatherParticleEmitter.h"

WeatherParticleEmitter::WeatherParticleEmitter(int maxParticles,
                                                         const char * textureName,
                                                         float particlePointSize,
                                                         float duration,
                                                         float gravity,
                                                         float initialParticleSpread,
                                                         hdGameObject *boundingBoxProvider,
                                                         const hdVec3& directionLo, const hdVec3& directionHi) : hdParticleEmitter(maxParticles, textureName, particlePointSize, duration)
{
    m_gravity = gravity;
    m_initialParticleSpread = initialParticleSpread;
    m_drawAsQuad = true;
    m_nextParticlePos = 0;
    m_boundingBoxProvider = boundingBoxProvider;
    m_directionLo = directionLo;
    m_directionHi = directionHi;
}


WeatherParticleEmitter::~WeatherParticleEmitter() {}


// Each splash adds 16 particles to the particle emitter.
void WeatherParticleEmitter::DoStartingTasks()
{
    ExtendTimeStampForStop();
}


// Constantly adds new particles - up to 3 deads replaced per step
void WeatherParticleEmitter::Step(hdTimeInterval elapsed)
{
    int newParticles = 0;

    for (int i = 0; i < m_maxParticles; ++i)
    {
        if (m_particles->GetItems()[i].life > 0.0)
        {
            hdParticle* p = (hdParticle*)&m_particles->GetItems()[i];

            p->position += (elapsed * p->direction);

            // translate, rotate, scale
            hdTranslateVertices(p->vertices, 4, (elapsed * p->direction));

            p->life = hdMax(p->life - elapsed, 0.0f);
        }
        else if (newParticles < 3)
        {
            MakeParticle(i);
            ++newParticles;
        }
    }
}


void WeatherParticleEmitter::MakeParticle(int pos)
{
    hdParticle particle;

    particle = hdParticle();

    // Initial position of the particle is a
    // random position within the aabb.
    particle.position = hdVec3(hdRandom(m_aabb.lower.x, m_aabb.upper.x),
                               hdRandom(m_aabb.lower.y, m_aabb.upper.y),
                               hdRandom(m_aabb.lower.z, m_aabb.upper.z));
    particle.color.Setf(1.0f, 1.0f, 1.0f, 1.0f);
    particle.life = hdRandom(0.5*m_duration,m_duration);
    particle.damping = hdRandom(0.8f, 1.3f);

    float halfWidth = 0.5f * m_particlePointSize;
    particle.vertices[0].Set(particle.position.x - halfWidth, particle.position.y- halfWidth, particle.position.z);
    particle.vertices[1].Set(particle.position.x - halfWidth, particle.position.y+ halfWidth, particle.position.z);
    particle.vertices[2].Set(particle.position.x + halfWidth, particle.position.y+ halfWidth, particle.position.z);
    particle.vertices[3].Set(particle.position.x + halfWidth, particle.position.y- halfWidth, particle.position.z);

    // Calculate X and Z vectors
    particle.direction.Set(hdRandom(m_directionLo.x, m_directionHi.x),
                           hdRandom(m_directionLo.y, m_directionHi.y),
                           hdRandom(m_directionLo.z, m_directionHi.z));
    particle.rotationDirection.Set(0, 0, hdRandom(-3.0f, 3.0f));
    particle.scaleDirection.Set(1.0f, 1.0f, 1.0f);

    if (!m_particles->Replace(particle, pos)) m_particles->Add(particle);
}


void WeatherParticleEmitter::ParticleAnimationFinished()
{
    this->Start(m_aabb);
}


const bool WeatherParticleEmitter::IsFixedDuration() const
{
    return false;
}
