#ifndef _HD_INTERFACE_LAYER_H_
#define _HD_INTERFACE_LAYER_H_

#include "hdGame.h"
#include "hdGraphicsLibrary.h"
#include "hdMath.h"
#include "hdCollections.h"

#include "hdInterface.h"

// Defines a flat space that contains interface items.
//
// The layer is responsible for distributing events to children.

class hdInterfaceLayer : public hdDrawable, public hdGameObject
{
public:
	hdInterfaceLayer(hdGameWorld* gameWorld, const hdAABB& aabb);
	
	~hdInterfaceLayer();
	
	int AddButton(hdButton* button);
	
	bool RemoveButton(hdButton* button);
	
	//void AddImage(hdUIImage* image);
	
	void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) {}
	
	void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) {}
	
	void ResetOBB() {}
	
	hdVec3* GetVertices()
	{
		return m_obb.vertices;
	}
	
	const int GetVertexCount() const
	{
		return m_obb.GetVertexCount();
	}
	
	void ApplyTransform(const hdTransform& transform)
	{
		m_obb.transform = transform;
		ResetOBB();
	}
	
	const bool isEnabled()
	{
		return m_isEnabled;
	}
	
	void Disable()
	{
		m_isEnabled = false;
	}
	
	void Enable()
	{
		m_isEnabled = true;
	}
	
	
	// Check whether the touch event was in the bounds of any buttons.
	bool MouseDown(float x, float y);
	
	void MouseOver(float x, float y);
	
	hdButton* MouseUp(float x, float y);
	
	
	// Check whether the double click was in the bounds of any buttons.
	void HandleDoubleTouch(float x, float y);
	
	void Draw() const;
	
	
private:
	//hdPointerList<hdUIImage, 64>* m_images;
	
	hdPointerList<hdButton, 64>* m_buttons;
	
	bool m_isEnabled;
};



#endif