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

#ifndef _HD_FONT_POLYGON_H_
#define _HD_FONT_POLYGON_H_

#include <hdk/ui/hdReceiver.h>
#include <hdk/ui/hdFontManager.h>

typedef struct
{
    FluidFontCharHeader* glyph;
    hdVec3 pos;
    float w, h;
} glyph_t;


class hdFontPolygon : public hdReceiver
{
public:
    hdFontPolygon();

    hdFontPolygon(const char* fontFileName,
                  hdGameWorld* gameWorld);

    hdFontPolygon(const char* fontFileName,
                  const char* text,
                  hdGameWorld* gameWorld,
                  const hdAABB& aabb);

    hdFontPolygon(const char* fontFileName,
                  const char* text,
                  hdGameWorld* gameWorld,
                  const float x, const float y,
                  const float w, const float h);

    ~hdFontPolygon();

    void SetAs2DBox(const float& x, const float& y, const float& w, const float& h);

    void SetAs2DBox(const hdVec2& aa, const hdVec2& bb);

    void SetAs2DBox(const hdVec3& aa, const hdVec3& bb);

    bool MouseDown(float x, float y);

    bool MouseOver(float x, float y);

    bool MouseUp(float x, float y);

    void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);

    void Draw() const;

    void SetRenderToTexture(const bool renderToTexture);

    const char* GetText() const;

    void SetText(const char* text);

    void SetTextFormatted(const char *fmt, ...);

    void SetTint(float r, float g, float b, float a);

    float* GetTint() const;

    void AlignCenter();

    void AlignRight();

    void AlignLeft();

    const float GetScale() const;

    void SetScale(const float scale);

    const float GetLineHeightScale() const;

    void SetLineHeightScale(const float scale);


    const hdVec3 GetRenderedCenter() const;

    void SetRenderedCenter(const float x, const float y);

private:
    void PrivateInit(const char* fontFileName,
                     const char* text);

    // Preprocess the text so we don't have to
    void TextIntoGlyphs();

    void RenderGlyph(const FluidFontCharHeader& charHeader,
                     const float xPos, const float yPos,
                     const float w, const float h) const;

    hdFont *m_font;

    char m_text[kMaxFontTextSize];

    hdTypedefList<glyph_t, kMaxFontTextSize>* m_glyphs;

    bool m_renderToTexture;

    float m_colorTint[4];

    hdColor4 m_ubTint;

    e_fontAlignment m_alignment;

    float m_scale;

    float m_lineHeightScale;

    float m_paragraphPadding;

    hdVec3 m_renderedCenter;
};


inline const char* hdFontPolygon::GetText() const
{
    return m_text;
}


inline void hdFontPolygon::SetTint(float r, float g, float b, float a)
{
    m_colorTint[0] = r;
    m_colorTint[1] = g;
    m_colorTint[2] = b;
    m_colorTint[3] = a;

    m_ubTint.Setf(r,g,b,a);
}


inline const float hdFontPolygon::GetScale() const
{
    return m_scale;
}


inline void hdFontPolygon::SetScale(const float scale)
{
    m_scale = scale;
}


inline const hdVec3 hdFontPolygon::GetRenderedCenter() const
{
    return m_renderedCenter;
}


inline void hdFontPolygon::SetRenderedCenter(const float x, const float y)
{
    m_renderedCenter.Set(x, y, 0.0f);
}


inline const float hdFontPolygon::GetLineHeightScale() const
{
    return m_lineHeightScale;
}


inline void hdFontPolygon::SetLineHeightScale(const float scale)
{
    m_lineHeightScale = scale;
}

#endif
