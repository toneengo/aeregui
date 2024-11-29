#include "screen.h"

using namespace AereGui;

Screen::Screen(GLContext* gl_ctx)
    : Widget(), m_gl_context(gl_ctx)
{
    m_pos = {0.0, 0.0};
    m_size = gl_ctx->getScreenSize();
}

void Screen::cursorPosCallback(double x, double y)
{
    glm::vec2 pos = glm::vec2{x - m_size.x/2.0, -(y - m_size.y/2.0)};
    Widget* w = findWidget(pos);

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
    m_size = m_gl_context->getScreenSize();
}
