#include "column.h"

using namespace AereGui;

Widget* Column::addRow(Widget* widget, float size)
{
    if (size == 0 && widget && widget->m_inherit.height == 0)
        size = widget->m_box.height;

    m_children.push_back(widget);
    m_heights.push_back(size);
    if (size < 1) 
    {
        m_inherit_heights.push_back(size == 0 ? 1 : size);
        m_inherit_rows++;
    }
    else
    {
        m_inherit_heights.push_back(0);
        m_absolute_height += size;
    }

    m_needs_update = true;
    return widget;
}

bool Column::contains(const Math::fvec2& pos)
{
    Math::fvec2 d = pos - m_box.pos;
    return d.x <= m_width && d.x >= 0.f &&
           d.y <= m_box.h && d.y >= 0.f;
}

void Column::clear()
{
    Widget::clear();
    m_heights.clear();
    m_inherit_heights.clear();
    m_absolute_height = 0;
    m_inherit_rows = 0;
}

void Column::draw(GLContext* ctx)
{
    if (m_parent && m_parent->m_needs_update)
    {
        float currWidth = 0;
        float currHeight = 0;
        for (int i = 0; i < m_children.size(); i++)
        {
            if (m_inherit_heights[i] > 0)
                m_heights[i] = (m_box.height - m_absolute_height - m_spacing * (m_children.size() - 1)) * m_inherit_heights[i] / m_inherit_rows;

            if (m_render_flags & WRAPPED && currWidth + m_heights[i] > m_box.width)
            {
                currWidth += m_box.width + m_spacing;
                currHeight = 0;
            }

            m_children[i]->setPos({m_box.x + currWidth, m_box.y + currHeight});
            m_children[i]->setSize({m_box.width, float(m_heights[i])});
            currHeight += m_heights[i] + m_spacing;
        }
        m_width = m_render_flags & WRAPPED ? currWidth + m_box.width : m_box.width;
        m_needs_update = false;
    }

    Widget::draw(ctx);
}
