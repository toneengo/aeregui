#include "button.h"

using namespace AereGui;

Button::Button(const char* label, _AEREGUI_FN_PTR fn)
    : Widget(), m_label(label)
{
    m_center = true;
    m_function = fn;
}

void Button::onMouseEnterEvent(bool enter)
{
    m_hovered = enter ? true : false;
    m_pressed = enter ? m_pressed : false;
}

void Button::onMouseDownEvent(int button, int action)
{
    m_pressed = action == GLFW_PRESS ? true : false;
    if (action == GLFW_RELEASE && m_hovered)
    {
        m_function();
    }
}

void Button::draw(GLContext* ctx)
{
    Widget::draw(ctx);
    int layer = 0;
    if (m_hovered) layer = 1;
    if (m_pressed) layer = 2;

    ctx->drawTexture(m_pos, m_texture_bounds, layer, true);
    ctx->drawText(m_label.c_str(), m_pos, m_text_scale, m_text_color, m_center);

}
