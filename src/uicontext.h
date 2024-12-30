#pragma once

#include "common.h"
#include "screen.h"
#include <GLFW/glfw3.h>

NAMESPACE_BEGIN(TexGui);

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

    Widget* addWidget(Widget* widget);

    void loadFont(const char* font);
    Screen* screenPtr() { return m_screen; };
    void preloadTextures(const char* dir);
private:
    Screen* m_screen;
    GLContext* m_gl_context;
};

NAMESPACE_END(TexGui);
