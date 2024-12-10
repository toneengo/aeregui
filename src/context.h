#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "shader.h"
#include "types.h"
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN(AereGui);

enum AereGui_flags : int
{
    CENTER_X = 0b00000001,
    CENTER_Y = 0b00000010,
    SLICE_9  = 0b00000100,
    SLICE_3  = 0b00001000,
    BLUR     = 0b00010000,
};

enum AereGui_state : uint8_t 
{
    STATE_NONE   = 0b00000001,
    STATE_ACTIVE = 0b00000010,
    STATE_HOVER  = 0b00000100,
    STATE_PRESS  = 0b00001000,
};

class Widget;
class Screen;
class GLContext;

// state/texture layer enum
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
    void setWidgetPos(Math::fvec2 pos);
    Math::fvec2 getWidgetPos() { return m_widget_pos; };

    void setScreenSize(int width, int height);
    Math::ivec2 getScreenSize() { return m_screen_size; };

    void loadFont(const char* font);
    void preloadTextures(const char* dir);

    int drawText(const char* text, Math::fvec2 pos, const Math::fvec4& col, float scale, int flags);
    void drawTexture(const Math::fbox& rect, TexEntry* e, int state, int flags);
    void drawQuad(const Math::fbox& rect, const Math::fvec4& col);
    Math::fvec2 m_widget_pos;
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
        nameIdx colquad;
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
        Shader colquad;
        Shader quad9slice;
        Shader text;
    } m_shaders;

    float fontPx;
    Math::ivec2 m_screen_size;

    int m_pixel_size = 2;

    int m_font_height;

};

NAMESPACE_END(AereGui);
