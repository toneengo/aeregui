#include "glad/gl.h"
#include "context.h"
#include "widget.h"
#include "screen.h"

#include <filesystem>

#include <glm/gtc/type_ptr.hpp>
#include "shaders/aeregui_shaders.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

using namespace AereGui;

constexpr int ATLAS_SIZE = 512;

void UIContext::render()
{
    m_screen->draw(m_gl_context);
}

UIContext::UIContext(GLFWwindow* window)
{
    m_gl_context = new GLContext(window);
    m_screen = new Screen(m_gl_context);
    m_screen->setPos(glm::vec2(0));
}

void UIContext::addWidget(Widget* widget)
{
    m_screen->addChild(widget);
}

void UIContext::loadFont(const char* font)
{
    m_gl_context->loadFont(font);
}

void UIContext::preloadTextures(const char* dir)
{
    m_gl_context->preloadTextures(dir);
}

UIContext::~UIContext()
{
    delete m_gl_context;
}

GLContext::GLContext(GLFWwindow* window)
    : fontPx(0.0)
{
    glEnable(GL_MULTISAMPLE);
    m_ta.font.bind = 0;
    m_ta.texture.bind = 1;

    glCreateBuffers(1, &m_ssb.text.buf);
    glNamedBufferStorage(m_ssb.text.buf, (1 << 16) / 16 * sizeof(Character), nullptr, GL_DYNAMIC_STORAGE_BIT);
    m_ssb.text.bind = 0;

    glCreateBuffers(1, &m_ssb.quad.buf);
    glNamedBufferStorage(m_ssb.quad.buf, (1 << 16) / 16 * sizeof(Quad), nullptr, GL_DYNAMIC_STORAGE_BIT);
    m_ssb.quad.bind = 1;

    glfwGetWindowSize(window, &m_screen_size.x, &m_screen_size.y);
    glCreateBuffers(1, &m_ub.screen_size.buf);
    glNamedBufferStorage(m_ub.screen_size.buf, sizeof(int) * 2, glm::value_ptr(m_screen_size), GL_DYNAMIC_STORAGE_BIT);
    m_ub.screen_size.bind = 0;

    float widgetPos[2] = { 0.f, 0.f };
    glCreateBuffers(1, &m_ub.widget_pos.buf);
    glNamedBufferStorage(m_ub.widget_pos.buf, sizeof(float) * 2, widgetPos, GL_DYNAMIC_STORAGE_BIT);
    m_ub.widget_pos.bind = 1;

    createShader(&m_shaders.text,
                 VERSION_HEADER + BUFFERS + TEXTVERT,
                 VERSION_HEADER + TEXTFRAG);

    createShader(&m_shaders.quad,
                 VERSION_HEADER + BUFFERS + QUADVERT,
                 VERSION_HEADER + QUADFRAG);
}

GLContext::~GLContext()
{

}

void GLContext::bindBuffers()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssb.text.bind, m_ssb.text.buf);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssb.quad.bind, m_ssb.quad.buf);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_ub.screen_size.bind, m_ub.screen_size.buf);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_ub.widget_pos.bind, m_ub.widget_pos.buf);
    glBindTextureUnit(m_ta.font.bind, m_ta.font.buf);
    glBindTextureUnit(m_ta.texture.bind, m_ta.texture.buf);
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

void GLContext::setScreenSize(int width, int height)
{
    m_screen_size.x = width;
    m_screen_size.y = height;
    glViewport(0, 0, width, height);
    glNamedBufferSubData(m_ub.screen_size.buf, 0, sizeof(int) * 2, glm::value_ptr(m_screen_size));
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
            currx += (m_char_map[text[idx]].advance >> 6) * scale;
        }
        pos.x -= currx / 2.0;
        pos.y -= m_font_height / 2.0;
    }

    currx = pos.x;

    for (int idx = 0; idx < numchars; idx++)
    {
        const CharInfo& info = m_char_map[text[idx]];
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

void GLContext::drawTexture(glm::vec2 pos, glm::vec4 texBounds, int layer, bool center)
{
    if (center)
    {
        pos.x -= texBounds.z / 2.0 * m_pixel_size;
        pos.y -= texBounds.w / 2.0 * m_pixel_size;
    }

    Quad buf = {
        .vector = glm::vec4{pos, 0.f, 0.f},
        .texBounds = texBounds,
        .layer = layer,
    };

    m_shaders.quad.use();
    bindBuffers();
    glNamedBufferSubData(m_ssb.quad.buf, 0, sizeof(Quad), &buf);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
}
/*
void GLContext::drawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 col)
{

}
*/

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
    m_font_height = face->height >> 6;
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
            continue;
        }

        m_char_map[c] = {
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

struct TexData
{
    unsigned char* hover;
    unsigned char* press;
};

    
int tc(int x, int y)
{
    return x + (y * ATLAS_SIZE);
}

std::unordered_map<std::string, AereGui::TexEntry> m_tex_map;
void GLContext::preloadTextures(const char* dir)
{
    std::vector<std::filesystem::directory_entry> files;
    for (const auto& f : std::filesystem::recursive_directory_iterator(dir))
    {
        files.push_back(f);
    }


    for (auto& f : files)
    {
        if (!f.is_regular_file())
        {
            continue;
        }

        std::string pstr = f.path().string();
        std::string fstr = f.path().filename().string();
        fstr.erase(fstr.begin() + fstr.find('.'), fstr.end());

        if (!pstr.ends_with(".png"))
        {
            printf("Unexpected file in sprites folder: %s", pstr.c_str());
            continue;
        }

        int width, height, channels;
        unsigned char* im = stbi_load(pstr.c_str(), &width, &height, &channels, 4);
        if (im == nullptr)
        {
            std::cout << "Failed to load file: " << pstr << "\n";
            continue;
        }

        if (m_tex_map.contains(fstr))
        {
            if (m_tex_map[fstr].width != width || m_tex_map[fstr].height != height)
            {
                printf("Texture variant dimension mismatch: %s", pstr.c_str());
                continue;
            }
        }
        else
        {
            m_tex_map[fstr].width = width; 
            m_tex_map[fstr].height = height; 
        }

        if (pstr.ends_with(".hover.png"))
        {
            m_tex_map[fstr]._hover = im;
            m_tex_map[fstr].hover = true;
        }
        else if (pstr.ends_with(".press.png"))
        {
            m_tex_map[fstr]._press = im;
            m_tex_map[fstr].press = true;
        }
        else
        {
            m_tex_map[fstr].data = im;
        }

    }

    // Do the packing
    stbrp_node* nodes = new stbrp_node[4096];
    stbrp_context ctx;
    stbrp_init_target(&ctx, ATLAS_SIZE, ATLAS_SIZE, nodes, 4096);
    stbrp_rect* boxes = new stbrp_rect[m_tex_map.size()];

    int i = 0;
    for (auto& e : m_tex_map)
    {
        boxes[i] = {
            i, e.second.width, e.second.height
        };
        i++;
    }
    if (!stbrp_pack_rects(&ctx, boxes, m_tex_map.size()))
    {
        std::cout << "Failed to pack all textures.\n";
        exit(1);
    }

    // Write all the textures into a big ol atlas
    unsigned char* atlas = new unsigned char[ATLAS_SIZE * ATLAS_SIZE * 4];
    unsigned char* hover = new unsigned char[ATLAS_SIZE * ATLAS_SIZE * 4];
    unsigned char* press = new unsigned char[ATLAS_SIZE * ATLAS_SIZE * 4];
    memset(atlas, 0, ATLAS_SIZE * ATLAS_SIZE * 4);
    memset(hover, 0, ATLAS_SIZE * ATLAS_SIZE * 4);
    memset(press, 0, ATLAS_SIZE * ATLAS_SIZE * 4);

    i = 0;
    for (auto& p : m_tex_map)
    {
        TexEntry& e = p.second;
        stbrp_rect& rect = boxes[i];
        // Copy the sprite data into the big chunga
        // Scanline copy
        if (e.data == nullptr)
        {
            printf("huhh %s \n", p.first.c_str());
            continue;
        }

        e.x = rect.x;
        e.y = rect.y;

        for (int row = 0; row < e.height; row++)
            memcpy(atlas     + 4 * tc(rect.x, rect.y + row), e.data     + (4 * e.width * row), e.width * 4);

        if (e._hover != nullptr)
            for (int row = 0; row < e.height; row++)
                memcpy(hover + 4 * tc(rect.x, rect.y + row), e._hover + (4 * e.width * row), e.width * 4);

        if (e._press != nullptr)
            for (int row = 0; row < e.height; row++)
                memcpy(press + 4 * tc(rect.x, rect.y + row), e._press + (4 * e.width * row), e.width * 4);

    }

    if(!stbi_write_png("resources/atlas.png", ATLAS_SIZE, ATLAS_SIZE, 4, atlas, 4 * ATLAS_SIZE))
    {
        std::cout << "Failed to write out atlas image\n";
        exit(1);
    }

    if(!stbi_write_png("resources/hover.png", ATLAS_SIZE, ATLAS_SIZE, 4, hover, 4 * ATLAS_SIZE))
    {
        std::cout << "Failed to write out atlas image\n";
        exit(1);
    }

    if(!stbi_write_png("resources/press.png", ATLAS_SIZE, ATLAS_SIZE, 4, press, 4 * ATLAS_SIZE))
    {
        std::cout << "Failed to write out atlas image\n";
        exit(1);
    }

    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_ta.texture.buf);
    glTextureStorage3D(m_ta.texture.buf, 1, GL_RGBA8, ATLAS_SIZE, ATLAS_SIZE, 128);

    glTextureSubImage3D(m_ta.texture.buf, 0, 0, 0, 0, ATLAS_SIZE, ATLAS_SIZE, 1, GL_RGBA, GL_UNSIGNED_BYTE, atlas);
    glTextureSubImage3D(m_ta.texture.buf, 0, 0, 0, 1, ATLAS_SIZE, ATLAS_SIZE, 1, GL_RGBA, GL_UNSIGNED_BYTE, hover);
    glTextureSubImage3D(m_ta.texture.buf, 0, 0, 0, 2, ATLAS_SIZE, ATLAS_SIZE, 1, GL_RGBA, GL_UNSIGNED_BYTE, press);

    delete [] atlas;
    delete [] hover;
    delete [] press;

    glTextureParameteri(m_ta.texture.buf, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTextureParameteri(m_ta.texture.buf, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ta.texture.buf, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_ta.texture.buf, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for (auto& e : m_tex_map)
    {
        stbi_image_free(e.second.data);
        stbi_image_free(e.second._hover);
        stbi_image_free(e.second._press);
    }
}
