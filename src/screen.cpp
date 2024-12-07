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
    m_cursor_pos = ivec2(x, y);
    Widget* w = findWidget(m_cursor_pos);

    if (w != m_hovered_widget && m_hovered_widget)
        m_hovered_widget->onCursorPosEvent(m_cursor_pos.x, m_cursor_pos.y);

    if (w != nullptr)
        w->onCursorPosEvent(m_cursor_pos.x, m_cursor_pos.y);
    
    if (m_active_widget != nullptr)
        m_active_widget->onCursorPosEvent(m_cursor_pos.x, m_cursor_pos.y);

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

void Screen::draw(GLContext* ctx)
{
    glEnable(GL_SCISSOR_TEST);
    Widget::draw(ctx);
    glDisable(GL_SCISSOR_TEST);
}
