
#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);
class Button : public Widget
{
public:
    Button();
    virtual void draw(GLContext* ctx);
};
NAMESPACE_END(AereGui);
