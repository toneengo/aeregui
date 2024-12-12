#include "listitem.h"

using namespace AereGui;
using namespace Math;

extern std::unordered_map<std::string, TexEntry> m_tex_map;
ListItem::ListItem(const char* image, const char* label)
    : Widget(), m_label(label)
{
    if (!m_tex_map.contains(image))
    {
        printf("invalid assign texture in list item");
        exit(1);
    }

    TexEntry* e = &m_tex_map[image];
    m_image_texentry = e;

    m_box.w = e->bounds.w * 2;
    m_box.h = e->bounds.h * 2;
}

void ListItem::onMouseDownEvent(int button, int action)
{
    Widget::onMouseDownEvent(button, action);
    if (m_parent && m_state & STATE_ACTIVE) m_parent->setActiveChild(this);
}

void ListItem::draw(GLContext* ctx)
{
    int state = m_state & STATE_ACTIVE ? STATE_ACTIVE : m_state;
    ctx->drawTexture(m_box, m_image_texentry, state, 0);
    ctx->drawTexture(m_box, m_texentry, state, SLICE_9);
}
