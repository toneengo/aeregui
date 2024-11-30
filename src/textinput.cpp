#include "textinput.h"
#include <chrono>

using namespace AereGui;

TextInput::TextInput(const char* placeholder)
    : Widget(), m_placeholder(placeholder)
{
    m_text_scale = 0.4;
}

void TextInput::onMouseEnterEvent(bool enter)
{
    m_hovered = enter ? true : false;
    m_pressed = enter ? m_pressed : false;
}

void TextInput::onMouseDownEvent(int button, int action)
{
    if (m_hovered)
    {
        switch (action)
        {
            case (GLFW_PRESS):
                m_pressed = true;
            case (GLFW_RELEASE):
                m_active = true;
            default:
                break;
        }
    }
    else
    {
        m_active = false;
    }
}

void TextInput::onKeyEvent(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_BACKSPACE && m_text_buffer.size() > 0 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        m_text_buffer.pop_back();
    }
}

void TextInput::onCharEvent(unsigned int codepoint)
{
    m_text_buffer.push_back(codepoint);
}

// ##### Timekeeping stuff ##### // 
using namespace std::chrono_literals;
namespace stc = std::chrono;

stc::nanoseconds flashTime = stc::milliseconds(1000) / 2;
stc::nanoseconds accumulator(0);

auto currentTime = stc::steady_clock::now();

bool showTextCursor = true;

void TextInput::draw(GLContext* ctx)
{
    // Calculate time elapsed since last loop
    auto startTime = stc::steady_clock::now();
    auto frameTime = startTime - currentTime;
    currentTime = startTime;
    accumulator += frameTime;

    if (accumulator >= flashTime)
    {
        showTextCursor = !showTextCursor;
        accumulator -= flashTime;
    }

    Widget::draw(ctx);

    ctx->drawTexture(m_pos, m_texentry,
        m_active ? STATE_ACTIVE : STATE_NONE,
        m_center);

    if (!m_active && m_text_buffer.size() == 0)
    {
        ctx->drawText(m_placeholder.c_str(), m_pos, m_text_scale, m_text_color, m_center);
        return;
    }

    int cursorPos = ctx->drawText(m_text_buffer.c_str(), m_pos, m_text_scale, m_text_color, m_center);
    if (showTextCursor && m_active)
        ctx->drawText("|", {cursorPos, m_pos.y}, m_text_scale, m_text_color, m_center);
}
