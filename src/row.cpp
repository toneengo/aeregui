#include "row.h"

using namespace AereGui;

Widget* Row::addCol(Widget* widget, float size)
{
    if (size == 0 && widget && widget->m_inherit.width == 0)
        size = widget->m_box.width;

    m_children.push_back(widget);
    m_widths.push_back(size);
    if (size < 1) 
    {
        m_inherit_widths.push_back(size == 0 ? 1 : size);
        m_inherit_cols++;
    }
    else
    {
        m_inherit_widths.push_back(0);
        m_absolute_width += size;
    }

    m_needs_update = true;
    return widget;
}

void Row::draw(GLContext* ctx)
{
    Widget::draw(ctx);
    if (m_parent && m_parent->m_needs_update)
    {
        float currWidth = 0;
        m_wrapped_height = 0;
        for (int i = 0; i < m_children.size(); i++)
        {
            if (m_inherit_widths[i] > 0)
                m_widths[i] = (m_box.width - m_absolute_width) * m_inherit_widths[i] / m_inherit_cols;

            if (m_render_flags & WRAPPED && currWidth + m_widths[i] > m_box.width)
            {
                currWidth = 0;
                m_wrapped_height += m_box.height;
            }
            m_children[i]->setPos({m_box.x + currWidth, m_box.y + m_wrapped_height});
            m_children[i]->setSize({float(m_widths[i]), m_box.height});
            currWidth += m_widths[i];
        }

        m_needs_update = false;
    }
}
