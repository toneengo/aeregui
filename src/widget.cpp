#include "widget.h"

using namespace AereGui;
Widget::Widget()
    : m_visible(true), m_pos({0, 0}) 
{
}

void Widget::draw(GLContext* ctx)
{
    if (!m_visible) return;
    //ctx->setWidgetPos(m_pos);
    //ctx->drawFromRenderData(m_render_data);
    for (auto& w : m_children)
    {
        if (!m_visible) continue;
        w->draw(ctx);
    }
}

void Widget::setPos(glm::vec2 pos)
{
    m_pos = pos;
}

void Widget::addWidget(Widget * widget)
{
    m_children.push_back(widget);
    widget->m_parent = this;
}
