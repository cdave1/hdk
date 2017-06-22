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

#ifndef _HDK_ENUMS_H_
#define _HDK_ENUMS_H_

#define _HDK_GAME_STARTING_LEVEL 1

#define _HDK_GAME_GOAL_MIN_PROPORTION 0.75f
#define	HDK_GAME_GOAL_MIDDLE_PROPORTION 0.9f
#define _HDK_GAME_GOAL_MAX_PROPORTION 1.0f

#define HACK_MAX_WORLD_TAG 10
#define _HDK_SCRIPTS_WILDCARD_TAG -999

typedef enum {
    e_hdkTypeBlock = 0x0002,
    e_hdkTypeJack = 0x0004,
    e_hdkTypePolygon = 0x0008,
    e_hdkTypeJoint = 0x0020,
    e_hdkTypeLevelPickerBlock = 0x0040,
    e_hdkTypeEvent = 0x0080,
    e_hdkTypeLayer = 0x0200,
    e_hdkType_COUNT = 0xF000,
} e_hdkType;


typedef enum
{
    e_hdkGameFlagsBlockLost = 0x0001,
    e_hdkGameFlagsBlockDrawRaw = 0x0002,
    e_hdkGameFlagsBlockTemporary = 0x0004,
    e_hdkGameFlagsBlockHeavyFirstTap = 0x0008,
    e_hdkGameFlagsBlockHeavySecondTap = 0x0020,
    e_hdkGameFlagsBlockHeavyThirdTap = 0x0040,
    e_hdkGameFlagsBlockHasRevJoint = 0x0080,
    e_hdkGameFlags_COUNT = 0xFFF0,
} e_hdkGameFlags;


typedef enum
{
    e_hdkDifficultyEasy = 0,
    e_hdkDifficultyMedium,
    e_hdkDifficultyHard,
} e_hdkDifficultyLevel;


typedef enum {
    e_hdkJackTypeDefault = 0,
    e_hdkJackType_Count
} e_hdkJackType;

struct JackTypeMenuItem
{
    e_hdkJackType jackType;
    const char* jackTypeName;
};

const struct JackTypeMenuItem JackTypeMenuItems[]
= {
    {e_hdkJackTypeDefault, "Default Jack" },
};


typedef enum {
    e_hdkShapeTypeRectangle = 0,
    e_hdkShapeTypeStar,
    e_hdkShapeTypeTriangle,
    e_hdkShapeTypeCylinder,
    e_hdkShapeTypeCross,
    e_hdkShapeTypeCustom,
    e_hdkShapeTypeSpikes,
    e_hdkShapeTypeTriStrip,
    e_hdkShapeTypeAABBCylinder,
    e_hdkShapeTypeHexagon,
    e_hdkShapeTypeMultiStar,
    e_hdkShapeType_Count,
} e_hdkShapeType;


struct shapeTypeMenuItem
{
    e_hdkShapeType shapeType;
    const char* shapeTypeName;
};

const struct shapeTypeMenuItem shapeTypeMenuItems[]
= {
    {e_hdkShapeTypeRectangle, "Rectangle" },
    {e_hdkShapeTypeStar, "Star" },
    {e_hdkShapeTypeTriangle, "Triangle" },
    {e_hdkShapeTypeCylinder, "Cylinder" },
    {e_hdkShapeTypeCross, "Cross" },
    {e_hdkShapeTypeCustom, "Custom shape" },
    {e_hdkShapeTypeSpikes, "Spikes" },
    {e_hdkShapeTypeTriStrip, "Triangle Strip"},
    {e_hdkShapeTypeAABBCylinder, "AABB Cylinder"},
    {e_hdkShapeTypeHexagon, "Hexagon"},
    {e_hdkShapeTypeMultiStar, "Multi-Point Star"},
};


typedef enum
{
    e_hdkBlockTypeNormal = 0,
    e_hdkBlockTypeNormalSpecial,
    e_hdkBlockTypeNormalStatic,
    e_hdkBlockTypeGoal,
    e_hdkBlockTypeGoalDestroyable,
    e_hdkBlockTypeBoundary,
    e_hdkBlockTypeGround,
    e_hdkBlockTypeHazard,
    e_hdkBlockTypeHazardDest,
    e_hdkBlockTypeActivator,
    e_hdkBlockTypeDraggable,
    e_hdkBlockTypeGenericSensor,
    e_hdkBlockTypeNormalInv,
    e_hdkBlockTypeEventSender,
    e_hdkBlockTypeMessage,
    e_hdkBlockTypeCosmetic,
    e_hdkBlockTypeLight,
    e_hdkBlockTypeBillboard,
    e_hdkBlockType_Count,
} e_hdkBlockType;


struct BlockTypeMenuItem
{
    e_hdkBlockType blockType;
    const char* blockTypeName;
};


const struct BlockTypeMenuItem BlockTypeMenuItems[]
= {
    {e_hdkBlockTypeNormal, "Normal" },
    {e_hdkBlockTypeNormalSpecial, "Normal (Special)" },
    {e_hdkBlockTypeNormalStatic, "Normal (Static)" },
    {e_hdkBlockTypeGoal, "Goal" },
    {e_hdkBlockTypeGoalDestroyable, "Destroyable Goal" },
    {e_hdkBlockTypeBoundary, "Boundary" },
    {e_hdkBlockTypeGround, "Ground" },
    {e_hdkBlockTypeHazard, "Hazard" },
    {e_hdkBlockTypeHazardDest, "Destroyable Hazard" },
    {e_hdkBlockTypeActivator, "Activator" },
    {e_hdkBlockTypeDraggable, "Draggable" },
    {e_hdkBlockTypeGenericSensor, "Generic Sensor" },
    {e_hdkBlockTypeNormalInv, "Normal (Invulnerable)" },
    {e_hdkBlockTypeEventSender, "Event Sender" },
    {e_hdkBlockTypeMessage, "Message"},
    {e_hdkBlockTypeCosmetic, "Cosmetic (Non Physics)" },
    {e_hdkBlockTypeLight, "Light" },
    {e_hdkBlockTypeBillboard, "Billboard"},
};


typedef enum {
    e_hdkMaterialWood = 0,
    e_hdkMaterialMetal,
    e_hdkMaterialGlass,
    e_hdkMaterialFloat,
    e_hdkMaterialIce,
    e_hdkMaterialRock,
    e_hdkMaterialEarth,
    e_hdkMaterialCustomTexture,
    e_hdkMaterialToyWood,
    e_hdkMaterialToyPlastic,
    e_hdkMaterialToyMetal,
    e_hdkMaterialToyRubber,
    e_hdkMaterialRawPolygon,
    e_hdkMaterialPlastic,
    e_hdkMaterialBomb,
    e_hdkMaterialJack,
    e_hdkMaterialGrinder,
    e_hdkMaterialLiquid,
    e_hdkMaterialText,
    e_hdkMaterialBouncyBomb,
    e_hdkMaterial_Count,
} e_hdkMaterial;


struct materialMenuItem
{
    e_hdkMaterial material;
    const char* materialName;
};


const struct materialMenuItem materialMenuItems[] = {
    { e_hdkMaterialWood, "Wood" },
    { e_hdkMaterialMetal, "Metal" },
    { e_hdkMaterialGlass, "Glass" },
    { e_hdkMaterialFloat, "Floaty" },
    { e_hdkMaterialIce, "Ice" },
    { e_hdkMaterialRock, "Rock" },
    { e_hdkMaterialEarth, "Earth" },
    { e_hdkMaterialCustomTexture, "Custom Texture" },
    { e_hdkMaterialToyWood, "Toy Game - Wood" },
    { e_hdkMaterialToyPlastic, " Toy Game - Plastic" },
    { e_hdkMaterialToyMetal, "Toy Game - Metal" },
    { e_hdkMaterialToyRubber, "Toy Game - Rubber" },
    { e_hdkMaterialRawPolygon, "Raw Polygon" },
    { e_hdkMaterialPlastic, "Plastic" },
    { e_hdkMaterialBomb, "Bomb" },
    { e_hdkMaterialJack, "Jack" },
    { e_hdkMaterialGrinder, "Grinder" },
    { e_hdkMaterialLiquid, "Liquid" },
    {e_hdkMaterialText, "Text Block" },
    {e_hdkMaterialBouncyBomb, "Bouncy Bomb"},
};


typedef enum
{
    e_hdkJointTypeDistance = 0,
    e_hdkJointTypeRevolute,
    e_hdkJointTypePrismatic,
    e_hdkJoinType_Count,
} e_hdkJointType;


struct JointTypeMenuItem
{
    e_hdkJointType jointType;
    const char* jointTypeName;
};


const struct JointTypeMenuItem JointTypeMenuItems[] = {
    {e_hdkJointTypeDistance, "Distance Joint (2 Bodies, 2 points)" },
    {e_hdkJointTypeRevolute, "Revolute (2 Bodies, 1 point)" },
    {e_hdkJointTypePrismatic, "Prismatic (2 Bodies, 2 points)" },
};


/*
 * These events are fairly crappy...really need
 * a function to call, and an event object payload
 * that can hold dynamic values. Good example is the
 * rev joint event names.
 *
 * Also, events are just messages - the event handler
 * on the receiver should be configurable separately.
 */
typedef enum
{
    e_hdkEventTypeFireGun = 0,
    e_hdkEventTypeShowMessage,
    e_hdkEventTypeJointMotorStartPos,
    e_hdkEventTypeJointMotorStartNeg,
    e_hdkEventTypeJointMotorStop,
    e_hdkEventType_Count,
} e_hdkEventType;


struct EventTypeMenuItem
{
    e_hdkEventType eventType;
    const char* eventTypeName;
};


const struct EventTypeMenuItem EventTypeMenuItems[] = {
    {e_hdkEventTypeFireGun, "Tap Fire Gun" },
    {e_hdkEventTypeShowMessage, "Tap Show Message" },
    {e_hdkEventTypeJointMotorStartPos, "Joint Motor start (+)"},
    {e_hdkEventTypeJointMotorStartNeg, "Joint motor start (-)"},
    {e_hdkEventTypeJointMotorStop, "Joint Motor Stop"},
};


#endif
