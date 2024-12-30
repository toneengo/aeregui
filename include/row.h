#pragma once

#include "defaults.h"
#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(AereGui);

class Row : public Widget
{
    friend class Widget;
public:
    Widget* addCol(Widget* widget, float size = 0);
    bool contains(const Math::fvec2& pos) override;

    template <typename... Cols>
    Row(Cols... columns)
        : Widget(), m_spacing(Defaults::Row::Spacing)
    {
        (addCol(nullptr, columns), ...);
    }
    virtual void draw(GLContext* ctx) override;

protected:
    std::vector<double> m_widths;
    std::vector<double> m_inherit_widths;
    float m_height;
    float m_row_height;
    float m_last_height;
    float m_spacing;
    
    Math::fvec2 m_col_pos;
    bool m_wrap;

private:
    float m_absolute_width = 0;
    float m_inherit_cols = 0;
};

NAMESPACE_END(AereGui);
