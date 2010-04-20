/*
 *  hdTypedefList.h
 *  AnimationEngine
 *
 *  Created by david on 11/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

/*
 *  hdTypedefList.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_COLLECTIONS_TYPEDEF_LIST_H
#define HD_COLLECTIONS_TYPEDEF_LIST_H

#include "hdCollections/hdCollectionSettings.h"

template<typename T, int Max>
class hdTypedefList
	{
	private:
		T m_list[Max];
		
		int m_itemCount;
		
	public:
		hdTypedefList()
		{
			memset(m_list, 0, sizeof(m_list));
			m_itemCount = 0;
		}
		
		int Add(const T& item);
		
		int AddAtIndex(const T& item, const int index);
		
		bool Replace(const T& item, const int index);
		
		bool RemoveAtIndex(const int index);
		
		bool RemoveAll();
		
		T* GetItems();
		
		const int GetItemCount() const;
		
		bool IsEmpty() const;
		
		bool IsFull() const;
		
		T& operator [] (unsigned int i);
		
		T operator [] (unsigned int i) const;
		
		//void Map(void (*func)(T& item) const);
		
		void Map(void (*func)(T& item));
	};


template<typename T, int Max>
inline int hdTypedefList<T, Max>::Add(const T& item)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	
	if (m_itemCount == Max) 
	{
		return HD_COLLECTIONS_ERROR_FULL;
	}
	
	return this->AddAtIndex(item, (const int)m_itemCount);
}


template<typename T, int Max>
inline int hdTypedefList<T, Max>::AddAtIndex(const T& item, const int index)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	
	if (m_itemCount == Max) 
	{
		return HD_COLLECTIONS_ERROR_FULL;
	}
	
	if (index > m_itemCount) 
	{
		return HD_COLLECTIONS_ERROR_INVALID_INDEX;
	}
	
	if (index < 0) 
	{
		return HD_COLLECTIONS_ERROR_INVALID_INDEX;
	}
	
	// Make room for the new item; ignore if index is the last item
	for (int i = m_itemCount; i >= index && i < (Max - 1); --i)
	{
		m_list[i+1] = m_list[i];
	}
	
	m_list[index] = item;
	++m_itemCount;
	
	return index;
}


template<typename T, int Max>
inline bool hdTypedefList<T, Max>::Replace(const T& item, const int index)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	if (index >= m_itemCount) return false;
	if (index < 0) return false;
	
	m_list[index] = item;
	return true;
}


template<typename T, int Max>
inline bool hdTypedefList<T, Max>::RemoveAtIndex(const int index)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	if (m_itemCount == 0) return false;
	if (index > m_itemCount) return false;
	if (index < 0) return false;
	
	// move everything down
	for (int i = index; i < m_itemCount-1; ++i)
	{
		m_list[i] = m_list[i+1];
	}
	--m_itemCount;
	return true;
}
	
	
template<typename T, int Max>
inline bool hdTypedefList<T, Max>::RemoveAll()
{
	m_itemCount = 0;
	memset(m_list, 0, sizeof(m_list));
	return true;
}


template<typename T, int Max>
inline T* hdTypedefList<T, Max>::GetItems()
{
	return m_list;
}


template<typename T, int Max>
inline const int hdTypedefList<T, Max>::GetItemCount() const
{
	return m_itemCount;
}


template<typename T, int Max>
inline bool hdTypedefList<T, Max>::IsEmpty() const
{
	return m_itemCount == 0;
}


template<typename T, int Max>
inline bool hdTypedefList<T, Max>::IsFull() const
{
	return m_itemCount == Max;
}


template<typename T, int Max>
inline T& hdTypedefList<T, Max>::operator [] (unsigned int i)
{
	return m_list[i];
}


template<typename T, int Max>
inline T hdTypedefList<T, Max>::operator [] (unsigned int i) const
{
	return m_list[i];
}


template<typename T, int Max>
inline void hdTypedefList<T, Max>::Map(void (*func)(T& item))
{
	for (int i = 0; i < m_itemCount; ++i)
	{
		T item = m_list[i];
		if (item != NULL)
		{
			(*func)(item);
		}
	}
}

// TODO: Map a list of functions
// TODO: Filter



#endif