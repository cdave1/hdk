#include "hdInterfaceLayer.h"


hdInterfaceLayer::hdInterfaceLayer(hdGameWorld* gameWorld, const hdAABB& aabb) : hdGameObject(gameWorld, aabb)
{
	m_buttons = new hdPointerList<hdButton, 64>();
	m_isEnabled = true;
}


hdInterfaceLayer::~hdInterfaceLayer()
{
	delete m_buttons;
}


int hdInterfaceLayer::AddButton(hdButton* button)
{
	return m_buttons->Add(button);
}

bool hdInterfaceLayer::RemoveButton(hdButton* button)
{
	return m_buttons->Remove(button);
}


// Check whether the touch event was in the bounds of any buttons.
bool hdInterfaceLayer::MouseDown(float x, float y)
{
	if (m_isEnabled == false) return false;
	
	bool down = false;
	
	for (int i = 0; i < m_buttons->GetItemCount(); i++)
	{
		//if (m_buttons->GetItems()[i]->MouseDown(x, y)) down = true;
	}
	return down;
}


void hdInterfaceLayer::MouseOver(float x, float y)
{
	if (m_isEnabled == false) return;
	
	for (int i = 0; i < m_buttons->GetItemCount(); i++)
	{
		//m_buttons->GetItems()[i]->MouseOver(x, y);
	}
}


// Returns the address of the first button to respond to the event.
hdButton* hdInterfaceLayer::MouseUp(float x, float y)
{
	if (m_isEnabled == false) return NULL;
	
	// HACK HACK HACK
	// Incorrect use of mouse up
	for (int i = 0; i < m_buttons->GetItemCount(); i++)
	{
		//if (m_buttons->GetItems()[i]->MouseUp(x, y))
		//{
			//return m_buttons->GetItems()[i];
		//}
	}
	return NULL;
}


// Check whether the double click was in the bounds of any buttons.
void hdInterfaceLayer::HandleDoubleTouch(float x, float y)
{
	if (m_isEnabled == false) return;
}


void hdInterfaceLayer::Draw() const
{
	if (this->IsHidden()) return;
	
	m_buttons->Map(&hdButton::Draw);
}