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

#ifndef HD_COLLECTIONS_TYPEDEF_LIST_H
#define HD_COLLECTIONS_TYPEDEF_LIST_H

#include <hdk/collections/hdCollectionSettings.h>

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

    const T Top() const;

    const int GetItemCount() const;

    bool IsEmpty() const;

    bool IsFull() const;

    T& operator [] (unsigned int i);

    T operator [] (unsigned int i) const;

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
const T hdTypedefList<T, Max>::Top() const
{
    if (m_itemCount == 0) return NULL;
    return m_list[m_itemCount - 1];
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

#endif
