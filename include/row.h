#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Row : public Widget
{
    friend class Widget;
public:
    Row(float height);
    void addCol(Widget* widget, float size);
    virtual void draw(GLContext* ctx) override;

protected:
    float m_height;
    int m_wrapped_height;
    int m_cols;
    int m_padding;
    
    Math::fvec2 m_col_pos;
    bool m_wrap;
};

NAMESPACE_END(AereGui);
