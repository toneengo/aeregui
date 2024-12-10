#include "button.h"
#include "util.h"

using namespace AereGui;

Button::Button(const char* label, _AEREGUI_FN_PTR fn)
    : Widget(), m_label(label), m_pressed_offset(0, 2), m_function(fn)
{
    m_text_scale = 0.4;
}

/*
void Button::onMouseEnterEvent(bool enter)
{
}
*/

void Button::onCursorPosEvent(int x, int y)
{
    Widget::onCursorPosEvent(x, y);
    // I don't want it to appear pressed when the mouse is dragged outside
    //if (!getFlagBit(m_state, STATE_HOVER)) setFlagBit(m_state, STATE_PRESS, 0);
}

void Button::onMouseDownEvent(int button, int action)
{
    Widget::onMouseDownEvent(button, action);
    if (m_state & STATE_HOVER && action == GLFW_RELEASE)
        m_function();
}

void Button::draw(GLContext* ctx)
{
    Widget::draw(ctx);

    ctx->drawTexture(m_box, m_texentry, m_state, SLICE_9);

    ctx->drawText(m_label.c_str(),
        m_state & STATE_PRESS ? m_box.pos + m_box.size / 2 + m_pressed_offset
                              : m_box.pos + m_box.size / 2,

        m_text_color, m_text_scale,
        CENTER_X | CENTER_Y);
}
