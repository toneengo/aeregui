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
    void onMouseEnterEvent(bool enter);
    void onMouseDownEvent(int button, int action);
protected:
    std::string m_title;
    float m_bar_height;
};

NAMESPACE_END(AereGui);
