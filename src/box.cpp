#include "box.h"

using namespace AereGui;
using namespace Math;

Box::Box()
    : Widget()
{}

void Box::draw(GLContext* ctx)
{
    ctx->drawTexture(m_box, m_texentry, m_state, m_render_flags);
    Widget::draw(ctx);
}
