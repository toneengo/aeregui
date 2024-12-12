#include "row.h"

using namespace AereGui;
Row::Row(float height)
    : m_cols(0), m_padding(0), m_wrap(1), m_height(height)
{
    m_box.height = height;
}

void Row::addCol(Widget* widget, float size)
{
    widget->setSize({size, m_height});
    if (m_wrap && m_parent && m_col_pos.x + size > m_parent->getSize().x)
    {
        m_col_pos.x = 0;
        m_col_pos.y += m_height;
        m_box.height += m_height;
    }
    
    widget->setPos({m_col_pos.x, m_col_pos.y});
    m_col_pos.x += size;

    m_box.width = std::max(m_col_pos.x, m_box.width);
    m_children.push_back(widget);
}

void Row::draw(GLContext* ctx)
{
    if (!m_visible) return;
    ctx->setWidgetPos(ctx->m_widget_pos + m_box.pos);
    //ctx->drawFromRenderData(m_render_data);
    for (auto& w : m_children)
    {
        if (!m_visible) continue;
        w->draw(ctx);
    }
    ctx->setWidgetPos(ctx->m_widget_pos - m_box.pos);

}
