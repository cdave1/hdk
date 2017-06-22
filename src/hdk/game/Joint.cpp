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

#include "Joint.h"

Joint::Joint() : hdPhysicsJoint(), hdPolygon()
{
    m_userType = (int)e_hdkTypeJoint;
    m_joint = NULL;

    m_firstTotemBlock = NULL;
    m_secondTotemBlock = NULL;

    m_secondBodyPoint.Set(0,0,0);

    m_enableMotor = false;
    m_enableLimit = false;
    m_lowerAngle = 0.0f;
    m_upperAngle = 0.0f;
    m_maxMotorTorque = 0.0f;
    m_motorSpeed = 0.0f;

    m_prismaticLowerTranslation = 0.0f;
    m_prismaticUpperTranslation = 0.0f;
    m_prismaticMaxMotorForce = 0.0f;
}


Joint::Joint(hdGameWorld *gameWorld,
             b2World *physicsWorld,
             e_hdkJointType jointType,
             const hdVec3& rotationPoint,
             Block* firstBody,
             Block* secondBody) : hdPhysicsJoint(), hdPolygon(gameWorld)
{
    m_userType = (int)e_hdkTypeJoint;
    m_joint = NULL;
    m_physicsWorld = physicsWorld;
    m_jointType = jointType;

    m_enableMotor = false;
    m_enableLimit = false;
    m_lowerAngle = 0.0f;
    m_upperAngle = 0.0f;
    m_maxMotorTorque = 0.0f;
    m_motorSpeed = 0.0f;

    m_prismaticLowerTranslation = 0.0f;
    m_prismaticUpperTranslation = 0.0f;
    m_prismaticMaxMotorForce = 0.0f;

    this->SetFirstBody(firstBody);
    this->SetSecondBody(secondBody);

    m_aabb.lower = rotationPoint - hdVec3(0.12f, 0.12f, 0.0f);
    m_aabb.upper = rotationPoint + hdVec3(0.12f, 0.12f, 0.0f);
    this->SetAs2DBox(m_aabb.lower, m_aabb.upper);

    switch (jointType) {
        case e_hdkJointTypeRevolute:
            m_colorTint[0] = 1.0f;
            m_colorTint[1] = 0.6f;
            m_colorTint[2] = 0.0f;
            m_colorTint[3] = 1.0f;
            break;
        case e_hdkJointTypeDistance:
            m_colorTint[0] = 0.0f;
            m_colorTint[1] = 0.6f;
            m_colorTint[2] = 1.0f;
            m_colorTint[3] = 1.0f;
            break;
        case e_hdkJointTypePrismatic:
            m_colorTint[0] = 1.0f;
            m_colorTint[1] = 1.0f;
            m_colorTint[2] = 0.0f;
            m_colorTint[3] = 1.0f;
            break;
        default:
            m_colorTint[0] = 1.0f;
            m_colorTint[1] = 0.6f;
            m_colorTint[2] = 0.0f;
            m_colorTint[3] = 1.0f;
            break;
    }

}


Joint::~Joint()
{
}


void Joint::Init(hdGameWorld *gameWorld,
                 b2World *physicsWorld)
{

    ((hdPolygon *)this)->Init(gameWorld);
    m_physicsWorld = physicsWorld;

    this->ResetAABB();
    this->ResetOBB();

    this->CreateJoint();
}


/*
 * The joint can only be destroyed if the physics bodies attached are the same as the ones
 * that were around when the joint was created.
 */
void Joint::CreateJoint()
{
    b2Body* firstBody = NULL;
    b2Body* secondBody = NULL;

    if(m_firstTotemBlock == NULL || m_secondTotemBlock == NULL)
    {
        hdPrintf("Couldn't create joint at %f, %f because one of the game objects was missing.",
                 this->GetWorldCenter().x, this->GetWorldCenter().y);
        return;
    }

    if (m_jointType == e_hdkJointTypeRevolute)
    {
        // The physics bodies may have changed while the joint was out doing
        // other things.
        firstBody = (b2Body *)m_firstTotemBlock->GetPhysicsBody();
        secondBody = (b2Body *)m_secondTotemBlock->GetPhysicsBody();

        if (firstBody == NULL || secondBody == NULL)
        {
            hdPrintf("Couldn't create revolute joint at %f, %f because one of the b2 physics bodies has gone away.",
                     this->GetWorldCenter().x, this->GetWorldCenter().y);
            return;
        }

        b2Vec2 b2RotPoint = b2Vec2(this->GetWorldCenter().x, this->GetWorldCenter().y);
        b2RevoluteJointDef jointDef;

        jointDef.Initialize(firstBody, secondBody, b2RotPoint);

        jointDef.enableMotor = m_enableMotor;
        jointDef.enableLimit = m_enableLimit;
        jointDef.lowerAngle = m_lowerAngle;
        jointDef.upperAngle = m_upperAngle;
        jointDef.maxMotorTorque = m_maxMotorTorque;
        jointDef.motorSpeed = m_motorSpeed;

        m_joint = (b2RevoluteJoint *)m_physicsWorld->CreateJoint(&jointDef);
    }
    else if (m_jointType == e_hdkJointTypePrismatic)
    {
        b2PrismaticJointDef jointDef;
        firstBody = (b2Body *)m_firstTotemBlock->GetPhysicsBody();
        secondBody = (b2Body *)m_secondTotemBlock->GetPhysicsBody();

        if (firstBody == NULL || secondBody == NULL)
        {
            hdPrintf("Couldn't create prismatic joint at %f, %f because one of the b2 physics bodies has gone away.",
                     this->GetWorldCenter().x, this->GetWorldCenter().y);
            return;
        }

        b2Vec2 anchorPoint = b2Vec2(this->GetWorldCenter().x, this->GetWorldCenter().y);
        b2Vec2 worldAxis = anchorPoint - b2Vec2(m_secondBodyPoint.x, m_secondBodyPoint.y);
        worldAxis.Normalize();
        jointDef.Initialize(firstBody, secondBody, anchorPoint, worldAxis);

        jointDef.lowerTranslation = m_prismaticLowerTranslation;
        jointDef.upperTranslation = m_prismaticUpperTranslation;
        jointDef.enableLimit = m_enableLimit;
        jointDef.maxMotorForce = m_prismaticMaxMotorForce;
        jointDef.motorSpeed = m_motorSpeed;
        jointDef.enableMotor = m_enableLimit;

        m_joint = (b2PrismaticJoint *)m_physicsWorld->CreateJoint(&jointDef);

    }
    else if (m_jointType == e_hdkJointTypeDistance)
    {
        b2DistanceJointDef jointDef;
        firstBody = (b2Body *)m_firstTotemBlock->GetPhysicsBody();
        secondBody = (b2Body *)m_secondTotemBlock->GetPhysicsBody();
        jointDef.collideConnected = true;
        if (firstBody == NULL || secondBody == NULL)
        {
            hdPrintf("Couldn't create distance joint at %f, %f because one of the b2 physics bodies has gone away.",
                     this->GetWorldCenter().x, this->GetWorldCenter().y);
            return;
        }

        b2Vec2 firstAnchorPoint = b2Vec2(this->GetWorldCenter().x, this->GetWorldCenter().y);
        b2Vec2 secondAnchorPoint = b2Vec2(m_secondBodyPoint.x, m_secondBodyPoint.y);
        jointDef.Initialize(firstBody, secondBody, firstAnchorPoint, secondAnchorPoint);
        m_joint = (b2DistanceJoint *)m_physicsWorld->CreateJoint(&jointDef);
    }
}


void Joint::DestroyJoint()
{
    if (m_joint != NULL)
    {
        m_physicsWorld->DestroyJoint(m_joint);
        m_joint = NULL;
    }
}


const int Joint::SetFirstBody(const Block* body)
{
    if (body == m_secondTotemBlock) return -1;
    if (body == NULL) return -1;
    m_firstTotemBlock = (Block*)body;
    return 0;
}


const int Joint::SetSecondBody(const Block* body)
{
    if (body == m_firstTotemBlock) return -1;
    if (body == NULL) return -1;
    m_secondTotemBlock = (Block*)body;
    m_secondBodyPoint = ((Block*)body)->GetWorldCenter();
    return 0;
}


void Joint::SetSecondJointPoint(const hdVec3& point)
{
    m_secondBodyPoint = point;
}


void Joint::Step()
{
    if (m_firstTotemBlock == NULL || m_secondTotemBlock == NULL) return;
    if (m_firstTotemBlock->GetPhysicsBody() == NULL || m_secondTotemBlock->GetPhysicsBody() == NULL)
    {
        m_joint = NULL;
        return;
    }
}


bool Joint::AABBIntersection(const hdAABB& aabb)
{
#ifdef LEVEL_EDITOR
    hdAABB depthAABB;
    depthAABB.lower.Set(m_aabb.lower.x, m_aabb.lower.y, m_aabb.lower.z - 2.5f);
    depthAABB.upper.Set(m_aabb.upper.x, m_aabb.upper.y, m_aabb.upper.z + 2.5f);
    return hdAABBIntersection(aabb, depthAABB);
#else
    if (m_joint == NULL) return false;
    if (m_firstTotemBlock == NULL || m_secondTotemBlock == NULL) return false;
    if (m_joint->GetBody1() == NULL || m_joint->GetBody2() == NULL) return false;
    if (m_firstTotemBlock->GetPhysicsBody() == NULL ||
        m_secondTotemBlock->GetPhysicsBody() == NULL) return false;

    if (m_jointType == e_hdkJointTypeDistance)
    {
        hdAABB depthAABB;
        hdVec3 a(m_joint->GetAnchor1().x, m_joint->GetAnchor1().y, - 2.5f);
        hdVec3 b(m_joint->GetAnchor2().x, m_joint->GetAnchor2().y, + 2.5f);

        depthAABB.lower = hdMin(a, b);
        depthAABB.upper = hdMax(a, b);
        return hdAABBIntersection(aabb, depthAABB);
    }
    else
    {
        return (m_firstTotemBlock->AABBIntersection(aabb)) ||
        (m_secondTotemBlock->AABBIntersection(aabb));
    }
#endif
}


#ifdef LEVEL_EDITOR
/*
 * The second joint point needs to be scaled if this is a distance joint.
 */
void Joint::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
    if (m_jointType == e_hdkJointTypeDistance)
    {
        hdTranslateVertices(&m_secondBodyPoint, 1, -scaleAxis);
        hdScaleVertices(&m_secondBodyPoint, 1, scaleVector);
        hdTranslateVertices(&m_secondBodyPoint, 1, scaleAxis);
    }

    hdTranslateVertices(m_vertices->GetItems(),
                        m_vertices->GetItemCount(),
                        -scaleAxis);

    hdScaleVertices(m_vertices->GetItems(),
                    m_vertices->GetItemCount(),
                    scaleVector);

    hdTranslateVertices(m_vertices->GetItems(),
                        m_vertices->GetItemCount(),
                        scaleAxis);

    this->ResetAABB();
    this->SetStartingVerticesToCurrent();
}

#endif



void Joint::Draw() const
{
#ifdef LEVEL_EDITOR
    hdVec3 center, firstCenter, secondCenter;

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, (fabs(m_firstTotemBlock->GetDepth())+0.09f));

    DrawPolygonWithTint((hdPolygon *)this, NULL, m_colorTint, m_alpha);

    if (m_jointType == e_hdkJointTypeDistance || m_jointType == e_hdkJointTypePrismatic)
    {
        if (m_joint != NULL && m_firstTotemBlock->GetPhysicsBody() != NULL && m_secondTotemBlock->GetPhysicsBody() != NULL)
        {
            hdglBegin(GL_LINES);
            hdglColor4f(0.5f, 0.5f, 1.0f, 1.0f);
            hdglVertex2f(m_joint->GetAnchor1().x, m_joint->GetAnchor1().y);
            if (m_jointType == e_hdkJointTypePrismatic)
            {
                hdglVertex2f(m_secondBodyPoint.x, m_secondBodyPoint.y);
            }
            else
            {
                hdglVertex2f(m_joint->GetAnchor2().x, m_joint->GetAnchor2().y);
            }
            hdglEnd();
        }
    }
    else if (m_jointType == e_hdkJointTypeRevolute)
    {
        hdglBegin(GL_LINES);
        hdglColor4f(0.9f, 0.9f, 0.0f, 1.0f);

        center = ((Joint *)this)->GetWorldCenter();
        firstCenter = ((Block *)((Joint *)this)->GetFirstBody())->GetWorldCenter();
        secondCenter = ((Block *)((Joint *)this)->GetSecondBody())->GetWorldCenter();

        hdglVertex2f(center.x, center.y);
        hdglVertex2f(firstCenter.x, firstCenter.y);

        hdglVertex2f(center.x, center.y);
        hdglVertex2f(secondCenter.x, secondCenter.y);
        hdglEnd();
    }

    hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    hdglBegin(GL_LINE_LOOP);
    for (int i = 0; i < this->GetVertexCount(); i++)
    {
        hdglVertex2f(m_vertices->GetItems()[i].x, m_vertices->GetItems()[i].y);
    }
    hdglEnd();

    hdglBegin(GL_LINE_LOOP);
    hdglVertex2f(m_aabb.lower.x, m_aabb.lower.y);
    hdglVertex2f(m_aabb.lower.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.lower.y);
    hdglEnd();

    glPopMatrix();

#else
    if (m_joint == NULL) return;
    if (m_firstTotemBlock == NULL || m_secondTotemBlock == NULL) return;
    if (m_joint->GetBody1() == NULL || m_joint->GetBody2() == NULL) return;
    if (m_firstTotemBlock->GetPhysicsBody() == NULL ||
        m_secondTotemBlock->GetPhysicsBody() == NULL) return;

    if (m_jointType == e_hdkJointTypeDistance)
    {
        if (m_joint != NULL && m_firstTotemBlock->GetPhysicsBody() != NULL && m_secondTotemBlock->GetPhysicsBody() != NULL)
        {
            glDisable(GL_BLEND);
#if TARGET_GL_OPENGL == 1
            glLineWidth(3.0f);
#endif
            hdglBegin(GL_LINES);
#if TARGET_GL_OPENGLES == 1
            glLineWidth(3.0f);
#endif
            hdglColor4ub(0, 0, 0, 255);
            hdglVertex3f(m_joint->GetAnchor1().x, m_joint->GetAnchor1().y, (fabs(m_firstTotemBlock->GetDepth())+0.01f));
            hdglVertex3f(m_joint->GetAnchor2().x, m_joint->GetAnchor2().y, (fabs(m_secondTotemBlock->GetDepth())+0.01f));

            hdglVertex3f(m_joint->GetAnchor1().x, m_joint->GetAnchor1().y, -(fabs(m_firstTotemBlock->GetDepth())+0.01f));
            hdglVertex3f(m_joint->GetAnchor2().x, m_joint->GetAnchor2().y, -(fabs(m_secondTotemBlock->GetDepth())+0.01f));
            hdglEnd();
            glEnable(GL_BLEND);
        }
        glLineWidth(1.0f);
    }

    if (m_joint != NULL && m_firstTotemBlock->GetPhysicsBody() != NULL)
    {
        hdglBegin(GL_QUADS);
        hdglColor4ub(255, 255, 255, 255);

        this->InternalDrawJoint(m_joint->GetAnchor1().x, m_joint->GetAnchor1().y,
                                (fabs(m_firstTotemBlock->GetDepth())+m_firstTotemBlock->GetZOffset()+0.02f));

        if (m_jointType == e_hdkJointTypeDistance)
        {
            this->InternalDrawJoint(m_joint->GetAnchor2().x, m_joint->GetAnchor2().y,
                                    (fabs(m_secondTotemBlock->GetDepth())+m_firstTotemBlock->GetZOffset()+0.02f));
        }

        hdglEnd();
    }
#endif
}


void Joint::InternalDrawJoint(float x, float y, float z) const
{
    hdglTexCoord2f(0.0f, 1.0f);
    hdglVertex3f(x-0.07f, y-0.07f, z);
    hdglTexCoord2f(0.0f, 0.0f);
    hdglVertex3f(x-0.07f, y+0.07f, z);
    hdglTexCoord2f(1.0f, 0.0f);
    hdglVertex3f(x+0.07f, y+0.07f, z);
    hdglTexCoord2f(1.0f, 1.0f);
    hdglVertex3f(x+0.07f, y-0.07f, z);
}
