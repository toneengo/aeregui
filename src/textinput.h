#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class TextInput : public Widget
{
    friend class Widget;
public:
    TextInput(const char* placeholder);
    virtual void draw(GLContext* ctx);
    void onMouseEnterEvent(bool enter);
    void onMouseDownEvent(int button, int action);
    void onKeyEvent(int key, int scancode, int action, int mods);
    void onCharEvent(unsigned int codepoint);

    std::string str() { return m_text_buffer; };
protected:
    std::string m_text_buffer;
    std::string m_placeholder;

    glm::vec2 m_text_offset;
};

NAMESPACE_END(AereGui);
