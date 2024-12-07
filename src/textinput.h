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
    void onCursorPosEvent(int x, int y);
    void onMouseDownEvent(int button, int action);
    void onKeyEvent(int key, int scancode, int action, int mods);
    void onCharEvent(unsigned int codepoint);

    std::string str() { return m_text_buf; };
protected:
    std::string m_text_buf;
    std::vector<float> m_pos_buf;

    std::string m_placeholder;

    float m_width;
    float m_offsetx;

    int m_text_cur;
    Math::ivec2 m_cursor_pos;
    Math::ivec2 m_last_cursor_pos;

    //x1, x2
    Math::ivec2 m_sel;

    Math::fvec4 m_padding;
private:
    inline void eraseSelection();
};

NAMESPACE_END(AereGui);
