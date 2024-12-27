#include "screen.h"

using namespace AereGui;
using namespace Math;

Screen::Screen(GLContext* gl_ctx)
    : Widget(), m_gl_context(gl_ctx)
{
    m_box.pos = {0.0, 0.0};
    m_box.size = gl_ctx->m_screen_size;
    m_window_scale = gl_ctx->m_window_scale;
}

void Screen::cursorPosCallback(double x, double y)
{
    x *= m_window_scale;
    y *= m_window_scale;
    if (m_drag_active)
    {
        m_active_widget->onCursorPosEvent(x - m_active_widget_pos.x, y - m_active_widget_pos.y);
        return;
    }

    m_cursor_pos = ivec2(x, y);
    ivec2 temp_pos = m_cursor_pos;
    Widget* w = findWidget(temp_pos);
    m_active_widget_pos = m_cursor_pos - temp_pos;

    if (w != m_hovered_widget && m_hovered_widget)
    {
        m_hovered_widget->onCursorExitEvent();
        if (w) w->onCursorEnterEvent();
    }

    if (w)
        w->onCursorPosEvent(temp_pos.x, temp_pos.y);
    
    if (m_active_widget != nullptr && m_active_widget != m_hovered_widget)
        m_active_widget->onCursorPosEvent(temp_pos.x, temp_pos.y);

    m_hovered_widget = w;
}

void Screen::mouseButtonCallback(int button, int action)
{
    //Deactivates current active widget if it is not hovered
    if (m_active_widget)
        m_active_widget->onMouseDownEvent(button, action);

    if (action == GLFW_PRESS)
    {
        if (m_active_widget != m_hovered_widget && m_hovered_widget)
        {
            m_hovered_widget->onMouseDownEvent(button, action);
            m_active_widget = m_hovered_widget;
        }
        m_drag_active = true;
    }
    else
    {
        m_drag_active = false;
    }
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
    ibox ogSx;
    glGetIntegerv(GL_SCISSOR_BOX, (GLint*)&ogSx);
    Widget::draw(ctx);
    glDisable(GL_SCISSOR_TEST);
}
