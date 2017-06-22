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

#ifndef _HDK_GAME_JOINT_H_
#define _HDK_GAME_JOINT_H_

#include "Box2D.h"

#include <hdk/physics.h>

#include "Enums.h"
#include "GameBlock.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class Joint : public hdPhysicsJoint,  public hdDrawable, public hdPolygon
{
    friend class hdPhysicsJoint;

public:
    Joint();

    Joint(hdGameWorld *gameWorld);

    Joint(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               e_hdkJointType jointType,
               const hdVec3& initialPoint,
               Block* firstBody,
               Block* secondBody);

    ~Joint();

    void Init(hdGameWorld *gameWorld,
              b2World *physicsWorld);

    void DestroyJoint();

    void CreateJoint();

    void Step();

    void Draw() const;


    bool AABBIntersection(const hdAABB& aabb);
#ifdef LEVEL_EDITOR
    void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);
#endif

    const e_hdkJointType GetJointType() const;

    const int SetFirstBody(const Block* body);

    const Block* GetFirstBody() const;

    const int SetSecondBody(const Block* body);

    const Block* GetSecondBody() const;

    void SetSecondJointPoint(const hdVec3& point);

    const hdVec3 GetSecondJointPoint() const;

    bool GetJointMotorEnabled() const;

    void SetJointMotorEnabled(const bool enabled);

    bool GetJointLimitEnabled() const;

    const float GetJointMotorSpeed() const;

    void SetJointMotorSpeed(const float speed);


    void SetRevoluteJointProperties(const bool enableMotor,
                                    const bool enableLimit,
                                    const float lowerAngle,
                                    const float upperAngle,
                                    const float maxMotorTorque,
                                    const float motorSpeed);

    const float GetRevoluteJointMaxTorque() const;

    const float GetRevoluteJointLowerAngle() const;

    const float GetRevoluteJointUpperAngle() const;

    void SetPrismaticJointProperties(const bool enableMotor,
                                     const bool enableLimit,
                                     const float motorSpeed,
                                     const float lowerTranslation,
                                     const float upperTranslation,
                                     const float maxMotorForce);

    const float GetPrismaticJointMaxMotorForce() const;

    const float GetPrismaticJointLowerTranslation() const;

    const float GetPrismaticJointUpperTranslation() const;

protected:
    float m_colorTint[4];

    e_hdkJointType m_jointType;

    Block* m_firstTotemBlock;

    Block* m_secondTotemBlock;

    hdVec3 m_secondBodyPoint;

private:
    friend class Level;

    void InternalDrawJoint(float x, float y, float z) const;

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_jointType;
        ar & m_colorTint;

        ar & m_secondBodyPoint.x;
        ar & m_secondBodyPoint.y;
        ar & m_secondBodyPoint.z;

        if (version >= 1)
        {
            ar & m_firstTotemBlock;
            ar & m_secondTotemBlock;
        }

        if (version >= 2)
        {
            ar & m_enableMotor;
            ar & m_enableLimit;
            ar & m_lowerAngle;
            ar & m_upperAngle;
            ar & m_maxMotorTorque;
            ar & m_motorSpeed;
        }

        if (version >= 3)
        {
            ar & m_prismaticLowerTranslation;
            ar & m_prismaticUpperTranslation;
            ar & m_prismaticMaxMotorForce;
        }
    }


    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_jointType;
        ar & m_colorTint;

        float x, y, z;
        ar & x;
        ar & y;
        ar & z;
        m_secondBodyPoint.Set(x,y,z);

        if (version >= 1)
        {
            ar & m_firstTotemBlock;
            ar & m_secondTotemBlock;
        }

        if (version >= 2)
        {
            ar & m_enableMotor;
            ar & m_enableLimit;
            ar & m_lowerAngle;
            ar & m_upperAngle;
            ar & m_maxMotorTorque;
            ar & m_motorSpeed;
        }

        if (version >= 3)
        {
            ar & m_prismaticLowerTranslation;
            ar & m_prismaticUpperTranslation;
            ar & m_prismaticMaxMotorForce;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    bool m_frameDraw;

    bool m_frameDrawReflect;

    // HACK HACK HACK
    // Revolute/Prismatic Joint Properties
    bool m_enableMotor;

    bool m_enableLimit;

    float m_motorSpeed;


    // Revolute properties
    float m_lowerAngle;

    float m_upperAngle;

    float m_maxMotorTorque;


    // Prismatic Properties
    float m_prismaticLowerTranslation;

    float m_prismaticUpperTranslation;

    float m_prismaticMaxMotorForce;
};

BOOST_CLASS_VERSION(Joint, 3)

inline 	const e_hdkJointType Joint::GetJointType() const
{
    return m_jointType;
}


inline const hdVec3 Joint::GetSecondJointPoint() const
{
    return m_secondBodyPoint;
}


inline const Block* Joint::GetFirstBody() const
{
    return m_firstTotemBlock;
}


inline const Block* Joint::GetSecondBody() const
{
    return m_secondTotemBlock;
}


inline bool Joint::GetJointMotorEnabled() const
{
    return m_enableMotor;
}


inline void Joint::SetJointMotorEnabled(const bool enabled)
{
    m_enableMotor = enabled;
}


inline const float Joint::GetJointMotorSpeed() const
{
    return m_motorSpeed;
}


inline void Joint::SetJointMotorSpeed(const float speed)
{
    m_motorSpeed = speed;
}


inline bool Joint::GetJointLimitEnabled() const
{
    return m_enableLimit;
}


inline void Joint::SetRevoluteJointProperties(const bool enableMotor,
                                                   const bool enableLimit,
                                                   const float lowerAngle,
                                                   const float upperAngle,
                                                   const float maxMotorTorque,
                                                   const float motorSpeed)
{
    m_enableMotor = enableMotor;
    m_enableLimit = enableLimit;
    m_motorSpeed = motorSpeed;

    m_lowerAngle = lowerAngle;
    m_upperAngle = upperAngle;
    m_maxMotorTorque = maxMotorTorque;
}


inline const float Joint::GetRevoluteJointMaxTorque() const
{
    return m_maxMotorTorque;
}


inline const float Joint::GetRevoluteJointLowerAngle() const
{
    return m_lowerAngle;
}


inline const float Joint::GetRevoluteJointUpperAngle() const
{
    return m_upperAngle;
}


inline void Joint::SetPrismaticJointProperties(const bool enableMotor, 
                                                    const bool enableLimit,
                                                    const float motorSpeed,
                                                    const float lowerTranslation,
                                                    const float upperTranslation,
                                                    const float maxMotorForce)
{
    m_enableMotor = enableMotor;
    m_enableLimit = enableLimit;
    m_motorSpeed = motorSpeed;
    
    m_prismaticLowerTranslation = lowerTranslation;
    m_prismaticUpperTranslation = upperTranslation;
    m_prismaticMaxMotorForce = maxMotorForce;
}


inline const float Joint::GetPrismaticJointMaxMotorForce() const
{
    return m_prismaticMaxMotorForce;
}


inline const float Joint::GetPrismaticJointLowerTranslation() const
{
    return m_prismaticLowerTranslation;
}


inline const float Joint::GetPrismaticJointUpperTranslation() const
{
    return m_prismaticUpperTranslation;
}

#endif
