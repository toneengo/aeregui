#include "button.h"

using namespace AereGui;

Button::Button(const char* label, _AEREGUI_FN_PTR fn)
    : Widget(), m_label(label), m_pressed_offset(0, -2), m_function(fn)
{
    m_text_scale = 0.4;
}

void Button::onMouseEnterEvent(bool enter)
{
    m_hovered = enter ? true : false;
    m_pressed = enter ? m_pressed : false;
}

void Button::onMouseDownEvent(int button, int action)
{
    if (m_hovered)
    {
        switch (action)
        {
            case (GLFW_PRESS):
                m_pressed = true;
                break;
            case (GLFW_RELEASE):
                m_pressed = false;
                m_active = true;
                m_function();
                break;
            default:
                break;
        }
    }
    else
    {
        m_active = false;
    }
}

void Button::draw(GLContext* ctx)
{
    Widget::draw(ctx);

    ctx->drawTexture(m_box, m_texentry,
        m_pressed ? STATE_PRESS :
        m_hovered ? STATE_HOVER :
        m_active ? STATE_ACTIVE : STATE_NONE);

    ctx->drawText(m_label.c_str(), m_pressed ? m_box.pos + m_pressed_offset : m_box.pos, m_text_color, m_text_scale, true);

}
