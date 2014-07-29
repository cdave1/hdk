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

#ifndef _HD_POINTER_LIST_H_
#define _HD_POINTER_LIST_H_

#include <hdk/hdCollections/hdCollectionSettings.h>

// A serializable list of pointers to other objects using
// templates rather than (void *). Will give the code a bit
// more semantic meaning.

template<class T, int Max>
class hdPointerList
{
private:
    T* m_list[Max];

    int m_itemCount;

public:
    hdPointerList()
    {
        memset(m_list, 0, sizeof(m_list));
        m_itemCount = 0;
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
};


template<class T, int Max>
inline int hdPointerList<T, Max>::Add(const T *item)
{
    hdAssert(m_itemCount >= 0 && m_itemCount <= Max);

    if (m_itemCount == Max)
    {
        return HD_COLLECTIONS_ERROR_FULL;
    }

    return this->AddAtIndex(item, (const int)m_itemCount);
}


template<class T, int Max>
inline int hdPointerList<T, Max>::AddAtIndex(const T *item, const int index)
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
        m_list[i] = NULL;
    }
    m_list[index] = (T *)item;
    ++m_itemCount;
    return index;
}


template<class T, int Max>
inline bool hdPointerList<T, Max>::Remove(const T* item)
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
inline bool hdPointerList<T, Max>::RemoveAtIndex(const int index)
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
inline bool hdPointerList<T, Max>::RemoveAll()
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
inline T** hdPointerList<T, Max>::GetItems() const
{
    return (T**)&m_list;
}


template<class T, int Max>
inline const int hdPointerList<T, Max>::GetItemCount() const
{
    return m_itemCount;
}


template<class T, int Max>
inline bool hdPointerList<T, Max>::IsEmpty() const
{
    return m_itemCount == 0;
}


template<class T, int Max>
inline bool hdPointerList<T, Max>::IsFull() const
{
    return m_itemCount == Max;
}


template<class T, int Max>
inline bool hdPointerList<T, Max>::Contains(const T* item) const
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
inline void hdPointerList<T, Max>::Map(void (T::*func)(void) const)
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
inline void hdPointerList<T, Max>::Map(void (T::*func)(void))
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

#endif
