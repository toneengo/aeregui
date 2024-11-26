#pragma once

#include <vector>
#include "font.h"
#include "context.h"
#include "glm/glm.hpp"

NAMESPACE_BEGIN(AereGui);

class Widget
{
public:
    Widget();
    virtual void draw(GLContext* ctx);
    void setPos(glm::vec2 pos);
    virtual void addWidget(Widget * widget);

protected:
    Widget* m_parent;
    std::vector<Widget*> m_children;
    RenderData m_render_data;

    bool m_visible;
    glm::vec2 m_pos;

    float m_font_scale;
};

NAMESPACE_END(AereGui);
