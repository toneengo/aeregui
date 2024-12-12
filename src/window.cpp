#include "window.h"
#include "util.h"

using namespace AereGui;
using namespace Math;

Window::Window(const char* title)
    : Widget(), m_title(title), m_moving(false), m_resizing(false)
{
    m_text_scale = 0.4;
}

void Window::onCursorPosEvent(int x, int y)
{
    Widget::onCursorPosEvent(x, y);
    if (m_moving)
    {
        setPos(fvec2(x - m_cursor_offset.x, y - m_cursor_offset.y));
        setFlagBit(m_state, STATE_HOVER, 1);
    }
    else if (m_resizing)
    {
        setSize(m_cursor_pos + m_cursor_offset);
        setFlagBit(m_state, STATE_HOVER, 1);
    }
}

void Window::onMouseDownEvent(int button, int action)
{
    Widget::onMouseDownEvent(button, action);

    //TODO: REMOVE * 2!!!!!
    if (!getFlagBit(m_state, STATE_ACTIVE)) return;
    if (action == GLFW_PRESS &&
        fbox(0, 0, m_box.width, m_texentry->top * 2).contains(m_cursor_pos))
    {
        m_cursor_offset = m_cursor_pos - m_box.pos;
        m_moving = true;
        return;
    }
    
    if (action == GLFW_PRESS &&
        fbox(m_box.width  - m_texentry->right * 2,
             m_box.height - m_texentry->bottom * 2,
             m_texentry->right * 2, m_texentry->bottom * 2).contains(m_cursor_pos))
    {
        m_cursor_offset = fvec2(m_box.width, m_box.height) - m_cursor_pos;
        m_resizing = true;
        return;
    }

    m_moving = false;
    m_resizing = false;
}

void Window::draw(GLContext* ctx)
{
    m_inner_box = fbox::expand(m_box, -8);
    ctx->drawTexture(m_box, m_texentry, m_state, SLICE_9);

    ctx->drawText(m_title.c_str(), {m_inner_box.x, m_box.y + m_texentry->top * 2/ 2}, m_text_color, m_text_scale, CENTER_Y);

    Widget::draw(ctx);
}
