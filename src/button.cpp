#include "button.h"

using namespace AereGui;

Button::Button()
    : Widget()
{
}

void Button::draw(GLContext* ctx)
{
    Widget::draw(ctx);

    ctx->drawText("Fuck it Btich", {0, 0}, 1, {1.0, 1.0, 0.0, 1.0}, 1);
}
