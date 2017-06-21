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

#include "hdParticleEmitter.h"

hdParticleEmitter::hdParticleEmitter(int maxParticles,
                                     const char * textureName,
                                     float particlePointSize,
                                     float duration)
{
    m_maxParticles = hdClamp(maxParticles, 0, kMaxParticles);

    m_texture = hdTextureManager::Instance()->FindTexture(textureName, TT_Sprite);

    m_particlePointSize = particlePointSize;

    m_duration = duration;

    m_blendStyle = e_particleBlendStyleNormal;

    this->Init();
}


hdParticleEmitter::hdParticleEmitter(int maxParticles,
                                     float particlePointSize,
                                     float duration)
{
    m_maxParticles = hdClamp(maxParticles, 0, kMaxParticles);

    m_texture = NULL;

    m_particlePointSize = particlePointSize;

    m_duration = duration;

    m_blendStyle = e_particleBlendStyleNormal;

    this->Init();
}


hdParticleEmitter::hdParticleEmitter(int maxParticles,
                                     float particlePointSize,
                                     float duration,
                                     e_particleBlendStyle style)
{
    m_maxParticles = hdClamp(maxParticles, 0, kMaxParticles);

    m_texture = NULL;

    m_particlePointSize = particlePointSize;

    m_duration = duration;

    m_blendStyle = style;

    this->Init();
}


void hdParticleEmitter::Init()
{
    m_particles = new hdTypedefList<hdParticle, kMaxParticles>();

    m_animation = hdAnimationController::CreateAnimation(this, false);
    m_animation->AddGameObject((hdGameObject *)this);
    m_animation->SetFinishedCallback(this, hdParticleEmitter::AnimationFinishedCallback);
    m_functionAction = new hdFunctionAction<hdParticleEmitter>();
    m_functionAction->SetFunctionObject(this);
    m_functionAction->SetFunction(&hdParticleEmitter::Step);
    m_functionAction->SetDuration(m_duration + 0.1f);
    m_animation->AddAction(m_functionAction);
    m_drawAsQuad = false;
}


hdParticleEmitter::~hdParticleEmitter()
{
    //hdAssert(m_animation != NULL);
    //m_animation->StopAnimation(false);
    hdAnimationController::Instance()->StopAnimations(this);
    hdAnimationController::Instance()->PurgeAnimations(this);
    delete m_particles;
}


void hdParticleEmitter::Start(const hdVec3& pos)
{
    hdAABB aabb;
    aabb.lower = hdVec3(pos.x - 0.01f, pos.y - 0.01f, pos.z - 0.01f);
    aabb.upper = hdVec3(pos.x + 0.01f, pos.y + 0.01f, pos.z + 0.01f);

    this->Start(aabb);
}



void hdParticleEmitter::Start(float x, float y)
{
    hdAABB aabb;
    aabb.lower = hdVec3(x - 0.01f, y - 0.01f, 0.01f);
    aabb.upper = hdVec3(x + 0.01f, y + 0.01f, 0.01f);

    this->Start(aabb);
}


void hdParticleEmitter::Start(const hdAABB& aabb)
{
    m_aabb = aabb;
    m_position = aabb.lower + (0.5f * (aabb.upper - aabb.lower));

    if (this->IsFixedDuration())
    {
        if (m_animation->GetStatus() == e_animationRunning || m_animation->GetStatus() == e_animationPaused)
        {
            m_animation->StopAnimation(true);
        }
        m_animation->StartAnimation();
    }
    else
    {
        // Only start animation if already stopped
        if (m_animation->GetStatus() != e_animationRunning && m_animation->GetStatus() != e_animationPaused)
        {
            m_animation->StartAnimation();
        }
    }


    this->DoStartingTasks();
}


void hdParticleEmitter::Start(const hdTexture *texture, float x, float y)
{
    m_texture = (hdTexture *)texture;
    this->Start(x, y);
}


void hdParticleEmitter::Start(const hdTexture *texture, const hdAABB& aabb)
{
    m_texture = (hdTexture *)texture;
    this->Start(aabb);
}


void hdParticleEmitter::ExtendTimeStampForStop()
{
    if (m_animation == NULL) return;
    if (m_functionAction == NULL) return;

    if (m_animation->GetStatus() == e_animationRunning || m_animation->GetStatus() == e_animationPaused)
    {
        m_functionAction->SetProgress(0.0);
    }
}



void hdParticleEmitter::Stop()
{
    m_animation->StopAnimation(false, true);
}


void hdParticleEmitter::AnimationFinishedCallback(void *handler, hdAnimation *animation)
{
    hdParticleEmitter *self = (hdParticleEmitter *)handler;
    self->ParticleAnimationFinished();
}


const bool hdParticleEmitter::IsFixedDuration() const
{
    return true;
}


void hdParticleEmitter::Draw() const
{
    if (m_animation->GetStatus() != e_animationRunning && m_animation->GetStatus() != e_animationPaused) return;

    hdglError("particle emitter start");
    glEnable(GL_BLEND);

    if (m_blendStyle == e_particleBlendStyleNormal)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else if (m_blendStyle == e_particleBlendStyleMultiply)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else if (m_blendStyle == e_particleBlendStyleDarken)
    {
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glEnable(GL_TEXTURE_2D);

    if (m_drawAsQuad)
    {
        hdglBindTexture(m_texture);

        hdParticle particle;
        hdglBegin(GL_QUADS);
        for (int i = 0; i < m_particles->GetItemCount(); i++)
        {
            particle = m_particles->GetItems()[i];

            hdglColor4ub(particle.color.r, particle.color.g, particle.color.b, (unsigned char)(255 * fabs(particle.life) / m_duration));

            hdglTexCoord2f(0.0f, 1.0f);
            hdglVertex3f(particle.vertices[0].x, particle.vertices[0].y, particle.vertices[0].z);
            hdglTexCoord2f(0.0f, 0.0f);
            hdglVertex3f(particle.vertices[1].x, particle.vertices[1].y, particle.vertices[1].z);
            hdglTexCoord2f(1.0f, 0.0f);
            hdglVertex3f(particle.vertices[2].x, particle.vertices[2].y, particle.vertices[2].z);
            hdglTexCoord2f(1.0f, 1.0f);
            hdglVertex3f(particle.vertices[3].x, particle.vertices[3].y, particle.vertices[3].z);

        }
        hdglEnd();
    }
    else
    {
        hdglBindTexture(m_texture);
        hdglBegin(GL_POINTS);
        glPointSize(m_particlePointSize);

        hdParticle particle;
        for (int i = 0; i < m_maxParticles; i++)
        {
            particle = m_particles->GetItems()[i];
            hdglColor4ub(particle.color.r, particle.color.g, particle.color.b, (unsigned char)(255 * fabs(particle.life) / m_duration));
            hdglVertex3f(particle.position.x, particle.position.y, particle.position.z);
        }
        hdglEnd();
        
    }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    hdglError("particle emitter end");
}
