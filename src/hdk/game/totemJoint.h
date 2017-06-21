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

#ifndef _TOTEM_JOINT_H_
#define _TOTEM_JOINT_H_

#include "Box2D.h"

#include "totemEnums.h"
#include "totemBlock.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class totemJoint : public hdPhysicsJoint,  public hdDrawable, public hdPolygon
{
    friend class hdPhysicsJoint;

public:
    totemJoint();

    totemJoint(hdGameWorld *gameWorld);

    totemJoint(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               e_totemJointType jointType,
               const hdVec3& initialPoint,
               totemBlock* firstBody,
               totemBlock* secondBody);

    ~totemJoint();

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

    const e_totemJointType GetJointType() const;

    const int SetFirstBody(const totemBlock* body);

    const totemBlock* GetFirstBody() const;

    const int SetSecondBody(const totemBlock* body);

    const totemBlock* GetSecondBody() const;

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

    e_totemJointType m_jointType;

    totemBlock* m_firstTotemBlock;

    totemBlock* m_secondTotemBlock;

    hdVec3 m_secondBodyPoint;

private:
    friend class totemLevel;

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

BOOST_CLASS_VERSION(totemJoint, 3)

inline 	const e_totemJointType totemJoint::GetJointType() const
{
    return m_jointType;
}


inline const hdVec3 totemJoint::GetSecondJointPoint() const
{
    return m_secondBodyPoint;
}


inline const totemBlock* totemJoint::GetFirstBody() const
{
    return m_firstTotemBlock;
}


inline const totemBlock* totemJoint::GetSecondBody() const
{
    return m_secondTotemBlock;
}


inline bool totemJoint::GetJointMotorEnabled() const
{
    return m_enableMotor;
}


inline void totemJoint::SetJointMotorEnabled(const bool enabled)
{
    m_enableMotor = enabled;
}


inline const float totemJoint::GetJointMotorSpeed() const
{
    return m_motorSpeed;
}


inline void totemJoint::SetJointMotorSpeed(const float speed)
{
    m_motorSpeed = speed;
}


inline bool totemJoint::GetJointLimitEnabled() const
{
    return m_enableLimit;
}


inline void totemJoint::SetRevoluteJointProperties(const bool enableMotor,
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


inline const float totemJoint::GetRevoluteJointMaxTorque() const
{
    return m_maxMotorTorque;
}


inline const float totemJoint::GetRevoluteJointLowerAngle() const
{
    return m_lowerAngle;
}


inline const float totemJoint::GetRevoluteJointUpperAngle() const
{
    return m_upperAngle;
}


inline void totemJoint::SetPrismaticJointProperties(const bool enableMotor, 
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


inline const float totemJoint::GetPrismaticJointMaxMotorForce() const
{
    return m_prismaticMaxMotorForce;
}


inline const float totemJoint::GetPrismaticJointLowerTranslation() const
{
    return m_prismaticLowerTranslation;
}


inline const float totemJoint::GetPrismaticJointUpperTranslation() const
{
    return m_prismaticUpperTranslation;
}

#endif
