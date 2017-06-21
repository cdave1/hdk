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

#ifndef _HDK_CHAPTER_BLOCK_H_
#define _HDK_CHAPTER_BLOCK_H_

#include <hdk/game.h>

class ChapterBlock : public totemBlock
{
public:
    ChapterBlock(totemBlock *parent, const char *chapterTitle, const bool isLocked, const float completionRatio);

    ~ChapterBlock();

    void Step();

    void Draw() const;

private:
    void InitInterface();

    totemBlock *m_parent;

    hdUIContainer *m_container;

    hdFontPolygon *m_chapterFont;

    hdUIImage *m_lockImage;
    
    char m_chapterTitle[128];
    
    bool m_isLocked;
    float m_completionRatio;
};

#endif
