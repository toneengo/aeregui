#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Window : public Widget
{
    friend class Widget;
public:
    Window(const char* title);
    virtual void draw(GLContext* ctx);
    void onCursorPosEvent(int x, int y);
    void onMouseDownEvent(int button, int action);
protected:
    std::string m_title;
    bool m_resizing;
    bool m_moving;
    Math::fvec2 m_cursor_offset;
    float m_bar_height;
};

NAMESPACE_END(AereGui);
