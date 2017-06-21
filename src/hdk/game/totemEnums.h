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

#ifndef _TOTEM_ENUMS_H_
#define _TOTEM_ENUMS_H_

#define TOTEM_GAME_STARTING_LEVEL 1

#define TOTEM_GAME_GOAL_MIN_PROPORTION 0.75f
#define	TOTEM_GAME_GOAL_MIDDLE_PROPORTION 0.9f
#define TOTEM_GAME_GOAL_MAX_PROPORTION 1.0f

#define HACK_MAX_WORLD_TAG 10
#define TOTEM_SCRIPTS_WILDCARD_TAG -999

typedef enum {
    e_totemTypeBlock = 0x0002,
    e_totemTypeJack = 0x0004,
    e_totemTypePolygon = 0x0008,
    e_totemTypeJoint = 0x0020,
    e_totemTypeLevelPickerBlock = 0x0040,
    e_totemTypeEvent = 0x0080,
    e_totemTypeLayer = 0x0200,
    e_totemType_COUNT = 0xF000,
} e_totemType;


typedef enum
{
    e_totemGameFlagsBlockLost = 0x0001,
    e_totemGameFlagsBlockDrawRaw = 0x0002,
    e_totemGameFlagsBlockTemporary = 0x0004,
    e_totemGameFlagsBlockHeavyFirstTap = 0x0008,
    e_totemGameFlagsBlockHeavySecondTap = 0x0020,
    e_totemGameFlagsBlockHeavyThirdTap = 0x0040,
    e_totemGameFlagsBlockHasRevJoint = 0x0080,
    e_totemGameFlags_COUNT = 0xFFF0,
} e_totemGameFlags;


typedef enum
{
    e_totemDifficultyEasy = 0,
    e_totemDifficultyMedium,
    e_totemDifficultyHard,
} e_totemDifficultyLevel;


typedef enum {
    e_totemJackTypeDefault = 0,
    e_totemJackType_Count
} e_totemJackType;

struct totemJackTypeMenuItem
{
    e_totemJackType jackType;
    const char* jackTypeName;
};

const struct totemJackTypeMenuItem totemJackTypeMenuItems[]
= {
    {e_totemJackTypeDefault, "Default Jack" },
};


typedef enum {
    e_totemShapeTypeRectangle = 0,
    e_totemShapeTypeStar,
    e_totemShapeTypeTriangle,
    e_totemShapeTypeCylinder,
    e_totemShapeTypeCross,
    e_totemShapeTypeCustom,
    e_totemShapeTypeSpikes,
    e_totemShapeTypeTriStrip,
    e_totemShapeTypeAABBCylinder,
    e_totemShapeTypeHexagon,
    e_totemShapeTypeMultiStar,
    e_totemShapeType_Count,
} e_totemShapeType;


struct totemShapeTypeMenuItem
{
    e_totemShapeType shapeType;
    const char* shapeTypeName;
};

const struct totemShapeTypeMenuItem totemShapeTypeMenuItems[]
= {
    {e_totemShapeTypeRectangle, "Rectangle" },
    {e_totemShapeTypeStar, "Star" },
    {e_totemShapeTypeTriangle, "Triangle" },
    {e_totemShapeTypeCylinder, "Cylinder" },
    {e_totemShapeTypeCross, "Cross" },
    {e_totemShapeTypeCustom, "Custom shape" },
    {e_totemShapeTypeSpikes, "Spikes" },
    {e_totemShapeTypeTriStrip, "Triangle Strip"},
    {e_totemShapeTypeAABBCylinder, "AABB Cylinder"},
    {e_totemShapeTypeHexagon, "Hexagon"},
    {e_totemShapeTypeMultiStar, "Multi-Point Star"},
};


typedef enum
{
    e_totemBlockTypeNormal = 0,
    e_totemBlockTypeNormalSpecial,
    e_totemBlockTypeNormalStatic,
    e_totemBlockTypeGoal,
    e_totemBlockTypeGoalDestroyable,
    e_totemBlockTypeBoundary,
    e_totemBlockTypeGround,
    e_totemBlockTypeHazard,
    e_totemBlockTypeHazardDest,
    e_totemBlockTypeActivator,
    e_totemBlockTypeDraggable,
    e_totemBlockTypeGenericSensor,
    e_totemBlockTypeNormalInv,
    e_totemBlockTypeEventSender,
    e_totemBlockTypeMessage,
    e_totemBlockTypeCosmetic,
    e_totemBlockTypeLight,
    e_totemBlockTypeBillboard,
    e_totemBlockType_Count,
} e_totemBlockType;


struct totemBlockTypeMenuItem
{
    e_totemBlockType blockType;
    const char* blockTypeName;
};


const struct totemBlockTypeMenuItem totemBlockTypeMenuItems[]
= {
    {e_totemBlockTypeNormal, "Normal" },
    {e_totemBlockTypeNormalSpecial, "Normal (Special)" },
    {e_totemBlockTypeNormalStatic, "Normal (Static)" },
    {e_totemBlockTypeGoal, "Goal" },
    {e_totemBlockTypeGoalDestroyable, "Destroyable Goal" },
    {e_totemBlockTypeBoundary, "Boundary" },
    {e_totemBlockTypeGround, "Ground" },
    {e_totemBlockTypeHazard, "Hazard" },
    {e_totemBlockTypeHazardDest, "Destroyable Hazard" },
    {e_totemBlockTypeActivator, "Activator" },
    {e_totemBlockTypeDraggable, "Draggable" },
    {e_totemBlockTypeGenericSensor, "Generic Sensor" },
    {e_totemBlockTypeNormalInv, "Normal (Invulnerable)" },
    {e_totemBlockTypeEventSender, "Event Sender" },
    {e_totemBlockTypeMessage, "Message"},
    {e_totemBlockTypeCosmetic, "Cosmetic (Non Physics)" },
    {e_totemBlockTypeLight, "Light" },
    {e_totemBlockTypeBillboard, "Billboard"},
};


typedef enum {
    e_totemMaterialWood = 0,
    e_totemMaterialMetal,
    e_totemMaterialGlass,
    e_totemMaterialFloat,
    e_totemMaterialIce,
    e_totemMaterialRock,
    e_totemMaterialEarth,
    e_totemMaterialCustomTexture,
    e_totemMaterialToyWood,
    e_totemMaterialToyPlastic,
    e_totemMaterialToyMetal,
    e_totemMaterialToyRubber,
    e_totemMaterialRawPolygon,
    e_totemMaterialPlastic,
    e_totemMaterialBomb,
    e_totemMaterialJack,
    e_totemMaterialGrinder,
    e_totemMaterialLiquid,
    e_totemMaterialText,
    e_totemMaterialBouncyBomb,
    e_totemMaterial_Count,
} e_totemMaterial;


struct totemMaterialMenuItem
{
    e_totemMaterial material;
    const char* materialName;
};


const struct totemMaterialMenuItem totemMaterialMenuItems[] = {
    { e_totemMaterialWood, "Wood" },
    { e_totemMaterialMetal, "Metal" },
    { e_totemMaterialGlass, "Glass" },
    { e_totemMaterialFloat, "Floaty" },
    { e_totemMaterialIce, "Ice" },
    { e_totemMaterialRock, "Rock" },
    { e_totemMaterialEarth, "Earth" },
    { e_totemMaterialCustomTexture, "Custom Texture" },
    { e_totemMaterialToyWood, "Toy Game - Wood" },
    { e_totemMaterialToyPlastic, " Toy Game - Plastic" },
    { e_totemMaterialToyMetal, "Toy Game - Metal" },
    { e_totemMaterialToyRubber, "Toy Game - Rubber" },
    { e_totemMaterialRawPolygon, "Raw Polygon" },
    { e_totemMaterialPlastic, "Plastic" },
    { e_totemMaterialBomb, "Bomb" },
    { e_totemMaterialJack, "Jack" },
    { e_totemMaterialGrinder, "Grinder" },
    { e_totemMaterialLiquid, "Liquid" },
    {e_totemMaterialText, "Text Block" },
    {e_totemMaterialBouncyBomb, "Bouncy Bomb"},
};


typedef enum
{
    e_totemJointTypeDistance = 0,
    e_totemJointTypeRevolute,
    e_totemJointTypePrismatic,
    e_totemJoinType_Count,
} e_totemJointType;


struct totemJointTypeMenuItem
{
    e_totemJointType jointType;
    const char* jointTypeName;
};


const struct totemJointTypeMenuItem totemJointTypeMenuItems[] = {
    {e_totemJointTypeDistance, "Distance Joint (2 Bodies, 2 points)" },
    {e_totemJointTypeRevolute, "Revolute (2 Bodies, 1 point)" },
    {e_totemJointTypePrismatic, "Prismatic (2 Bodies, 2 points)" },
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
    e_totemEventTypeFireGun = 0,
    e_totemEventTypeShowMessage,
    e_totemEventTypeJointMotorStartPos,
    e_totemEventTypeJointMotorStartNeg,
    e_totemEventTypeJointMotorStop,
    e_totemEventType_Count,
} e_totemEventType;


struct totemEventTypeMenuItem
{
    e_totemEventType eventType;
    const char* eventTypeName;
};


const struct totemEventTypeMenuItem totemEventTypeMenuItems[] = {
    {e_totemEventTypeFireGun, "Tap Fire Gun" },
    {e_totemEventTypeShowMessage, "Tap Show Message" },
    {e_totemEventTypeJointMotorStartPos, "Joint Motor start (+)"},
    {e_totemEventTypeJointMotorStartNeg, "Joint motor start (-)"},
    {e_totemEventTypeJointMotorStop, "Joint Motor Stop"},
};


#endif