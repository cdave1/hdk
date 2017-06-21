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

#include "hdFontPolygon.h"

hdFontPolygon::hdFontPolygon() : hdReceiver()
{
    m_font = NULL;
    m_glyphs = NULL;
}


hdFontPolygon::hdFontPolygon(const char* fontFileName,
                             hdGameWorld* gameWorld) : hdReceiver(gameWorld)
{
    m_font = NULL;
    m_glyphs = NULL;

    this->PrivateInit(fontFileName, "");
    m_renderToTexture = false;
}


hdFontPolygon::hdFontPolygon(const char* fontFileName,
                             const char* text,
                             hdGameWorld* gameWorld,
                             const hdAABB& aabb) : hdReceiver(gameWorld)
{
    m_font = NULL;
    m_glyphs = NULL;

    this->PrivateInit(fontFileName, text);
    this->SetAs2DBox(aabb.lower, aabb.upper);
    this->ResetAABB();
}


hdFontPolygon::hdFontPolygon(const char* fontFileName,
                             const char* text,
                             hdGameWorld* gameWorld,
                             const float x, const float y,
                             const float w, const float h) : hdReceiver(gameWorld)
{
    m_font = NULL;
    m_glyphs = NULL;

    this->PrivateInit(fontFileName, text);
    this->SetAs2DBox(hdVec3(x,y,0.0), hdVec3(x+w,y+h,0.0));
    this->ResetAABB();
}


void hdFontPolygon::PrivateInit(const char* fontFileName,
                                const char* text)
{
    if (strlen(fontFileName) == 0)
    {
        hdError(-1, "Font: zero-length filename, name: %s. Aborting...", fontFileName);
        assert(false);
    }

    m_renderToTexture = true;

    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 1.0f;
    m_colorTint[3] = 1.0f;

    m_renderedCenter.Set(0,0,0);

    m_font = hdFontManager::Instance()->FindFont(fontFileName);

    m_scale = 1.0f;

    m_lineHeightScale = 0.8f;

    m_paragraphPadding = 0.5f;

    if (strlen(text) > 0)
    {
        SetText(text);
    }

    m_alignment = e_fontAlignmentLeft;
}



hdFontPolygon::~hdFontPolygon()
{
    if (m_glyphs != NULL) delete m_glyphs;
}


bool hdFontPolygon::MouseDown(float x, float y)
{
    return 0;
}


bool hdFontPolygon::MouseOver(float x, float y)
{
    return 0;
}


bool hdFontPolygon::MouseUp(float x, float y)
{
    return 0;
}


void hdFontPolygon::SetRenderToTexture(const bool renderToTexture)
{
}


void hdFontPolygon::SetText(const char* text)
{
    snprintf(m_text, kMaxFontTextSize, "%s", text);

    TextIntoGlyphs();
}


void hdFontPolygon::SetTextFormatted(const char *fmt, ...)
{
    va_list		argptr;
    static char	msg[kMaxFontTextSize];

    va_start(argptr, fmt);
    (void)vsnprintf(msg, sizeof( msg ), fmt, argptr);
    va_end(argptr);

    msg[sizeof(msg) - 1] = '\0';

    snprintf(m_text, kMaxFontTextSize, "%s", msg);

    TextIntoGlyphs();
}


#define NEWLINE (uint8)'\n'
#define SPACECHAR (uint8) ' '

// Preprocess the text so we don't have to do it on every draw.
//
// TODO: Unicode support
void hdFontPolygon::TextIntoGlyphs()
{
    short i;

    uint8 ascii;
    int len;

    float xWidth, yOffset, maxXWidth, aabbWidth, aabbHeight;
    hdVec3 worldCenter;

    int lineOffs;
    float drawableLineHeight;

    FluidFontCharHeader glyph;
    static float linewidths[32];
    int lines;
    glyph_t g;


    if (m_glyphs == NULL)
        m_glyphs = new hdTypedefList<glyph_t, kMaxFontTextSize>();

    m_glyphs->RemoveAll();

    if (m_font == NULL)
    {
        hdPrintf("No font found.\n");
        return;
    }

    if (m_font->m_texture == NULL)
    {
        hdPrintf("No font texture.\n");
    }

    len = strlen(m_text);
    maxXWidth = 0.0f;

    // HACK HACK HACK
    if (m_scale < 0.2f)
    {
        drawableLineHeight = ((m_lineHeightScale * m_font->m_lineHeight) * m_scale);
    }
    else
    {
        drawableLineHeight = ceil((m_lineHeightScale * m_font->m_lineHeight) * m_scale);
    }

    worldCenter = hdVec3(0,0,0);
    aabbWidth = ((hdAABB)m_aabb).Width();
    aabbHeight = ((hdAABB)m_aabb).Height();

    if (m_alignment == e_fontAlignmentLeft)
    {
        xWidth = 0.0f;
        yOffset = 0.0f;
        lineOffs = 0;

        for (i = 0; i < len; ++i)
        {
            ascii = (uint8)m_text[i];
            glyph = m_font->fileChars[ascii];

            if (ascii == NEWLINE ||
                (ascii == SPACECHAR &&
                 (xWidth + (glyph.screenWidth * m_scale)) > aabbWidth)
                )
            {
                // HACK HACK HACK
                // if xWidth is 0.0 then assume previous char was a newline too.
                if (xWidth == 0.0f)
                {
                    yOffset += (m_paragraphPadding * drawableLineHeight);
                }
                else
                {
                    yOffset += (drawableLineHeight);
                }

                xWidth = 0.0f;
                ++lineOffs;
            }
            else
            {
                g.glyph = &(m_font->fileChars[ascii]);
                g.pos.Set(xWidth + (-0.5f * aabbWidth),
                          (0.5f * aabbHeight) - yOffset, 0);
                g.w = ((float)glyph.screenWidth) * m_scale;
                g.h = ((float)glyph.screenHeight) * m_scale;

                m_glyphs->Add(g);

                xWidth += ((float)glyph.screenWidth) * m_scale;
                maxXWidth = hdMax(maxXWidth, xWidth);
            }
        }
    }
    else if (m_alignment == e_fontAlignmentRight)
    {
        xWidth = 0.0f;
        yOffset = 0.0f;
        lineOffs = 0;

        // Preprocess: find the number of lines
        for (i = 0; i < len; ++i)
        {
            ascii = (uint8)m_text[i];
            glyph = m_font->fileChars[ascii];

            if (ascii == NEWLINE ||
                (ascii == SPACECHAR &&
                 (xWidth + (glyph.screenWidth * m_scale)) > aabbWidth)
                )
            {
                // HACK HACK HACK
                // if xWidth is 0.0 then assume previous char was a newline too.
                if (xWidth == 0.0f)
                {
                    yOffset += (m_paragraphPadding * drawableLineHeight);
                }
                else
                {
                    yOffset += (drawableLineHeight);
                }

                xWidth = 0.0f;
                ++lineOffs;
            }
        }

        xWidth = 0.0f;

        // Work backwards from the last char.
        for (i = len - 1; i >= 0; --i)
        {
            ascii = (uint8)m_text[i];
            glyph = m_font->fileChars[ascii];

            if (ascii == NEWLINE ||
                (ascii == SPACECHAR &&
                 (xWidth + (glyph.screenWidth * m_scale)) > aabbWidth)
                )
            {
                yOffset -= (drawableLineHeight);

                xWidth = 0.0f;
                --lineOffs;
            }
            else
            {
                xWidth += (glyph.screenWidth * m_scale);

                g.glyph = &(m_font->fileChars[ascii]);
                g.pos.Set((0.5f * aabbWidth) - xWidth,
                          (0.5f * aabbHeight) - (lineOffs * drawableLineHeight), 0);
                g.w = ((float)glyph.screenWidth) * m_scale;
                g.h = ((float)glyph.screenHeight) * m_scale;

                m_glyphs->Add(g);

                maxXWidth = hdMax(maxXWidth, xWidth);
            }
        }
    }
    else if (m_alignment == e_fontAlignmentCenter)
    {
        /*
         * Preprocess: find all linewidths first.
         */

        // Max lines is 32.

        linewidths[0] = 0.0f;
        lines = 0;

        for (i = 0; i < len; ++i)
        {
            ascii = (uint8)m_text[i];
            glyph = m_font->fileChars[ascii];

            if (ascii == NEWLINE ||
                (ascii == SPACECHAR &&
                 (linewidths[lines] + (glyph.screenWidth * m_scale)) > aabbWidth)
                )
            {
                ++lines;
                linewidths[lines] = 0.0f;
            }
            else
            {
                linewidths[lines] += (glyph.screenWidth * m_scale);
            }
        }

        xWidth = (worldCenter.x - (linewidths[0]/2.0f)) - (-0.5f * aabbWidth);
        yOffset = 0.0f;
        lineOffs = 0;

        for (i = 0; i < len; ++i)
        {
            ascii = (uint8)m_text[i];
            glyph = m_font->fileChars[ascii];

            if (ascii == NEWLINE ||
                (ascii == SPACECHAR &&
                 (xWidth + (glyph.screenWidth * m_scale)) > aabbWidth)
                )
            {
                // HACK HACK HACK
                // if xWidth is 0.0 then assume previous char was a newline too.
                if (xWidth == 0.0f)
                {
                    yOffset += (m_paragraphPadding * drawableLineHeight);
                }
                else
                {
                    yOffset += (drawableLineHeight);
                }

                ++lineOffs;
                xWidth = (worldCenter.x - (linewidths[lineOffs]/2.0f)) - (-0.5f * aabbWidth);
            }
            else
            {
                g.glyph = &(m_font->fileChars[ascii]);
                g.pos.Set(xWidth + (-0.5f * aabbWidth),
                          (0.5f * aabbHeight) - yOffset, 0);
                g.w = ((float)glyph.screenWidth) * m_scale;
                g.h = ((float)glyph.screenHeight) * m_scale;

                m_glyphs->Add(g);

                xWidth += (glyph.screenWidth * m_scale);
                maxXWidth = hdMax(maxXWidth, xWidth);
            }
        }
    }
}


void hdFontPolygon::AlignCenter()
{
    m_alignment = e_fontAlignmentCenter;
    TextIntoGlyphs();
}


void hdFontPolygon::AlignRight()
{
    m_alignment = e_fontAlignmentRight;
    TextIntoGlyphs();
}


void hdFontPolygon::AlignLeft()
{
    m_alignment = e_fontAlignmentLeft;
    TextIntoGlyphs();
}


void hdFontPolygon::RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector)
{
    if (m_canRotate)
    {
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -this->GetWorldCenter());
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(),
                            this->GetWorldCenter() + (transVector - this->GetWorldCenter()));

        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
        m_obb.transform.rotation.Set(rotVector.x, rotVector.y, rotVector.z);
    }
    else
    {
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(),
                            -m_obb.transform.translation);
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(),
                            m_obb.transform.translation + (transVector - m_obb.transform.translation));

        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
    }

    this->ResetAABB();
}



void hdFontPolygon::SetAs2DBox(const float& x, const float& y, const float& w, const float& h)
{
    ((hdPolygon*)this)->SetAs2DBox(x, y, w, h);
    this->ResetAABB();
    TextIntoGlyphs();
}

void hdFontPolygon::SetAs2DBox(const hdVec2& aa, const hdVec2& bb)
{
    ((hdPolygon*)this)->SetAs2DBox(aa, bb);
    this->ResetAABB();
    TextIntoGlyphs();
}

void hdFontPolygon::SetAs2DBox(const hdVec3& aa, const hdVec3& bb)
{
    ((hdPolygon*)this)->SetAs2DBox(aa, bb);
    this->ResetAABB();
    TextIntoGlyphs();
}


void hdFontPolygon::Draw() const
{
    unsigned i;
    glyph_t g;
    hdVec3 worldCenter;

    if (m_glyphs == NULL) return;
    if (this->IsHidden()) return;
    if (m_font == NULL) return;
    if (m_font->m_texture == NULL) return;

    glEnable(GL_TEXTURE_2D);
    hdglBindTexture(m_font->m_texture);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    worldCenter = ((hdFontPolygon *)this)->GetWorldCenter();

    if (m_canRotate)
    {
        glTranslatef(worldCenter.x,
                     worldCenter.y,
                     0.0f);

        glRotatef(m_obb.transform.rotation.z * (180.0f/hd_pi),
                  0.0f, 0.0f, 1.0f);

        glTranslatef(-worldCenter.x,
                     -worldCenter.y,
                     0.0f);
    }

    glTranslatef(worldCenter.x,
                 worldCenter.y,
                 0.0f);

    hdglBegin(GL_QUADS);
    hdglColor4ub(m_ubTint.r, m_ubTint.g, m_ubTint.b,
                 hdClamp(m_ubTint.a, (uint8)0, (uint8)(255 * m_alpha)));

    for (i = 0; i < m_glyphs->GetItemCount(); ++i)
    {
        g = m_glyphs->GetItems()[i];

        RenderGlyph(*(g.glyph), g.pos.x, g.pos.y, g.w, g.h);
    }

    hdglEnd();

#ifdef DEBUG_FONTS
    /*
     * Draw a bounding box around the font aabb.
     */
    hdglBindTexture(NULL);

    hdglBegin(GL_QUADS);
    hdglColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    hdglVertex2f(m_aabb.lower.x, m_aabb.lower.y);
    hdglVertex2f(m_aabb.lower.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.lower.y);
    hdglEnd();
#endif

    glPopMatrix();
}


void hdFontPolygon::RenderGlyph(const FluidFontCharHeader& charHeader,
                                const float xPos, const float yPos,
                                const float w, const float h) const
{
    hdglColor4ub(m_ubTint.r, m_ubTint.g, m_ubTint.b,
                 hdClamp(m_ubTint.a, (uint8)0, (uint8)(255 * m_alpha)));

    hdglTexCoord2f(charHeader.texCoords[0].x, charHeader.texCoords[0].y);
    hdglVertex2f(xPos, yPos - h);

    hdglTexCoord2f(charHeader.texCoords[1].x, charHeader.texCoords[1].y);
    hdglVertex2f(xPos, yPos);

    hdglTexCoord2f(charHeader.texCoords[3].x, charHeader.texCoords[3].y);
    hdglVertex2f(xPos + w, yPos);

    hdglTexCoord2f(charHeader.texCoords[2].x, charHeader.texCoords[2].y);
    hdglVertex2f(xPos + w, yPos - h);

#ifdef DEBUG_FONTS
    hdglEnd();

    hdPrintf("\t%c: x offset: %d, y offset: %d w: %d h: %d\n", m_text[i],
             charHeader.xOffset, charHeader.yOffset,
             charHeader.byteWidth, charHeader.byteHeight);
    hdPrintf("\t%c: y bottom: %f, y top: %f\n", m_text[i],
             (m_aabb.upper.y - (charHeader.byteHeight + charHeader.yOffset)),
             (m_aabb.upper.y - (charHeader.byteHeight + charHeader.yOffset)) +
             charHeader.byteHeight);

    int currentTexNum;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexNum);
    glBindTexture(GL_TEXTURE_2D, 0);

    hdglBegin(GL_QUADS);

    hdglColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    hdglVertex2f(xPos, yPos - (charHeader.screenHeight * m_scale));
    
    hdglVertex2f(xPos, yPos);
    
    hdglVertex2f(xPos + (charHeader.screenWidth * m_scale), yPos);
    
    hdglVertex2f(xPos + (charHeader.screenWidth * m_scale), 
                 yPos - (charHeader.screenHeight * m_scale));
    hdglEnd();
    
    glBindTexture(GL_TEXTURE_2D, currentTexNum);
    
    hdglBegin(GL_QUADS);
    
#endif
}
