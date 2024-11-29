#pragma once

#include "context.h"
#include "common.h"
#include "widget.h"
#include "shader.h"

NAMESPACE_BEGIN(AereGui);

class Screen : public Widget
{
    friend class Widget;
    friend class GLContext;
public:
    Screen(GLContext* gl_ctx);
    void cursorPosCallback(double x, double y);
    void mouseButtonCallback(int button, int action);
    void keyCallback(int key, int scancode, int action, int mods);
    void charCallback(unsigned int codepoint);
    void framebufferSizeCallback(int width, int height);
private:
    GLContext* m_gl_context;
    Widget* m_hovered_widget = nullptr;
    Widget* m_active_widget = nullptr;
};

NAMESPACE_END(AereGui);
