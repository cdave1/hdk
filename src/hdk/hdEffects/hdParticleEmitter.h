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

/*
 * Responsible for drawing particles of a particular texture
 * at a particular point on the screen.
 *
 * Has a game object to determine position.
 *
 * Has an animation object to handle duration/cancel etc.
 *
 * Emitter should actually be deleted once the animation is finished.
 *
 * has a texture name, total,
 */

#ifndef _HD_PARTICLE_EMITTER_H_
#define _HD_PARTICLE_EMITTER_H_

#include <hdk/hdk.h>

#define kMaxParticles 256

class hdAnimationController;
class hdAnimation;

struct hdParticle
{
    hdParticle() :
        position(),
        direction(),
        rotationDirection(),
        scaleDirection(1,1,1),
        life(0.0),
        color(1.0f,1.0f,1.0f,1.0f) {}

    hdVec3 position;
    hdVec3 direction;
    hdVec3 rotationDirection;
    hdVec3 scaleDirection;
    hdVec3 vertices[4];
    hdTimeInterval life;
    hdColor4 color;
    float damping;
};


typedef enum
{
    e_particleBlendStyleNormal = 0,
    e_particleBlendStyleMultiply,
    e_particleBlendStyleDarken,
} e_particleBlendStyle;



class hdParticleEmitter : public hdDrawable
{
public:
    hdParticleEmitter(int maxParticles,
                      const char * texture,
                      float particlePointSize,
                      float duration);

    hdParticleEmitter(int maxParticles,
                      float particlePointSize,
                      float duration);

    hdParticleEmitter(int maxParticles,
                      float particlePointSize,
                      float duration,
                      e_particleBlendStyle style);

    ~hdParticleEmitter();

    void Start(const hdVec3& pos);

    void Start(float x, float y);

    void Start(const hdAABB& aabb);

    void Start(const hdTexture *texture, float x, float y);

    void Start(const hdTexture *texture, const hdAABB& aabb);

    void Stop();

    virtual void Step(hdTimeInterval elapsed) = 0;

    static void AnimationFinishedCallback(void *object, hdAnimation *anim);

    void SetParticlePointSize(const float pointSize);

    void SetParticleBlendStyle(const e_particleBlendStyle style);

    void Draw() const;

    void SetNextParticleCount(const int count);

    hdVec3 m_position;

    hdAABB m_aabb;

protected:
    virtual void ParticleAnimationFinished() = 0;

    virtual void DoStartingTasks() {}

    virtual const bool IsFixedDuration() const;

    virtual void MakeParticle(int pos) = 0;

    const float GetRemainingTime() const;

    /*
     * The function action will be rewound to the start.
     */
    void ExtendTimeStampForStop();

    hdTypedefList<hdParticle, kMaxParticles>* m_particles;

    int m_maxParticles;

    hdTexture* m_texture;

    float m_particlePointSize;

    float m_duration;

    bool m_drawAsQuad;

    e_particleBlendStyle m_blendStyle;

    int m_nextParticleCount;

private:
    void Init();

    hdAnimation* m_animation;

    hdFunctionAction<hdParticleEmitter>* m_functionAction;

};


inline const float hdParticleEmitter::GetRemainingTime() const
{
    return m_duration - m_functionAction->GetProgress();
}


inline void hdParticleEmitter::SetParticlePointSize(const float pointSize)
{
    m_particlePointSize = pointSize;
}


inline void hdParticleEmitter::SetParticleBlendStyle(const e_particleBlendStyle style)
{
    m_blendStyle = style;
}


inline void hdParticleEmitter::SetNextParticleCount(const int count)
{
    m_nextParticleCount = count;
}

#endif
