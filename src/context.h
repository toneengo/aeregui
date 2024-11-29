#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "shader.h"
#include "font.h"
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN(AereGui);

class Widget;
class Screen;
class GLContext;

class UIContext
{
public:
    UIContext(GLFWwindow* window);
    ~UIContext();

    // Renders all GUI elements in 'data'.
    void render();

    // Clears render data.
    void clear();

    void addWidget(Widget* widget);

    void loadFont(const char* font);
    Screen* screenPtr() { return m_screen; };
    void preloadTextures(const char* dir);
private:
    Screen* m_screen;
    GLContext* m_gl_context;
};

class GLContext
{
public:
    GLContext(GLFWwindow* window);
    ~GLContext();
    void drawFromRenderData(const RenderData& data);
    void bindBuffers();
    void setWidgetPos(glm::vec2 pos);

    void setScreenSize(int width, int height);
    glm::ivec2 getScreenSize() { return m_screen_size; };

    void loadFont(const char* font);
    void preloadTextures(const char* dir);

    int drawText(const char* text, glm::vec2 pos, float scale, glm::vec4 col, bool center);
    void drawTexture(glm::vec2 pos, glm::vec4 texBounds, int layer, bool center);
    void drawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 col);
private:
    // Buffer name/Buffer binding pair
    struct nameIdx
    {
        uint32_t buf;
        uint32_t bind;
    };

    // Uniform buffers
    struct
    {
        nameIdx screen_size;
        nameIdx widget_pos;
    } m_ub;

    // Shader storage buffers
    struct
    {
        nameIdx text;
        nameIdx quad;
    } m_ssb;

    // Texture arrays
    struct
    {
        nameIdx texture;
        nameIdx font;
    } m_ta;

    struct
    {
        Shader quad;
        Shader text;
    } m_shaders;

    float fontPx;
    glm::ivec2 m_screen_size;

    int m_pixel_size = 2;

    int m_font_height;

    std::unordered_map<char, AereGui::CharInfo> m_char_map;
};

NAMESPACE_END(AereGui);
