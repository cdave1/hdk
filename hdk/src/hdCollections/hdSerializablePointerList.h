/*
 *  hdSerializablePointerList.h
 *  AnimationEngine
 *
 *  Created by david on 31/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_SERIALIZABLE_POINTER_LIST_H_
#define _HD_SERIALIZABLE_POINTER_LIST_H_ 

#include "hdCollections/hdCollectionSettings.h"

// A serializable list of pointers to other objects using
// templates rather than (void *). Will give the code a bit
// more semantic meaning.

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

template<class T, int Max>
class hdSerializablePointerList
{
private:
	T* m_list[Max];
	
	int m_itemCount;
	
public:
	hdSerializablePointerList()
	{
		memset(m_list, 0, sizeof(m_list));
		m_itemCount = 0;
	}
	
	~hdSerializablePointerList()
	{
		//this->RemoveAll();
	}
	
	int Add(const T* item);
	
	int AddAtIndex(const T* item, const int index);
	
	bool Remove(const T* item);
	
	bool RemoveAtIndex(const int index);
	
	bool RemoveAll();
	
	T** GetItems() const;
	
	const int GetItemCount() const;
	
	bool IsEmpty() const;
	
	bool IsFull() const;
	
	bool Contains(const T* item) const;
	
	void Map(void (T::*func)(void) const);
	
	void Map(void (T::*func)(void));
	
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & m_itemCount;
		ar & m_list;
	}
	
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & m_itemCount;
		ar & m_list;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};


template<class T, int Max>
inline int hdSerializablePointerList<T, Max>::Add(const T *item)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	
	if (m_itemCount == Max) return -1;
	
	return this->AddAtIndex(item, (const int)m_itemCount);
}


template<class T, int Max>
inline int hdSerializablePointerList<T, Max>::AddAtIndex(const T *item, const int index)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	if (m_itemCount == Max) return -1;
	if (index > m_itemCount) return -1;
	if (index < 0) return -1;
	
	// Make room for the new item
	for (int i = m_itemCount; i >= index && i < (Max - 1); --i)
	{
		m_list[i+1] = m_list[i];
		m_list[i] = NULL;
	}
	m_list[index] = (T *)item;
	++m_itemCount; 
	return index;
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::Remove(const T* item)
{
	if (item == NULL) return false;
	
	// find the index
	int i = 0;
	while (i <= m_itemCount) 
	{ 
		if (item == m_list[i]) break;
		++i;
	}
	if (i >= m_itemCount) return false;
	return this->RemoveAtIndex(i);
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::RemoveAtIndex(const int index)
{
	hdAssert(m_itemCount >= 0 && m_itemCount <= Max);
	if (m_itemCount == 0) return false;
	if (index >= m_itemCount) return false;
	if (index < 0) return false;
	
	delete m_list[index];
	m_list[index] = NULL;
	
	// move everything down
	for (int i = index; i < m_itemCount-1; ++i)
	{
		m_list[i] = m_list[i+1];
	}
	m_list[m_itemCount-1] = NULL;
	--m_itemCount;
	return true;
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::RemoveAll()
{
	// cannot simply set the m_itemCount to zero - need to delete things.
	for (int i = 0; i < m_itemCount; ++i)
	{	
		delete m_list[i];
		m_list[i] = NULL;
	}
	m_itemCount = 0;
	return true;
}


template<class T, int Max>
inline T** hdSerializablePointerList<T, Max>::GetItems() const
{
	return (T**)&m_list;
}


template<class T, int Max>
inline const int hdSerializablePointerList<T, Max>::GetItemCount() const
{
	return m_itemCount;
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::IsEmpty() const
{
	return m_itemCount == 0;
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::IsFull() const
{
	return m_itemCount == Max;
}


template<class T, int Max>
inline bool hdSerializablePointerList<T, Max>::Contains(const T* item) const
{
	if (item == NULL) return false;
	if (m_itemCount == 0) return false;
	
	int i = 0;
	bool b = false;
	while (i <= m_itemCount) 
	{ 
		if (item == m_list[i]) 
		{
			b = true;
			break;
		}
		++i;
	}
	return b;
}


template<class T, int Max>
inline void hdSerializablePointerList<T, Max>::Map(void (T::*func)(void) const)
{
	for (int i = 0; i < m_itemCount; ++i)
	{
		T* item = (T *)m_list[i];
		if (item != NULL)
		{
			(item->*func)();
		}
	}
}


// Lazy man's overloading
template<class T, int Max>
inline void hdSerializablePointerList<T, Max>::Map(void (T::*func)(void))
{
	for (int i = 0; i < m_itemCount; ++i)
	{
		T* item = (T *)m_list[i];
		if (item != NULL)
		{
			(item->*func)();
		}
	}
}

// TODO: Map a list of functions
// TODO: Filter



#endif