#include "defaults.h"
#include "listitem.h"

using namespace AereGui;
using namespace Math;

extern std::unordered_map<std::string, TexEntry> m_tex_map;
ListItem::ListItem(const char* image, const char* label, bool* binding)
    : Widget(), m_label(label), m_binding(binding)
{
    assignTexture(Defaults::ListItem::Texture);
    m_padding = Defaults::ListItem::Padding;
    if (!m_tex_map.contains(image))
    {
        printf("invalid assign texture in list item");
        exit(1);
    }

    TexEntry* e = &m_tex_map[image];
    m_image_texentry = e;

    setSize(fvec2(e->bounds.w * m_pixel_size + m_padding.left + m_padding.right, e->bounds.h * m_pixel_size + m_padding.top + m_padding.bottom));

    *m_binding = false;
}

void ListItem::onMouseDownEvent(int button, int action)
{
    Widget::onMouseDownEvent(button, action);
    if (m_state & STATE_ACTIVE)
        *m_binding = true;
    else
        *m_binding = false;
}

void ListItem::draw(GLContext* ctx)
{
    int state = m_state & STATE_ACTIVE ? STATE_ACTIVE : m_state;
    ctx->drawTexture(fbox::pad(m_box, m_padding), m_image_texentry, state, m_pixel_size, 0);
    ctx->drawTexture(m_box, m_texentry, state, m_pixel_size, SLICE_9);
}
