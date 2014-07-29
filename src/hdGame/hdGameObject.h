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
 * A game object is a vertex store that will be used when the game engine renders a scene.
 *
 * Raw game objects are not meant to be rendered directly - sub classes of it must directly
 * implement the rendering functions.
 *
 * Game object has
 * - position (x,y,z)
 * - scale (xScale,yScale,ZScale)
 * - volume: GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar
 * - angle (xAngle, yAngle, zAngle).
 *
 * Drawable Game object:
 * - vertices
 * - vertex texture
 * - colour/alpha
 * - texture pointer
 * - texture scale (vec)
 * - texture offset
 *
 * Game objects exist within a world. Individual game objects
 * do not care about the world or any other game objects at this level.
 * Specific game object implementations may worry about this.
 *
 * The hdAnimationEngine applies animation functions to properties of
 * game objects.
 */

#ifndef hd_GAMEOBJECT_H
#define hd_GAMEOBJECT_H

#include "hdMath.h"
#include "hdGame/hdGameWorld.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct hdGameObjectDef
{
    hdGameObjectDef()
    {
        hidden = false;
        name = 0;
        userType = 0;
    }
    bool hidden;
    int name;
    int userType;
};

class hdGameWorld;

class hdGameObject
{
public:
    friend class hdGameWorld;

    hdGameObject();

    hdGameObject(hdGameWorld* world);

    hdGameObject(hdGameWorld* world, const hdAABB& aabb);

    virtual ~hdGameObject();

    void Init(hdGameWorld *gameWorld);

    const bool IsHidden() const;

    const bool IsVisible() const;

    void Hide();

    void Show();

    const int GetName() const;

    void SetName(int name);

    const int GetUserType() const;

    const int GetTag() const;

    void SetTag(const int tag);

    const hdAABB GetAABB();

    const hdAABB ResetAndGetAABB();

    const hdOBB& GetOBB() const;

    const hdOBB& ResetAndGetOBB();

    hdTransform& GetTransform();

    const hdVec3 GetWorldCenter();

    void ResetAABB();

    virtual void ResetOBB() = 0;

    virtual void ApplyTransform(const hdTransform& transform) = 0;

    virtual hdVec3* GetVertices() = 0;

    virtual const int GetVertexCount() const = 0;

    virtual void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) = 0;

    virtual void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) = 0;

    virtual void Translate(const hdVec3& translationVector, const hdVec3& translationAxis);

    /// Get the next body in the world's body list.
    hdGameObject* GetNext();

    /// Get the parent world of this body.
    hdGameWorld* GetWorld() const;

    void SetWorld(const hdGameWorld* world);

    const float GetAlpha();

    void SetAlpha(const float alpha);

    // Hit test
    //bool ContainsPoint(const hdVec3& point);
    virtual bool AABBIntersection(const hdAABB& aabb);

    // Ray intersection - ray start point, and the actual intersection point.
    virtual bool RayIntersection(const hdVec3* ray, hdVec3& vOut);


protected:

    hdGameObject* m_next;

    hdGameObject* m_prev;

    int m_name;

    bool m_isHidden;

    int m_userType;

    int m_tag;

    hdGameWorld* m_world;

    hdAABB m_aabb;

    hdOBB m_obb;

    float m_alpha;

    hdGameObjectDef GetCurrentDefinition() const;

    const hdGameObjectDef GetDefinition();

    hdGameObjectDef m_def;


private:

    friend class boost::serialization::access;

    // Loading and saving
    //
    // loading must retrieve the def and then initialize
    // the block via the def.
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_userType;
        ar & m_isHidden;
        ar & m_name;

        ar & m_aabb.lower.x;
        ar & m_aabb.lower.y;
        ar & m_aabb.lower.z;

        ar & m_aabb.upper.x;
        ar & m_aabb.upper.y;
        ar & m_aabb.upper.z;

        if (version >= 1)
        {
            ar & m_alpha;
        }

        if (version >= 2)
        {
            ar & m_tag;
        }
    }


    // Loading and saving
    //
    // loading must retrieve the def and then initialize
    // the block via the def.
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_userType;
        ar & m_isHidden;
        ar & m_name;

        float32 x, y, z;
        ar & x;
        ar & y;
        ar & z;
        m_aabb.lower = hdVec3(x,y,z);
        ar & x;
        ar & y;
        ar & z;
        m_aabb.upper = hdVec3(x,y,z);

        if (version >= 1)
        {
            ar & m_alpha;
        }

        if (version >= 2)
        {
            ar & m_tag;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_VERSION(hdGameObject, 2)


inline const int32 hdGameObject::GetName() const
{
    return m_name;
}


inline void hdGameObject::SetName(int name)
{
    m_name = name;
}


inline const bool hdGameObject::IsHidden() const
{
    return m_isHidden;
}


inline const bool hdGameObject::IsVisible() const
{
    return !m_isHidden;
}


inline hdGameObject* hdGameObject::GetNext()
{
    return m_next;
}


inline hdGameWorld* hdGameObject::GetWorld() const
{
    return m_world;
}


inline void hdGameObject::SetWorld(const hdGameWorld* world)
{
    m_world = (hdGameWorld *)world;
}


inline const hdGameObjectDef hdGameObject::GetDefinition()
{
    return m_def;
}


inline const float hdGameObject::GetAlpha()
{
    return m_alpha;
}


inline void hdGameObject::SetAlpha(const float alpha)
{
    m_alpha = alpha;
}



inline void hdGameObject::Hide()
{
    m_isHidden = true;
}


inline void hdGameObject::Show()
{
    m_isHidden = false;
}


inline const int hdGameObject::GetUserType() const
{
    return m_userType;
}


inline const int hdGameObject::GetTag() const
{
    return m_tag;
}


inline void hdGameObject::SetTag(const int tag)
{
    m_tag = tag;
}


inline const hdAABB hdGameObject::GetAABB()
{
    return m_aabb;
}


inline const hdAABB hdGameObject::ResetAndGetAABB()
{
    this->ResetAABB();
    return m_aabb;
}


inline const hdOBB& hdGameObject::GetOBB() const
{
    return m_obb;
}


inline const hdOBB& hdGameObject::ResetAndGetOBB()
{
    this->ResetOBB();
    return m_obb;
}


inline hdTransform& hdGameObject::GetTransform()
{
    return m_obb.transform;
}


inline const hdVec3 hdGameObject::GetWorldCenter()
{
    return m_aabb.lower + (0.5f * (m_aabb.upper - m_aabb.lower));
}

#endif