#include "window.h"

using namespace AereGui;

Window::Window(const char* title)
    : Widget(), m_title(title), m_bar_height(20)
{
    m_text_scale = 0.4;
}

void Window::onMouseEnterEvent(bool enter)
{
    m_hovered = enter ? true : false;
    m_pressed = enter ? m_pressed : false;
}

void Window::onMouseDownEvent(int button, int action)
{
    if (m_hovered)
    {
        switch (action)
        {
            case (GLFW_PRESS):
                m_pressed = true;
                m_active = true;
                break;
            case (GLFW_RELEASE):
                m_pressed = false;
                m_active = true;
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

void Window::draw(GLContext* ctx)
{
    Widget::draw(ctx);

    ctx->draw9Slice(m_pos, m_texentry,
        m_active ? STATE_ACTIVE : STATE_NONE,
        m_center, m_size);

    ctx->drawText(m_title.c_str(), m_pos + glm::vec2(0, m_size.y - m_bar_height), m_text_scale, m_text_color, m_center);

}
