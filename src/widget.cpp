#include "widget.h"

const int PIXEL_SIZE = 4;
using namespace AereGui;
Widget::Widget()
    : m_visible(true), m_pos({0, 0}), m_text_color(glm::vec4(1.0)),
      m_text_scale(1.0), m_center(false),
      m_last_hovered(false), m_hovered(false), m_pressed(false)
{
}

void Widget::setPos(glm::vec2 pos)
{
    m_pos = pos;
    return;
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

bool Widget::contains(glm::vec2 pos)
{
    pos -= m_pos;
    if (m_center)
    {
        pos.x += m_size.x / 2;
        pos.y += m_size.y / 2;
    }
    return pos.x <= m_size.x && pos.x >= 0.f &&
           pos.y <= m_size.y && pos.y >= 0.f;
}

extern std::unordered_map<std::string, TexEntry> m_tex_map;
void Widget::assignTexture(std::string tex)
{
    if (!m_tex_map.contains(tex))
    {
        printf("invalid assign texture");
        exit(1);
    }
    TexEntry e = m_tex_map[tex];
    m_has_hover_texture = e.hover;
    m_has_press_texture = e.press;
    
    m_texture_bounds.x = e.x;
    m_texture_bounds.y = e.y;
    m_texture_bounds.z = e.width;
    m_texture_bounds.w = e.height;

    m_size.x = e.width * PIXEL_SIZE;
    m_size.y = e.height * PIXEL_SIZE;
}

void Widget::addChild(Widget* widget)
{
    m_children.push_back(widget);
    widget->m_parent = this;
}

Widget* Widget::findWidget(glm::vec2 pos)
{
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
    {
        Widget* child = *it;
        if (child->visible() && child->contains(pos + m_pos))
            return child->findWidget(pos + m_pos);
    }
    return contains(pos) && m_visible ? this : nullptr;
}
