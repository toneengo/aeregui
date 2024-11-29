#pragma once

#include "common.h"
#include "widget.h"

using _AEREGUI_FN_PTR = void(*)();

NAMESPACE_BEGIN(AereGui);

class Button : public Widget
{
    friend class Widget;
public:
    Button(const char* label, _AEREGUI_FN_PTR fn);
    virtual void draw(GLContext* ctx);
    void onMouseEnterEvent(bool enter);
    void onMouseDownEvent(int button, int action);
protected:
    std::string m_label;
    _AEREGUI_FN_PTR m_function;

    glm::vec2 m_pressed_offset;
};

NAMESPACE_END(AereGui);
