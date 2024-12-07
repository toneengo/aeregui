#include "widget.h"
#include "util.h"

using namespace AereGui;
using namespace Math;
Widget::Widget()
    : m_visible(true), m_box(0, 0, 1, 1), m_text_color(1, 1, 1, 1),
      m_text_scale(1.0),
      m_state(STATE_NONE)
{
}

void Widget::onCursorPosEvent(int x, int y)
{
    bool enter = contains(fvec2(x, y));
    setFlagBit(m_state, STATE_HOVER, enter ? 1 : 0);
    /*
    if (!enter)
        setFlagBit(m_state, STATE_PRESS, 0);
    */
}

void Widget::onMouseDownEvent(int button, int action)
{
    if (action == GLFW_PRESS)
    {
        if (m_state & STATE_HOVER)
        {
            setFlagBit(m_state, STATE_PRESS, 1);
            setFlagBit(m_state, STATE_ACTIVE, 1);
            return;
        }

        setFlagBit(m_state, STATE_ACTIVE, 0);
    }

    if (action == GLFW_RELEASE) {
        setFlagBit(m_state, STATE_PRESS, 0);
    }
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

bool Widget::contains(const Math::fvec2& pos)
{
    Math::fvec2 d = pos - m_box.pos;
    return d.x <= m_box.w && d.x >= 0.f &&
           d.y <= m_box.h && d.y >= 0.f;
}

extern std::unordered_map<std::string, TexEntry> m_tex_map;
void Widget::assignTexture(std::string tex)
{
    if (!m_tex_map.contains(tex))
    {
        printf("invalid assign texture");
        exit(1);
    }

    TexEntry* e = &m_tex_map[tex];
    m_texentry = e;

    m_box.w = e->bounds.w * 2;
    m_box.h = e->bounds.h * 2;
}

void Widget::addChild(Widget* widget)
{
    m_children.push_back(widget);
    widget->m_parent = this;
}

Widget* Widget::findWidget(const fvec2& pos)
{
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
    {
        Widget* child = *it;
        if (child->visible() && child->contains(pos + m_box.pos))
            return child->findWidget(pos + m_box.pos);
    }
    return contains(pos) && m_visible ? this : nullptr;
}
