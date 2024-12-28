#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Row : public Widget
{
    friend class Widget;
public:
    Widget* addCol(Widget* widget, float size = 0);

    template <typename... Cols>
    Row(Cols... columns)
        : Widget()
    {
        (addCol(nullptr, columns), ...);
    }
    virtual void draw(GLContext* ctx) override;

protected:
    std::vector<double> m_widths;
    std::vector<double> m_inherit_widths;
    float m_height;
    int m_wrapped_height = 0;
    
    Math::fvec2 m_col_pos;
    bool m_wrap;

private:
    float m_absolute_width = 0;
    float m_inherit_cols = 0;
};

NAMESPACE_END(AereGui);
