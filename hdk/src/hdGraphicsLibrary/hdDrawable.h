#ifndef HD_DRAWABLE_H
#define HD_DRAWABLE_H

//#include "hdOrthographicProjection.h"

class hdDrawable 
{
public:
	hdDrawable() 
	{
		//m_projection = NULL;
	}
	
	
	virtual void Draw () const = 0;
	
	virtual void DrawSpecial() const {}
	
	//const hdOrthographicProjection* GetProjection() const;
	
	
	//void SetProjection(hdOrthographicProjection* projection);
	
	
private:
	//hdOrthographicProjection *m_projection;
};


//inline const hdOrthographicProjection* hdDrawable::GetProjection() const
//{
//	return m_projection;
//}
/*
inline void hdDrawable::SetProjection(hdOrthographicProjection* projection)
{
	m_projection = projection;
}
*/

#endif