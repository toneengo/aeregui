#include "defaults.h"
#include "box.h"

using namespace AereGui;
using namespace Math;

Box::Box(int x, int y, int width, int height)
    : Widget(x, y, width, height)
{
    m_padding = Defaults::Box::Padding;
}

void Box::draw(GLContext* ctx)
{
    ctx->drawTexture(m_box, m_texentry, m_state, m_pixel_size, m_render_flags);
    Widget::draw(ctx);
}
