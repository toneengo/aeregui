#include "glad/glad.h"
#include "context.h"
#include "widget.h"
#include "screen.h"

#include <glm/gtc/type_ptr.hpp>
#include "shaders/aeregui_shaders.hpp"

using namespace AereGui;

void UIContext::render()
{
    m_screen->draw(m_gl_context);
}

UIContext::UIContext(GLFWwindow* window)
{
    m_gl_context = new GLContext(window);
    m_screen = new Screen();
    m_screen->setPos(glm::vec2(0));
}

void UIContext::addWidget(Widget* widget)
{
    m_screen->addWidget(widget);
}

void UIContext::loadFont(const char* font)
{
    m_gl_context->loadFont(font);
}

GLContext::GLContext(GLFWwindow* window)
    : fontPx(0.0)
{
    m_ta.font.bind = 0;

    glCreateBuffers(1, &m_ssb.text.buf);
    glNamedBufferStorage(m_ssb.text.buf, (1 << 16) / 16 * sizeof(Character), nullptr, GL_DYNAMIC_STORAGE_BIT);
    m_ssb.text.bind = 1;

    int screenSize[2];
    glfwGetWindowSize(window, &screenSize[0], &screenSize[1]);
    glCreateBuffers(1, &m_ub.screen_size.buf);
    glNamedBufferStorage(m_ub.screen_size.buf, sizeof(int) * 2, screenSize, GL_DYNAMIC_STORAGE_BIT);
    m_ub.screen_size.bind = 2;

    float widgetPos[2] = { 0.f, 0.f };
    glCreateBuffers(1, &m_ub.widget_pos.buf);
    glNamedBufferStorage(m_ub.widget_pos.buf, sizeof(float) * 2, widgetPos, GL_DYNAMIC_STORAGE_BIT);
    m_ub.widget_pos.bind = 3;

    createShader(&m_shaders.text,
                 VERSION_HEADER + BUFFERS + TEXTVERT,
                 VERSION_HEADER + TEXTFRAG);
}

void GLContext::bindBuffers()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssb.text.bind, m_ssb.text.buf);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_ub.screen_size.bind, m_ub.screen_size.buf);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_ub.widget_pos.bind, m_ub.widget_pos.buf);
    glBindTextureUnit(m_ta.font.bind, m_ta.font.buf);
}

void GLContext::drawFromRenderData(const RenderData& data)
{
    /*
    m_shaders.text.use();
    glNamedBufferSubData(m_ssb.text.buf, 0, sizeof(Character) * data.text.size(), data.text.data()); 
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.text.size());

    m_shaders.quad.use()
    glNamedBufferSubData(m_ssb.quad.buf, 0, sizeof(Character) * data.quad.size(), data.quad.data()); 
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.quad.size());
    */
}

void GLContext::setWidgetPos(glm::vec2 pos)
{
    glNamedBufferSubData(m_ub.widget_pos.buf, 0, sizeof(float) * 2, glm::value_ptr(pos) );
}

void GLContext::setScreenSize(glm::ivec2 size)
{
    glNamedBufferSubData(m_ub.screen_size.buf, 0, sizeof(int) * 2, glm::value_ptr(size) );
}

void GLContext::drawText(const char* text, glm::vec2 pos, float scale, glm::vec4 col, bool center)
{
    size_t numchars = strlen(text);
    Character buf[128];

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
        buf[idx] = {
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

    m_shaders.text.use();
    bindBuffers();
    glNamedBufferSubData(m_ssb.text.buf, 0, sizeof(Character) * numchars, buf);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, numchars);
}

#include <ft2build.h>
#include FT_FREETYPE_H
void GLContext::loadFont(const char* font)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        assert(false);
    }

    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
    {
        printf("ERROR::FREETYPE: Failed to load font\n");
        assert(false);
    }
    
    FT_Set_Pixel_Sizes(face, 0, 48);

    fontPx = 48;
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_ta.font.buf);
    glTextureStorage3D(m_ta.font.buf, 1, GL_R8, fontPx, fontPx, 128);

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

        glTextureSubImage3D(m_ta.font.buf, 0, 0, 0, currIdx, fontPx, fontPx, 1, GL_RED, GL_UNSIGNED_BYTE, empty);
        glTextureSubImage3D(m_ta.font.buf, 0, 0, 0, currIdx, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        currIdx++;
    }

    glTextureParameteri(m_ta.font.buf, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTextureParameteri(m_ta.font.buf, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ta.font.buf, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_ta.font.buf, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] empty;
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    m_shaders.text.use();
    glUniform1f(m_shaders.text.getLocation("fontPx"), fontPx);
}
