#include <string>

#include "glad/glad.h"

#include "aeregui.h"
#include "shaders/aeregui_shaders.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace AereGui;

GuiHelper::GuiHelper(const char* font,
                     uint32_t font_idx,
                     uint32_t screenSz_idx,
                     uint32_t text_idx)
{
    loadFont(font);
    ta.font.bind = font_idx;
    std::string ft = FONTTEXTURE;
    ft.replace(FONTTEXTURE.find("@"), 1, std::to_string(font_idx));

    glCreateBuffers(1, &ub.screenSz.buf);
    glNamedBufferStorage(ub.screenSz.buf, sizeof(int) * 2, glm::value_ptr(screenSz), GL_DYNAMIC_STORAGE_BIT);
    ub.screenSz.bind = screenSz_idx;
    std::string ss = SCREENSZBUF;
    ss.replace(SCREENSZBUF.find("@"), 1, std::to_string(screenSz_idx));

    glCreateBuffers(1, &ssb.text.buf);
    glNamedBufferStorage(ssb.text.buf, (1 << 16) / 16 * sizeof(Character), nullptr, GL_DYNAMIC_STORAGE_BIT);
    ssb.text.bind = text_idx;
    std::string tb = TEXTBUF;
    tb.replace(TEXTBUF.find("@"), 1, std::to_string(text_idx));

    shaders.text = std::make_unique<gShader>();
    createShader(shaders.text.get(),
                 VERSION_HEADER + ss + tb + TEXTVERT,
                 VERSION_HEADER + ft + TEXTFRAG);

    shaders.text->use();
    glUniform1f(shaders.text->getLocation("fontPx"), fontPx);
}

//#TODO: they all have to be different binding indices right now
void GuiHelper::bind()
{
    glBindTextureUnit(ta.font.bind, ta.font.buf);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssb.text.bind, ssb.text.buf);
    glBindBufferBase(GL_UNIFORM_BUFFER, ub.screenSz.bind, ub.screenSz.buf);
}

void GuiHelper::setMousePos(glm::vec2 pos)
{
    mousePos = pos;
}

void GuiHelper::setScreenSz(glm::ivec2 size)
{
    screenSz = size;
    glNamedBufferSubData(ub.screenSz.buf, 0, sizeof(int32_t) * 2, glm::value_ptr(screenSz)); 
}

void GuiHelper::render()
{
    shaders.text->use();
    glNamedBufferSubData(ssb.text.buf, 0, sizeof(Character) * data.text.size(), data.text.data()); 
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.text.size());
}

void GuiHelper::clear()
{
    data.text.clear();
}

void GuiHelper::addText(const char* text,
                        glm::vec2 pos, float scale, glm::vec4 col, bool center)
{
    int numchars = strlen(text);
    int curSize = data.text.size();
    data.text.resize(data.text.size() + numchars);

    float currx = 0;
    if (center)
    {
        for (int idx = 0; idx < numchars; idx++)
        {
            currx += (charMap[text[idx]].advance >> 6) * scale;
        }
        pos.x -= currx / 2.0;
        pos.y -= fontPx / 2.0;
    }

    currx = pos.x;

    for (int idx = 0; idx < numchars; idx++)
    {
        const CharInfo& info = charMap[text[idx]];
        Character& c = data.text[idx + curSize];
        c = {
            .vector = {
                currx + info.bearing.x * scale,
                pos.y - (info.size.y - info.bearing.y) * scale,
                info.size.x * scale,
                info.size.y * scale,
            },
            .col = col,
            .layer = info.layer,
            .scale = scale,
        };

        currx += (info.advance >> 6) * scale;
    }
    
    //returns rectangular area taken up by text
    /*
    return {pos.x / (screenSz.x/2.0),
            pos.y / (screenSz.y/2.0),
            (currx - pos.x) / (screenSz.x/2.0),
            fontPx / (screenSz.y/2.0)};
    */
}

void GuiHelper::loadFont(const char* font)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        assert(false);
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/unifont.ttf", 0, &face))
    {
        printf("ERROR::FREETYPE: Failed to load font\n");
        assert(false);
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);

    fontPx = 48;
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &ta.font.buf);
    glTextureStorage3D(ta.font.buf, 1, GL_R8, fontPx, fontPx, 128);

    unsigned char * empty = new unsigned char[fontPx * fontPx];
    memset(empty, 0, fontPx * fontPx * sizeof(unsigned char));

    int currIdx = 0;
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
            continue;
        }

        charMap[c] = {
            .layer = currIdx,
            .size = {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            .bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top},
            .advance = (uint32_t)face->glyph->advance.x,
        };

        glTextureSubImage3D(ta.font.buf, 0, 0, 0, currIdx, fontPx, fontPx, 1, GL_RED, GL_UNSIGNED_BYTE, empty);
        glTextureSubImage3D(ta.font.buf, 0, 0, 0, currIdx, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        currIdx++;
    }

    glTextureParameteri(ta.font.buf, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTextureParameteri(ta.font.buf, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ta.font.buf, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(ta.font.buf, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] empty;
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}
