#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class GridList : public Widget
{
    friend class Widget;
public:
    GridList();
    virtual void draw(GLContext* ctx) override;

protected:

};

NAMESPACE_END(AereGui);
