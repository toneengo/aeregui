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

    void setPos(glm::vec2 pos) { m_pos = pos; };
    void setSize(glm::vec2 size) { m_size = size; };

    virtual void addChild(Widget * widget);

    Widget* findWidget(glm::vec2 pos);
    bool contains(glm::vec2 pos);
    bool visible() { return m_visible; };

    void assignTexture(std::string tex);

    virtual void onMouseDownEvent(int button, int action) {};
    virtual void onMouseEnterEvent(bool enter) {};
    virtual void onKeyEvent(int key, int scancode, int action, int mods) {};
    virtual void onCharEvent(unsigned int codepoint) {};

protected:
    Widget* m_parent;
    std::vector<Widget*> m_children;
    RenderData m_render_data;

    bool m_visible;

    bool m_hovered;
    bool m_pressed;
    bool m_active;
    
    int m_texture_tl;
    int m_texture_tc;
    int m_texture_tr;

    int m_texture_l;
    int m_texture_c;
    int m_texture_r;

    int m_texture_bl;
    int m_texture_bc;
    int m_texture_br;

    bool m_has_hover_texture;
    bool m_has_press_texture;
    glm::vec4 m_texture_bounds;

    glm::vec2 m_pos;
    glm::vec2 m_size;

    glm::vec2 m_text_offset;

    glm::vec4 m_text_color;
    float m_text_scale;
    bool m_center;
};

NAMESPACE_END();
