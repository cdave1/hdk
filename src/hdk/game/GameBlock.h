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
 A block is a polygon with physics information.

 The block def determines what kind of block will be created. Composite blocks
 have more than one physics shape, although may only have one game object (mainly
 because game objects are to be rendered as textured quads or even pt sprites).
 */

#ifndef _HDK_GAME_BLOCK_H_
#define _HDK_GAME_BLOCK_H_

#include "Box2D.h"

#include <hdk/physics.h>
#include <hdk/graphics/hdDrawable.h>
#include <hdk/graphics/hdPolygon.h>
#include <hdk/game/Enums.h>
#include <hdk/game/Utils.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct hdGameObjectDef;

class Block : public hdDrawable, public hdPhysicsObject, public hdPolygon {
    friend class hdGameWorld;
    friend class hdGameObject;
    friend class hdPolygon;

public:
    Block();

    Block(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               const hdVec2& aa,
               const hdVec2& bb,
               const e_hdkMaterial material,
               const e_hdkShapeType shapeType);

    Block(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               const hdVec2& aa,
               const hdVec2& bb,
               const e_hdkMaterial material,
               const e_hdkShapeType shapeType,
               const e_hdkBlockType blockType);

    Block(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               const hdVec3& center,
               const float halfWidth,
               const float halfHeight,
               const e_hdkMaterial material,
               const e_hdkShapeType shapeType,
               const e_hdkBlockType blockType);

    Block(hdGameWorld *gameWorld,
               b2World *physicsWorld,
               const hdVec2& aa,
               const hdVec2& bb,
               const e_hdkMaterial material,
               const e_hdkShapeType shapeType,
               const e_hdkBlockType blockType,
               const char* textureName);

    ~Block();

    void Init(hdGameWorld *gameWorld);

    void Init(hdGameWorld *gameWorld,
              b2World *physicsWorld);

    void Draw() const;

    void DrawBlock(bool front, const hdVec3& screenCenter);

    void DrawHighlight(const hdVec3& screenCenter, const hdColor4& color);

    void DrawExtruded(hdTexture *texture, bool front, const hdVec3& screenCenter);

    void DrawTrianglesFront();

    void DrawTrianglesBack();

    bool AABBIntersection(const hdAABB& aabb);

    bool RayIntersection(const hdVec3* ray, hdVec3& vOut);

    bool b2BodyIntersection(const hdAABB& aabb, const bool useDepth);

    const hdAABB GetDepthAABB();

    hdVec3* GetVerticesWithDepth();

    virtual void Step();

    void CreatePhysicsBody();

    void SetMaterial(const e_hdkMaterial material);

    void SetTint(const float r, const float g, const float b, const float a);

    const float* GetTint();

    const hdTexture* GetTexture();

    const char* GetTextureName() const;

    void SetTextureName(const char* textureName);

    const e_hdkMaterial GetMaterial();

    const e_hdkBlockType GetBlockType();

    void SetBlockType(const e_hdkBlockType blockType);

    const e_hdkShapeType GetShapeType();

    const bool IsPhysicsActor() const;

    const bool IsDrawable() const;

    const bool IsStatic() const;

    const bool IsHazard() const;

    const bool IsSensor() const;

    const bool IsDestroyable() const;

    const bool IsSpecial() const;

    const bool IsGoal() const;

    const bool IsDraggable() const;

    const bool IsEventSender() const;

    const bool IsTransparent() const;

    const bool IsExtrusion() const;

    const bool IsTextureChangeable() const;

    const float GetArea() const;


    void SetDepth(const float depth);

    const float GetDepth() const;


    void SetZOffset(const float offset);

    const float GetZOffset() const;


    void SetYRotation(const float rotRads);

    const float GetYRotation() const;



    const uint32 GetGameFlag() const;

    const bool ContainsGameFlag(const uint32 flag) const;

    void AddGameFlag(const uint32 flag);

    const bool RemoveGameFlag(const uint32 flag);

    void SetGameFlag(const uint32 flag);

private:
    friend class Level;
    friend class boost::serialization::access;

    void PrivateInit(hdGameWorld *gameWorld,
                     b2World *physicsWorld,
                     const hdVec2& aa,
                     const hdVec2& bb,
                     const e_hdkMaterial material,
                     const e_hdkShapeType shapeType,
                     const e_hdkBlockType blockType);

    void ResetAppearance();

    char m_textureName[kMaxTexturePathSize];

    hdTexture* m_texture;

    float m_colorTint[4];

    hdColor4 m_tintBytes;

    float m_depth;

    float m_yRotation;

    float m_zOffset;

    e_hdkMaterial m_material;

    e_hdkBlockType m_blockType;

    e_hdkShapeType m_shapeType;

    uint32 m_gameFlag;

    hdTypedefList<hdVec3, kMaxPolygonVertices> *m_triStripHullVertices;

    // Extrusion texture co-ord precalc:
    hdVec2 m_extrusionTexCoords[kMaxPolygonVertices << 2];

    bool m_generatedExtrusionTex;

    bool m_drawWithCustomLighting;

    bool m_frameDraw;

    bool m_frameDrawReflect;

    bool m_frameStepRaw;

    bool m_frameStepBillboard;

    bool m_frameStepNormal;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_material;
        ar & m_shapeType;
        ar & m_blockType;

        if (version >= 3)
        {
            ar & m_textureName;
            ar & m_colorTint;
        }
        if (version >= 4)
        {
            ar & m_depth;
        }
        if (version >= 5)
        {
            ar & m_zOffset;
        }
        if (version >= 6)
        {
            ar & m_yRotation;
        }
    }


    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_material;
        ar & m_shapeType;
        ar & m_blockType;

        if (version >= 3)
        {
            ar & m_textureName;
            ar & m_colorTint;
        }
        if (version >= 4)
        {
            ar & m_depth;
        }
        if (version >= 5)
        {
            ar & m_zOffset;
        }
        if (version >= 6)
        {
            ar & m_yRotation;
        }

        m_drawWithCustomLighting = NULL;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

};

BOOST_CLASS_VERSION(Block, 6)


inline const float* Block::GetTint()
{
    return m_colorTint;
}


inline void Block::SetTint(const float r, const float g, const float b, const float a)
{
    m_colorTint[0] = r; m_colorTint[1] = g; m_colorTint[2] = b; m_colorTint[3] = a;
    m_tintBytes.Set(m_colorTint[0] * 255, m_colorTint[1] * 255, m_colorTint[2] * 255, m_colorTint[3] * 255);
}


inline const hdTexture* Block::GetTexture()
{
    return m_texture;
}


inline const char* Block::GetTextureName() const
{
    return m_textureName;
}


inline void Block::SetTextureName(const char* textureName)
{
    if (strlen(textureName) == 0)
    {
        snprintf(m_textureName, kMaxTexturePathSize, "");
    }
    else
    {
        snprintf(m_textureName, kMaxTexturePathSize, "%s", textureName);
    }

    this->ResetAppearance();
}


inline void Block::SetMaterial(const e_hdkMaterial material)
{
    m_material = material;
    this->ResetAppearance();
}


inline const e_hdkMaterial Block::GetMaterial()
{
    return m_material;
}


inline const e_hdkBlockType Block::GetBlockType()
{
    return m_blockType;
}


inline void Block::SetBlockType(const e_hdkBlockType blockType)
{
    m_blockType = blockType;
    this->ResetAppearance();
}


inline const e_hdkShapeType Block::GetShapeType()
{
    return m_shapeType;
}


inline const bool Block::IsPhysicsActor() const
{
    return m_blockType != e_hdkBlockTypeMessage &&
    m_blockType != e_hdkBlockTypeEventSender &&
    m_blockType != e_hdkBlockTypeCosmetic &&
    m_blockType != e_hdkBlockTypeBillboard;
}


inline const bool Block::IsDrawable() const
{
    if (this->IsPhysicsActor() && NULL == m_physicsBody) return false;

    return m_blockType != e_hdkBlockTypeMessage &&
    m_blockType != e_hdkBlockTypeLight &&
    this->IsVisible();
}


inline const bool Block::IsHazard() const
{
    // TODO: deprecate IsHazard in favor of specific materials
    return m_blockType == e_hdkBlockTypeHazard ||
    m_blockType == e_hdkBlockTypeHazardDest;
}


inline const bool Block::IsStatic() const
{
    return m_blockType == e_hdkBlockTypeGoal ||
    m_blockType == e_hdkBlockTypeBoundary ||
    m_blockType == e_hdkBlockTypeGround ||
    m_blockType == e_hdkBlockTypeHazard ||
    m_blockType == e_hdkBlockTypeNormalStatic ||
    m_blockType == e_hdkBlockTypeGenericSensor ||
    m_blockType == e_hdkBlockTypeCosmetic ||
    m_blockType == e_hdkBlockTypeBillboard;
}


inline const bool Block::IsSensor() const
{
    return m_blockType == e_hdkBlockTypeGoal ||
    m_blockType == e_hdkBlockTypeHazard ||
    m_blockType == e_hdkBlockTypeGenericSensor;
}


inline const bool Block::IsDestroyable() const
{
    return m_blockType == e_hdkBlockTypeNormal ||
    m_blockType == e_hdkBlockTypeNormalSpecial ||
    m_blockType == e_hdkBlockTypeNormalStatic;
}


inline const bool Block::IsSpecial() const
{
    return m_blockType == e_hdkBlockTypeNormalSpecial;
}


inline const bool Block::IsGoal() const
{
    return m_blockType == e_hdkBlockTypeGoal;
}


inline const bool Block::IsDraggable() const
{
    return m_blockType == e_hdkBlockTypeDraggable;
}


inline const bool Block::IsEventSender() const
{
    return m_blockType == e_hdkBlockTypeEventSender;
}


inline const bool Block::IsTransparent() const
{
    return m_blockType == e_hdkBlockTypeBillboard || 
    m_material == e_hdkMaterialGlass;
}


inline const bool Block::IsTextureChangeable() const
{
    return m_material == e_hdkMaterialCustomTexture ||
    m_material == e_hdkMaterialJack ||
    m_material == e_hdkMaterialText;
}


inline const bool Block::IsExtrusion() const
{
    return m_blockType != e_hdkBlockTypeBillboard;
}


inline const float Block::GetDepth() const
{
    return m_depth;
}


inline void Block::SetDepth(const float depth)
{
    m_depth = depth;
}


inline const float Block::GetZOffset() const
{
    return m_zOffset;
}



inline void Block::SetYRotation(const float rotRads)
{
    if (m_blockType == e_hdkBlockTypeCosmetic)
    {
        m_yRotation = rotRads;
    }
}


inline const float Block::GetYRotation() const
{
    if (m_blockType != e_hdkBlockTypeCosmetic) return 0.0f;
    return m_yRotation;
}


inline const float Block::GetArea() const
{
    return hdPolygonArea(m_vertices->GetItems(), m_vertices->GetItemCount());
}


inline const uint32 Block::GetGameFlag() const
{
    return m_gameFlag;
}


inline const bool Block::ContainsGameFlag(const uint32 flag) const
{
    return ((m_gameFlag & flag) == flag);
}


inline void Block::AddGameFlag(const uint32 flag)
{
    m_gameFlag |= flag;
}


inline const bool Block::RemoveGameFlag(const uint32 flag)
{
    if (this->ContainsGameFlag(flag))
    {
        m_gameFlag ^= flag;
        return true;
    }
    return false;
}


inline void Block::SetGameFlag(const uint32 flag)
{
    m_gameFlag = flag;
}

#endif
