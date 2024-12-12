#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Box : public Widget
{
    friend class Widget;
public:
    Box();
    virtual void draw(GLContext* ctx);
protected:
};

NAMESPACE_END(AereGui);
