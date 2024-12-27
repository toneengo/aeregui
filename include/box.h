#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Box : public Widget
{
    friend class Widget;
public:
    Box(int x = 0, int y = 0, int width = INHERIT, int height = INHERIT);
    virtual void draw(GLContext* ctx);
protected:
};

NAMESPACE_END(AereGui);
