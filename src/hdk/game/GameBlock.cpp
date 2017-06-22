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

#include "GameBlock.h"

#include <hdk/core/hdConfig.h>

/*
 * Thanks to boost serialization, there are multiple entry points to the Block init functions.
 *
 * This is the reason for the extra "isDrawable" test in the destructor when deleting
 * m_triStripHullVertices.
 *
 * TODO: Fix this.
 */
Block::Block() : hdPolygon()
{
    m_userType = (int)e_hdkTypeBlock;

    snprintf(m_textureName, kMaxTexturePathSize, "%c", '\0');

    m_texture = NULL;
    m_physicsBody = NULL;
    m_shapeType = e_hdkShapeTypeRectangle;
    m_blockType = e_hdkBlockTypeNormal;

    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 1.0f;
    m_colorTint[3] = 1.0f;

    m_depth = -0.25f;
    m_zOffset = 0.0f;

    m_gameFlag = 0;

    m_yRotation = 0.0f;

    m_triStripHullVertices = NULL;

    m_generatedExtrusionTex = false;

    if ("Lowest" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY) ||
        "Low" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY))
    {
        m_drawWithCustomLighting = false;
    }
    else
    {
        m_drawWithCustomLighting = true;
    }
}


Block::Block(hdGameWorld *gameWorld,
                       b2World *physicsWorld,
                       const hdVec2& aa,
                       const hdVec2& bb,
                       const e_hdkMaterial material,
                       const e_hdkShapeType shapeType) : hdPolygon(gameWorld)
{
    this->PrivateInit(gameWorld, physicsWorld, aa, bb, material, shapeType, e_hdkBlockTypeNormal);
}


Block::Block(hdGameWorld *gameWorld,
                       b2World *physicsWorld,
                       const hdVec2& aa,
                       const hdVec2& bb,
                       const e_hdkMaterial material,
                       const e_hdkShapeType shapeType,
                       const e_hdkBlockType blockType) : hdPolygon(gameWorld)
{
    this->PrivateInit(gameWorld, physicsWorld, aa, bb, material, shapeType, blockType);
}


Block::Block(hdGameWorld *gameWorld,
                       b2World *physicsWorld,
                       const hdVec3& center,
                       const float halfWidth,
                       const float halfHeight,
                       const e_hdkMaterial material,
                       const e_hdkShapeType shapeType,
                       const e_hdkBlockType blockType) : hdPolygon(gameWorld)
{
    hdVec2 aa(center.x - halfWidth, center.y - halfHeight);
    hdVec2 bb(center.x + halfWidth, center.y + halfHeight);

    this->PrivateInit(gameWorld, physicsWorld, aa, bb, material, shapeType, blockType);
}


Block::Block(hdGameWorld *gameWorld,
                       b2World *physicsWorld,
                       const hdVec2& aa,
                       const hdVec2& bb,
                       const e_hdkMaterial material,
                       const e_hdkShapeType shapeType,
                       const e_hdkBlockType blockType,
                       const char* textureName) : hdPolygon(gameWorld)
{
    this->PrivateInit(gameWorld, physicsWorld, aa, bb, material, shapeType, blockType);

    snprintf(m_textureName, kMaxTexturePathSize, "%s", textureName);
    m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
}


void Block::PrivateInit(hdGameWorld *gameWorld,
                             b2World *physicsWorld,
                             const hdVec2& aa,
                             const hdVec2& bb,
                             const e_hdkMaterial material,
                             const e_hdkShapeType shapeType,
                             const e_hdkBlockType blockType)
{
    m_textureName[0] = 0;
    m_texture = NULL;
    m_userType = (int)e_hdkTypeBlock;
    m_depth = -0.3f;
    m_physicsBody = NULL;
    m_gameFlag = 0;
    m_zOffset = 0.0f;
    m_yRotation = 0.0f;
    hdVec2 boundingBox;
    m_shapeType = shapeType;
    m_blockType = blockType;
    this->SetMaterial(material);

    m_triStripHullVertices = NULL;

    if ("Lowest" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY) ||
        "Low" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY))
    {
        m_drawWithCustomLighting = false;
    }
    else
    {
        m_drawWithCustomLighting = true;
    }

    if (shapeType == e_hdkShapeTypeRectangle)
    {
        this->SetAs2DBox(aa, bb);
    }
    else
    {
        // Square bounding box
        hdVec2 boundingBox = bb - aa;
        float sideLength = hdMax(boundingBox.x, boundingBox.y);
        hdVec2 _bb = aa + hdVec2(sideLength, sideLength);

        if (shapeType == e_hdkShapeTypeCylinder)
        {
            this->SetAs2DCylinder(aa, _bb, sideLength, 12);
        }
        else if (shapeType == e_hdkShapeTypeStar)
        {
            this->SetAs2DStar(aa, _bb, sideLength, sideLength * 0.4f, 5);
        }
        else if (shapeType == e_hdkShapeTypeMultiStar)
        {
            // Number of points is the sidelength / 1.0f
            this->SetAs2DStar(aa, _bb, sideLength, sideLength * 0.75f,
                              hdClamp((int)ceil(sideLength * 2.0f), 4, 24));
        }
        else if (shapeType == e_hdkShapeTypeCustom
                 || shapeType == e_hdkShapeTypeTriStrip)
        {
            this->AddPoint(aa.x, aa.y);
        }
        else if (shapeType == e_hdkShapeTypeSpikes)
        {
            // 8 spikes along the x, with height of bb.y - aa.y
            for (float x = aa.x; x < bb.x; x += (bb.x - aa.x)/8.0f)
            {
                this->AddPoint(x, aa.y);
                this->AddPoint(x + ((bb.x - aa.x)/8.0f)/2.0f, bb.y);
                this->AddPoint(x + (bb.x - aa.x)/8.0f, aa.y);
            }
            ((hdGameObject *)this)->ResetAABB();
            this->ResetTextureCoords();
        }
        else if (shapeType == e_hdkShapeTypeHexagon)
        {
            this->SetAs2DCylinder(aa, _bb, sideLength, 6);
        }
        else if (shapeType == e_hdkShapeTypeAABBCylinder)
        {
            // aa bb width - cylinder diameter.
            this->SetAs2DCylinder(aa, _bb, sideLength, 12);
        }
    }

    this->Init(gameWorld, physicsWorld);
}


void Block::Init(hdGameWorld *gameWorld)
{
    m_triStripHullVertices = NULL;
    ((hdPolygon *)this)->Init(gameWorld);
}


void Block::Init(hdGameWorld *gameWorld,
                      b2World *physicsWorld)
{

    ((hdPolygon *)this)->Init(gameWorld);
    m_userType = (int)e_hdkTypeBlock;
    m_physicsWorld = physicsWorld;
    m_triStripHullVertices = NULL;

    if ("Lowest" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY) ||
        "Low" == hdConfig::GetValueForKey(CONFIG_LIGHTINGLEVEL_KEY))
    {
        m_drawWithCustomLighting = false;
    }
    else
    {
        m_drawWithCustomLighting = true;
    }

    if (this->GetMaterial() == e_hdkMaterialGrinder)
    {
        m_drawWithCustomLighting = false;
    }

    this->ResetAABB();
    this->ResetOBB();

    this->CreatePhysicsBody();

    this->ResetAppearance();
}


Block::~Block()
{
    // HACK HACK HACK
    // For some reason, m_triStripHullVertices goes away if there is no texture.
    // TODO: Fix this, obviously.
    if (this->IsDrawable())
    {
        if (m_triStripHullVertices != NULL && m_texture != NULL)
            delete m_triStripHullVertices;
    }
}


void Block::Step()
{
    if (this->IsPhysicsActor() == false) return;
    /* Static bodies do not translate or rotate */
    if (this->IsStatic()) return;
    if (m_physicsBody == NULL) return;

    b2XForm xform = m_physicsBody->GetXForm();

    // Translate to origin, rotate, translate back again.
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);

    hdRotateVertices(this->GetVertices(), this->GetVertexCount(), hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()) - m_obb.transform.rotation);

    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (hdVec3(xform.position.x, xform.position.y, m_zOffset) - m_obb.transform.translation));

    // Tri strip? Rotate hull vertices
    if (m_shapeType == e_hdkShapeTypeTriStrip)
    {
        // Triangle strip vertices must be arranged into a hull
        if (m_triStripHullVertices != NULL)
        {
            hdTranslateVertices(m_triStripHullVertices->GetItems(),
                                m_triStripHullVertices->GetItemCount(),
                                -m_obb.transform.translation);

            hdRotateVertices(m_triStripHullVertices->GetItems(),
                             m_triStripHullVertices->GetItemCount(),
                             hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()) - m_obb.transform.rotation);

            hdTranslateVertices(m_triStripHullVertices->GetItems(),
                                m_triStripHullVertices->GetItemCount(),
                                m_obb.transform.translation + (hdVec3(xform.position.x, xform.position.y, m_zOffset) - m_obb.transform.translation));

        }
    }

    m_obb.transform.translation = hdVec3(xform.position.x, xform.position.y, m_zOffset);
    m_obb.transform.rotation =  hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle());

    this->ResetAABB();
}


void Block::SetZOffset(const float offset)
{
    // All vertices have the same z offset
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        this->GetVertices()[i].Set(this->GetVertices()[i].x,
                                   this->GetVertices()[i].y,
                                   offset);
        this->GetStartingVertices()[i].Set(this->GetStartingVertices()[i].x,
                                           this->GetStartingVertices()[i].y,
                                           offset);
    }

    this->ResetAABB();

    m_zOffset = offset;
    m_obb.transform.translation = hdVec3(m_obb.transform.translation.x, m_obb.transform.translation.y, m_zOffset);
}


void Block::CreatePhysicsBody()
{
    if (this->GetVertexCount() <= 2) return;
    if (this->IsPhysicsActor() == false) return;

    hdVec3 box = (m_aabb.upper - m_aabb.lower);
    hdVec3 center = m_aabb.lower + (0.5 * box);

    float friction = 0.7f;
    float density = 2.5f;
    float restitution = 0.15f;

    if (m_material == e_hdkMaterialMetal)
    {
        friction = 0.1f;
        density = 5.0f;
    }
    else if (m_material == e_hdkMaterialWood)
    {
        friction = 0.6f;
    }
    else if (m_material == e_hdkMaterialIce)
    {
        friction = 0.1f;
    }
    else if (m_material == e_hdkMaterialBomb)
    {
        restitution = 0.6f;
        friction = 0.25f;
    }
    else if (m_material == e_hdkMaterialJack)
    {
        friction = 0.1f;
        density = 5.0f;
    }

    if (m_blockType == e_hdkBlockTypeDraggable)
    {
        friction = 0.5f;
        density = 4.0f;
        restitution = 0.5f;
    }

    if (m_material == e_hdkMaterialBouncyBomb)
    {
        restitution = 0.8f;
        friction = 0.1f;
        density = 0.01f;
    }

    int maskBits = 0x02;
    int categoryBits = 0x02;

#ifdef DEBUG
    for (int i = 0; i < m_vertices->GetItemCount(); ++i)
    {
        printf("Vertex: %3.5f, %3.5f, %3.5f\n", m_vertices->GetItems()[i].x, m_vertices->GetItems()[i].y, m_vertices->GetItems()[i].z);
    }
#endif

    if (m_shapeType == e_hdkShapeTypeRectangle)
    {
        hdMake2DSingleShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                       this->GetVertices(),
                                       this->GetVertexCount(),
                                       friction, density, 0.25f,
                                       this->IsStatic(),
                                       this->IsSensor(),
                                       0, categoryBits, maskBits);
    }
    else if (m_shapeType == e_hdkShapeTypeCylinder)
    {
        h2CirclePhysicsBody(m_physicsWorld, &m_physicsBody, center, box.x / 2.0f,
                            friction, density,
                            this->IsStatic(),
                            this->IsSensor(),
                            0, categoryBits, maskBits);
    }
    else if (m_shapeType == e_hdkShapeTypeHexagon)
    {
        hdMake2DSingleShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                       this->GetVertices(),
                                       this->GetVertexCount(),
                                       friction, density, 0.25f,
                                       this->IsStatic(),
                                       this->IsSensor(),
                                       0, categoryBits, maskBits);
    }
    else if (m_shapeType == e_hdkShapeTypeStar || m_shapeType == e_hdkShapeTypeMultiStar)
    {
        if (this->GetVertexCount() > 14)
        {
            h2CirclePhysicsBody2(m_physicsWorld, &m_physicsBody, center, box.x / 2.0f,
                                 friction, density, restitution,
                                 this->IsStatic(),
                                 this->IsSensor(),
                                 0, categoryBits, maskBits);
        }
        else
        {

            hd2DMultiShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                      this->GetVertices(),
                                      this->GetVertexCount(),
                                      friction, density, restitution,
                                      this->IsStatic(),
                                      this->IsSensor(),
                                      0, categoryBits, maskBits);
        }
    }
    else if (m_shapeType == e_hdkShapeTypeSpikes)
    {
        hdVec3 tmp[4];
        tmp[0] = this->GetVertices()[0];
        tmp[1] = this->GetVertices()[1];
        tmp[2] = this->GetVertices()[this->GetVertexCount()-2];
        tmp[3] = this->GetVertices()[this->GetVertexCount()-1];

        hd2DMultiShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                  tmp,
                                  4,
                                  friction, density, restitution,
                                  this->IsStatic(),
                                  this->IsSensor(),
                                  0, categoryBits, maskBits);
    }
    else if (m_shapeType == e_hdkShapeTypeTriStrip)
    {
        hd2DTriangleStripPhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                     this->GetVertices(),
                                     this->GetVertexCount(),
                                     friction, density,
                                     this->IsStatic(),
                                     this->IsSensor(),
                                     0, categoryBits, maskBits);
    }
    else if (m_shapeType == e_hdkShapeTypeCustom)
    {
        if (this->GetVertexCount() < b2_maxPolygonVertices)
        {
            hdMake2DSingleShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                           this->GetVertices(),
                                           this->GetVertexCount(),
                                           friction, density, 0.25f,
                                           this->IsStatic(),
                                           this->IsSensor(),
                                           0, categoryBits, maskBits);
        }
        else
        {
            hd2DMultiShapePhysicsBody(m_physicsWorld, &m_physicsBody, center,
                                      this->GetVertices(),
                                      this->GetVertexCount(),
                                      friction, density, restitution,
                                      this->IsStatic(),
                                      this->IsSensor(),
                                      0, categoryBits, maskBits);
        }
    }

    if (m_physicsBody != NULL)
    {
        m_physicsBody->SetUserData((hdGameObject *)this);

        if (this->IsDraggable() || m_material != e_hdkMaterialGrinder)
        {
            m_physicsBody->SetBullet(true);
        }
    }
}


void Block::ResetAppearance()
{
    m_texture = NULL;

    if (m_blockType == e_hdkBlockTypeNormal
        || m_blockType == e_hdkBlockTypeNormalSpecial
        || m_blockType == e_hdkBlockTypeNormalStatic
        || m_blockType == e_hdkBlockTypeGround
        || m_blockType == e_hdkBlockTypeDraggable
        || m_blockType == e_hdkBlockTypeGenericSensor
        || m_blockType == e_hdkBlockTypeNormalInv
        || m_blockType == e_hdkBlockTypeEventSender
        || m_blockType == e_hdkBlockTypeMessage
        || m_blockType == e_hdkBlockTypeCosmetic
        || m_blockType == e_hdkBlockTypeBillboard)
    {
        switch(m_material)
        {
            case e_hdkMaterialWood:
                this->SetTint(hdRandom(0.9f, 1.0f), hdRandom(0.85f, 0.95f), hdRandom(0.5f, 0.6f), 1.0f);
                if (rand() % 3 == 0)
                {
                    m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_wood01_128.tga", TT_16Wall);
                }
                else if (rand() % 3 == 1)
                {
                    m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_wood05_128.png", TT_16Wall);
                }
                else
                {
                    m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_wood06_128.png", TT_16Wall);
                }
                break;
            case e_hdkMaterialMetal:
                this->SetTint(hdRandom(0.8f, 0.9f), hdRandom(0.75f, 0.95f), hdRandom(0.95f, 1.0f), 1.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_metal01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialIce:
                this->SetTint(hdRandom(0.9f, 0.95f), hdRandom(0.95f, 0.975f), hdRandom(0.975f, 1.0f), 1.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_ice01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialEarth:
                this->SetTint(102.0f/255.0f, 51.0f/255.0f, 0.0f, 1.0f);
                break;
            case e_hdkMaterialGlass:
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_glass01_128.png", TT_16Wall);
                this->SetTint(hdRandom(0.6f, 0.8f), hdRandom(0.8f, 0.9f), hdRandom(0.9f, 1.0f), 0.8f);
                break;
            case e_hdkMaterialRawPolygon:
                if (m_texture == NULL)
                {
                    if (strlen(m_textureName) > 0)
                    {
                        m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_16Wall);
                    }
                }
                break;
            case e_hdkMaterialToyWood:
                UtilGenerateColor(m_colorTint, hdRandom(0.0f, 20.0f), 20.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_toywood01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialToyPlastic:
                UtilGenerateColor(m_colorTint, hdRandom(0.0f, 20.0f), 20.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/nomaterial.tga", TT_16Wall);
                break;
            case e_hdkMaterialToyMetal:
                UtilGenerateColor(m_colorTint, hdRandom(0.0f, 20.0f), 20.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/nomaterial.tga", TT_16Wall);
                break;
            case e_hdkMaterialToyRubber:
                UtilGenerateColor(m_colorTint, hdRandom(0.0f, 20.0f), 20.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/nomaterial.tga", TT_16Wall);
                break;
            case e_hdkMaterialPlastic:
                this->SetTint(hdRandom(0.6f, 0.8f), hdRandom(0.8f, 0.9f), hdRandom(0.9f, 1.0f), 1.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_plastic01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialBouncyBomb:
            case e_hdkMaterialBomb:
                this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_bomb01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialGrinder:
                this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
                m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_grinder01_128.tga", TT_16Wall);
                break;
            case e_hdkMaterialJack:
                this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
                if (m_texture == NULL)
                {
                    if (strlen(m_textureName) > 0)
                    {
                        m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_16Wall);
                    }
                    else
                    {
                        m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_jack01_128.tga", TT_16Wall);

                    }
                }
                break;
            case e_hdkMaterialText:
            case e_hdkMaterialCustomTexture:
                if (m_texture == NULL)
                {
                    if (strlen(m_textureName) > 0)
                    {
                        if (m_blockType == e_hdkBlockTypeBillboard)
                        {
#ifdef LEVEL_EDITOR
                            m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
#else
                            m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
#endif
                        }
                        else
                        {
                            m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_16Wall);
                        }
                    }
                }
                this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            default:
                this->SetTint(0.0f, 0.5f, 1.0f, 1.0f);
                break;
        }
    }
    else if (m_blockType == e_hdkBlockTypeGoal)
    {
        m_texture = hdTextureManager::Instance()->FindTexture("Materials/c_yel14_64.tga", TT_16Wall);
        this->SetTint(1.0f, 1.0f, 0.5f, 0.75f);
    }
    else if (m_blockType == e_hdkBlockTypeBoundary)
    {
        this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if (m_blockType == e_hdkBlockTypeHazard || m_blockType == e_hdkBlockTypeHazardDest)
    {
        m_texture = hdTextureManager::Instance()->FindTexture("Materials/m_hazard01_128.tga", TT_16Wall);
        this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        m_texture = hdTextureManager::Instance()->FindTexture("Materials/nomaterial.tga", TT_16Wall);
        this->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Tint depth modifier:
    //
    // Tint should darken the further away it is from the camera.
    //
    // Modifier: - zoffset + m_depth effects this.
    // if zoffset + abs(m_depth) > 0.5, tint[i] *= (1.0 + (0.1 * (zoffset + abs(m_depth))))
    //
    // Between 0 and 0.5 tint is 0.8f;

#ifndef LEVEL_EDITOR
    float tintModifier;
    if (m_zOffset + fabs(m_depth) > 1.0f)
    {
        tintModifier = 1.0f + (0.2f * (m_zOffset + fabs(m_depth)));
        m_colorTint[0] = hdClamp(m_colorTint[0] * tintModifier, 0.0f, 1.0f);
        m_colorTint[1] = hdClamp(m_colorTint[1] * tintModifier, 0.0f, 1.0f);
        m_colorTint[2] = hdClamp(m_colorTint[2] * tintModifier, 0.0f, 1.0f);
    }
    else if (m_zOffset + fabs(m_depth) <= 0.0f)
    {
        float mod = 1.0f - (0.15f * fabs(m_zOffset + fabs(m_depth)));
        tintModifier = hdMax(0.8f, mod);

        m_colorTint[0] = hdClamp(m_colorTint[0] * tintModifier, 0.4f, 1.0f);
        m_colorTint[1] = hdClamp(m_colorTint[1] * tintModifier, 0.4f, 1.0f);
        m_colorTint[2] = hdClamp(m_colorTint[2] * tintModifier, 0.4f, 1.0f);
    }
    else if (m_material == e_hdkMaterialCustomTexture)
    {
        m_colorTint[0] = 0.85f;
        m_colorTint[1] = 0.85f;
        m_colorTint[2] = 0.85f;
    }

    m_tintBytes.Set(m_colorTint[0] * 255, m_colorTint[1] * 255, m_colorTint[2] * 255, m_colorTint[3] * 255);
#endif
}


const hdAABB Block::GetDepthAABB()
{
    hdAABB depthAABB;
    depthAABB.lower.Set(m_aabb.lower.x, m_aabb.lower.y, m_aabb.lower.z + m_depth);
    depthAABB.upper.Set(m_aabb.upper.x, m_aabb.upper.y, m_aabb.upper.z - m_depth);
    return depthAABB;
}


bool Block::AABBIntersection(const hdAABB& aabb)
{
    hdAABB depthAABB;
    depthAABB.lower.Set(m_aabb.lower.x, m_aabb.lower.y, m_aabb.lower.z + m_depth);
    depthAABB.upper.Set(m_aabb.upper.x, m_aabb.upper.y, m_aabb.upper.z - m_depth);
    return hdAABBIntersection(aabb, depthAABB);
}


bool Block::b2BodyIntersection(const hdAABB& aabb, const bool useDepth)
{
    hdAABB depthAABB;
    float depthOffset;

    depthOffset = (useDepth) ? fabs(m_depth) * 2.0f : 0.0f;

    if (this->IsPhysicsActor() && !this->IsStatic())
    {
        if (m_physicsBody == NULL) return false;

        depthOffset += fabs(m_zOffset);

        hdVec3 box = (0.5 * (m_aabb.upper - m_aabb.lower));
        b2XForm xform = m_physicsBody->GetXForm();

        depthAABB.lower.Set(xform.position.x - box.x - depthOffset,
                            xform.position.y - box.y - depthOffset,
                            m_aabb.lower.z + m_depth);
        depthAABB.upper.Set(xform.position.x + box.x + depthOffset,
                            xform.position.y + box.y + depthOffset,
                            m_aabb.upper.z - m_depth);

        return hdAABBIntersection(aabb, depthAABB);
    }
    else
    {
        depthOffset += fabs(m_zOffset);

        depthAABB.lower.Set(m_aabb.lower.x - depthOffset,
                            m_aabb.lower.y - depthOffset,
                            m_aabb.lower.z - depthOffset);
        depthAABB.upper.Set(m_aabb.upper.x + depthOffset,
                            m_aabb.upper.y + depthOffset,
                            m_aabb.upper.z + depthOffset);

        return hdAABBIntersection(aabb, depthAABB);
    }
}


bool Block::RayIntersection(const hdVec3* ray, hdVec3& vOut)
{
    if (this->GetVertexCount() < 3) return false;

    hdVec3* vertices = this->GetVertices();
    hdVec3 triangle[3];
    int i1, i2;
    static hdVec3 verticesWithDepth[256];

    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        verticesWithDepth[i].x = this->GetVertices()[i].x;
        verticesWithDepth[i].y = this->GetVertices()[i].y;
        verticesWithDepth[i].z = this->GetVertices()[i].z - m_depth;
    }

    if (hdRayIntersectsVertices(verticesWithDepth, this->GetVertexCount(), this->GetWorldCenter(), ray, vOut)) return true;

    // quad strip
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        i1 = i;
        i2 = (i+1)%(this->GetVertexCount());

        triangle[0].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z - m_depth);
        triangle[1].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z + m_depth);
        triangle[2].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z - m_depth);

        if (hdRayIntersectsTriangle(triangle, ray, vOut)) return true;

        triangle[0].Set(vertices[i1].x, vertices[i1].y, vertices[i1].z + m_depth);
        triangle[1].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z - m_depth);
        triangle[2].Set(vertices[i2].x, vertices[i2].y, vertices[i2].z + m_depth);

        if (hdRayIntersectsTriangle(triangle, ray, vOut)) return true;
    }

    return false;
}


void Block::DrawHighlight(const hdVec3& screenCenter, const hdColor4& color)
{
    if (!this->IsDrawable()) return;

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    float tint[4];

    tint[0] = ((float)color.r) / 255.0f;
    tint[1] = ((float)color.g) / 255.0f;
    tint[2] = ((float)color.b) / 255.0f;
    tint[3] = ((float)color.a) / 255.0f;

    static hdVec3 vertices[256];
    static hdVec3 verticesWithDepth[256];

    memset(vertices, 0, sizeof(vertices));
    memset(verticesWithDepth, 0, sizeof(verticesWithDepth));

    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        vertices[i].x = this->GetVertices()[i].x;
        vertices[i].y = this->GetVertices()[i].y;
        vertices[i].z = this->GetVertices()[i].z;
    }

    hdVec3 center = this->GetWorldCenter();
    hdTranslateVertices(vertices, this->GetVertexCount(), -center);
    hdScaleVertices(vertices, this->GetVertexCount(), hdVec3(1.05f, 1.05f, 1.05f));
    hdTranslateVertices(vertices, this->GetVertexCount(), center);

    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        verticesWithDepth[i].x = vertices[i].x;
        verticesWithDepth[i].y = vertices[i].y;
        verticesWithDepth[i].z = vertices[i].z - (m_depth - 0.02f);
    }

    DrawVerticesWithTint(verticesWithDepth,
                         this->GetTextureCoordinates(),
                         this->GetVertexCount(),
                         this->GetAABB(),
                         NULL, tint, m_alpha);

    hdglBegin(GL_QUADS);
    DrawExtrusion(vertices,
                  this->GetTextureCoordinates(),
                  this->GetVertexCount(),
                  this->GetAABB(),
                  NULL, tint, m_alpha, m_depth, true, false, false, screenCenter);
    hdglEnd();

    glDisable(GL_CULL_FACE);
}


void Block::DrawExtruded(hdTexture *texture, bool front, const hdVec3& screenCenter)
{
    int i;
    int triStripVertCount;
    static hdVec3 triStripVerts[256];
    Block *block;

    if (!this->IsDrawable()) return;

    block = ((Block *)this);

    if (m_shapeType == e_hdkShapeTypeTriStrip)
    {
        // Triangle strip vertices must be arranged into a hull
        if (m_triStripHullVertices == NULL)
        {
            m_triStripHullVertices = new hdTypedefList<hdVec3, kMaxPolygonVertices>();

            triStripVertCount = this->GetVertexCount() - 1;

            for(i = 0; i < this->GetVertexCount(); i += 2)
            {
                triStripVerts[triStripVertCount].x = block->GetVertices()[i].x;
                triStripVerts[triStripVertCount].y = block->GetVertices()[i].y;
                triStripVerts[triStripVertCount].z = block->GetVertices()[i].z;
                --triStripVertCount;
            }
            for(i = this->GetVertexCount()-(1 + (this->GetVertexCount() % 2)); i > 0; i -= 2)
            {
                triStripVerts[triStripVertCount].x = block->GetVertices()[i].x;
                triStripVerts[triStripVertCount].y = block->GetVertices()[i].y;
                triStripVerts[triStripVertCount].z = block->GetVertices()[i].z;
                --triStripVertCount;
            }

            for (i = 0; i < this->GetVertexCount(); ++i)
            {
                m_triStripHullVertices->Add(triStripVerts[i]);
            }

            hdAssert(triStripVertCount == -1);
        }

        // Generate extrusion tex coords if we've not already done it.
        if (false == m_generatedExtrusionTex)
        {
            memset(m_extrusionTexCoords, 0, sizeof(m_extrusionTexCoords));

            GenerateExtrusionTextureVertices(m_extrusionTexCoords,
                                             m_triStripHullVertices->GetItems(),
                                             block->GetTextureCoordinates(),
                                             block->GetVertexCount(),
                                             block->GetStartingAABB(),
                                             texture,
                                             block->GetDepth(),
                                             block->GetTextureRepeatX());
        }

#ifdef LEVEL_EDITOR
        m_generatedExtrusionTex = false;
#endif

        DrawExtrusion(m_triStripHullVertices->GetItems(),
                      m_extrusionTexCoords,
                      block->GetVertexCount(),
                      block->GetStartingAABB(),
                      texture,
                      block->GetTint(),
                      block->GetAlpha(),
                      block->GetDepth(), m_drawWithCustomLighting, !front, true, screenCenter);

#ifdef LEVEL_EDITOR
        delete m_triStripHullVertices;
        m_triStripHullVertices = NULL;
#endif
    }
    else
    {
        m_triStripHullVertices = NULL;
        
        if (false == m_generatedExtrusionTex)
        {
            memset(m_extrusionTexCoords, 0, sizeof(m_extrusionTexCoords));
            
            GenerateExtrusionTextureVertices(m_extrusionTexCoords, 
                                             block->GetVertices(), 
                                             block->GetTextureCoordinates(), 
                                             block->GetVertexCount(), 
                                             block->GetStartingAABB(), 
                                             texture, 
                                             block->GetDepth(),
                                             block->GetTextureRepeatX());
        }
        
        DrawExtrusion(block->GetVertices(), 
                      m_extrusionTexCoords, 
                      block->GetVertexCount(), 
                      block->GetStartingAABB(), 
                      texture, 
                      block->GetTint(),
                      block->GetAlpha(),
                      block->GetDepth(), m_drawWithCustomLighting, !front,
                      true,
                      screenCenter);
    }
}


void Block::DrawTrianglesFront()
{
#ifdef LEVEL_EDITOR
    hdglColor4f(m_colorTint[0], m_colorTint[1], m_colorTint[2], m_colorTint[3]);
#else
    if (!this->IsDrawable()) return;
    hdglColor4ub(m_tintBytes.r, m_tintBytes.g, m_tintBytes.b, hdClamp((int)(m_alpha * 255), 0, (int)m_tintBytes.a) );
#endif
    
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        if (m_texture != NULL) 
        {
            hdglTexCoord2f( ((Block *)this)->GetTextureCoordinates()[i].x,  ((Block *)this)->GetTextureCoordinates()[i].y);
        }
        hdglVertex3f(((Block *)this)->GetVertices()[i].x, 
                     ((Block *)this)->GetVertices()[i].y, 
                     ((Block *)this)->GetVertices()[i].z - m_depth);
    }
}


void Block::DrawTrianglesBack()
{
    if (!this->IsDrawable()) return;
    
    hdVec3 *verticesWithOppDepth = new hdVec3[this->GetVertexCount()];
    hdVec2 *oppTexCoords = new hdVec2[this->GetVertexCount()];
    
    if(m_shapeType == e_hdkShapeTypeTriStrip)
    {
        for (int i = 0; i < this->GetVertexCount();++i)
        {
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetVertices()[i].x;
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetVertices()[i].y;
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].z = ((Block *)this)->GetVertices()[i].z + m_depth;
            
            oppTexCoords[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetTextureCoordinates()[i].x;
            oppTexCoords[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetTextureCoordinates()[i].y;
        }
    }
    else
    {
        for (int i = this->GetVertexCount()-1; i >= 0 ; i--)
        {
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetVertices()[i].x;
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetVertices()[i].y;
            verticesWithOppDepth[this->GetVertexCount() - 1 - i].z = ((Block *)this)->GetVertices()[i].z + m_depth;
        }
        
        for (int i = this->GetVertexCount()-1; i >= 0 ; i--)
        {
            oppTexCoords[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetTextureCoordinates()[i].x;
            oppTexCoords[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetTextureCoordinates()[i].y;
        }
    }
    
    hdglColor4ub(m_tintBytes.r, m_tintBytes.g, m_tintBytes.b, hdClamp((int)(m_alpha * 255), 0, (int)m_tintBytes.a) );
    
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        if (m_texture != NULL) 
        {
            hdglTexCoord2f( oppTexCoords[i].x,  oppTexCoords[i].y);
        }
        hdglVertex3f(verticesWithOppDepth[i].x, verticesWithOppDepth[i].y, verticesWithOppDepth[i].z);
    }
    
    delete verticesWithOppDepth;
    delete oppTexCoords;
}


void Block::Draw() const
{
#ifdef LEVEL_EDITOR
    GLenum prim;
    GLboolean blendEnabled;
    GLint blendFuncSrc, blendFuncDst;
    
    hdVec3 verticesWithDepth[1024];
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        verticesWithDepth[i].x = ((Block *)this)->GetVertices()[i].x;
        verticesWithDepth[i].y = ((Block *)this)->GetVertices()[i].y;
        verticesWithDepth[i].z = ((Block *)this)->GetVertices()[i].z + m_depth;
    }
    
    hdVec3 verticesWithOppDepth[1024];
    for (int i = this->GetVertexCount()-1; i >= 0 ; i--)
    {
        verticesWithOppDepth[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetVertices()[i].x;
        verticesWithOppDepth[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetVertices()[i].y;
        verticesWithOppDepth[this->GetVertexCount() - 1 - i].z = ((Block *)this)->GetVertices()[i].z - m_depth;
    }
    
    hdVec2 oppTexCoords[1024];
    for (int i = this->GetVertexCount()-1; i >= 0 ; i--)
    {
        oppTexCoords[this->GetVertexCount() - 1 - i].x = ((Block *)this)->GetTextureCoordinates()[i].x;
        oppTexCoords[this->GetVertexCount() - 1 - i].y = ((Block *)this)->GetTextureCoordinates()[i].y;
    }
    
    if(m_shapeType== e_hdkShapeTypeSpikes) 
    {
        prim = GL_TRIANGLES;
    } 
    else if (m_shapeType == e_hdkShapeTypeRectangle) 
    {
        prim = GL_QUADS;
    } 
    else if(m_shapeType == e_hdkShapeTypeTriStrip) 
    {
        prim = GL_TRIANGLE_STRIP;
    } 
    else 
    {
#if TARGET_GL_OPENGL == 1
        prim = GL_POLYGON;
#else
        prim = GL_TRIANGLE_FAN;
#endif
    }
    
    glEnable(GL_TEXTURE_2D);
    hdglBindTexture(m_texture);
    
    if (IsTransparent())
    {
        glGetBooleanv(GL_BLEND, &blendEnabled);
        glEnable(GL_BLEND);
        glGetIntegerv(GL_BLEND_SRC, &blendFuncSrc);
        glGetIntegerv(GL_BLEND_DST, &blendFuncDst); 
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    hdglBegin(prim); 
    ((Block *)this)->DrawTrianglesFront();
    hdglEnd();
    
    if (IsExtrusion())
    {	
        ((Block *)this)->DrawExtruded(m_texture, false, hdVec3(0,0,0));
    }
    
    if (IsTransparent())
    {
        if (blendEnabled == GL_FALSE)
        {
            glDisable(GL_BLEND);
        }
        glBlendFunc(blendFuncSrc, blendFuncDst);
    }
    
#if 0	
    hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    hdglBegin(GL_LINE_LOOP);
    for (int i = 0; i < this->GetVertexCount(); ++i)
    {
        hdglVertex2f(m_vertices->GetItems()[i].x, m_vertices->GetItems()[i].y);
    }
    hdglEnd();
    
    // Draw a line to indicate the axis of rotation
    hdVec3 center = ((Block *)this)->GetWorldCenter();
    
    hdVec3 line(0.0f, ((m_aabb.upper.x - m_aabb.lower.x)/2.0f), 0.0f);
    hdVec3 out;
    hdMatrix rotMatrix;
    
    MatrixRotationZ(rotMatrix, m_obb.transform.rotation.z);
    MatrixVec3Multiply(out, line, rotMatrix);
    
    hdglBegin(GL_LINES);
    hdglVertex3f(center.x, center.y, -m_depth+0.09f);
    hdglVertex3f(center.x + out.x, center.y + out.y, -m_depth+0.09f);
    hdglEnd();
    
    
    hdglBegin(GL_LINE_LOOP);
    hdglVertex3f(m_aabb.lower.x, m_aabb.lower.y, -m_depth+0.09f);
    hdglVertex3f(m_aabb.lower.x, m_aabb.upper.y, -m_depth+0.09f);
    hdglVertex3f(m_aabb.upper.x, m_aabb.upper.y, -m_depth+0.09f);
    hdglVertex3f(m_aabb.upper.x, m_aabb.lower.y, -m_depth+0.09f);
    hdglEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    GLboolean depthTest;
    
    glGetBooleanv(GL_DEPTH_TEST, &depthTest);
    
    glDisable(GL_DEPTH_TEST);
    if (m_tag == 1001)
    {
        glLineWidth(4.0f);
        hdglBegin(GL_LINE_LOOP);
        
        hdglColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        hdglVertex3f(m_aabb.lower.x, m_aabb.lower.y, -(fabs(m_depth)+0.09f));
        hdglVertex3f(m_aabb.lower.x, m_aabb.upper.y, -(fabs(m_depth)+0.09f));
        hdglVertex3f(m_aabb.upper.x, m_aabb.upper.y, -(fabs(m_depth)+0.09f));
        hdglVertex3f(m_aabb.upper.x, m_aabb.lower.y, -(fabs(m_depth)+0.09f));
        hdglEnd();
        glLineWidth(1.0f);
    }
    
    if (depthTest == GL_TRUE)
    {
        glEnable(GL_DEPTH_TEST);
    }
    
    glEnable(GL_TEXTURE_2D);
#endif
#endif
}


void Block::DrawBlock(bool front, const hdVec3& screenCenter)
{
    GLenum currPrim;
    GLboolean blendEnabled, cullEnabled;
    GLint blendFuncSrc, blendFuncDst;
    
    currPrim = -1;
    hdglBindTexture(m_texture);
    
    if(GetShapeType() == e_hdkShapeTypeSpikes) 
    {
        currPrim = GL_TRIANGLES;
    } 
    else if (GetShapeType() == e_hdkShapeTypeRectangle) 
    {
        currPrim = GL_QUADS;
    } 
    else if(GetShapeType() == e_hdkShapeTypeTriStrip) 
    {
        currPrim = GL_TRIANGLE_STRIP;
    } 
    else 
    {
        currPrim = GL_TRIANGLE_FAN;
    }
    
    if (IsTransparent())
    {
        glGetBooleanv(GL_BLEND, &blendEnabled);
        glGetBooleanv(GL_CULL_FACE, &cullEnabled);
        glEnable(GL_BLEND);
        glGetIntegerv(GL_BLEND_SRC, &blendFuncSrc);
        glGetIntegerv(GL_BLEND_DST, &blendFuncDst); 
        
        if (m_texture->isPremultipliedAlpha && 
            hdClamp(m_alpha, 0.0f, m_colorTint[3]) == 1.0f)
        {
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        glDisable(GL_CULL_FACE);
        
        if (IsExtrusion())
        {	
            DrawExtruded(m_texture, front, screenCenter);
        }
        
        hdglBegin(currPrim); 
        DrawTrianglesFront();
        hdglEnd();
        
        if (blendEnabled == GL_FALSE)
        {
            glDisable(GL_BLEND);
        }
        
        if (cullEnabled)
        {
            glEnable(GL_CULL_FACE);
        }
        
        glBlendFunc(blendFuncSrc, blendFuncDst);
    }
    else
    {
        hdglBegin(currPrim); 
        DrawTrianglesFront();
        hdglEnd();
        
        if (IsExtrusion())
        {	
            DrawExtruded(m_texture, front, screenCenter);
        }
    }
}
