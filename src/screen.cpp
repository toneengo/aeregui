#include "screen.h"

using namespace AereGui;
using namespace Math;

Screen::Screen(GLContext* gl_ctx)
    : Widget(), m_gl_context(gl_ctx)
{
    m_box.pos = {0.0, 0.0};
    m_box.size = gl_ctx->getScreenSize();
}

void Screen::cursorPosCallback(double x, double y)
{
    Widget* w = findWidget(ivec2(x, y));

    if (w != m_hovered_widget && m_hovered_widget)
        m_hovered_widget->onMouseEnterEvent(false);

    if (w != nullptr)
        w->onMouseEnterEvent(true);

    m_hovered_widget = w;
}

void Screen::mouseButtonCallback(int button, int action)
{
    //Deactivates current active widget if it is not hovered
    if (m_hovered_widget != m_active_widget && m_active_widget)
        m_active_widget->onMouseDownEvent(button, action);

    m_active_widget = m_hovered_widget;
    if (m_hovered_widget)
        m_hovered_widget->onMouseDownEvent(button, action);
}

void Screen::keyCallback(int key, int scancode, int action, int mods)
{
    if (m_active_widget)
        m_active_widget->onKeyEvent(key, scancode, action, mods);
}

void Screen::charCallback(unsigned int codepoint)
{
    if (m_active_widget)
    {
        m_active_widget->onCharEvent(codepoint);
    }
}

void Screen::framebufferSizeCallback(int width, int height)
{
    m_gl_context->setScreenSize(width, height);
    m_box.size = m_gl_context->getScreenSize();
}
