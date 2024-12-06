#pragma once

#include <vector>
#include "font.h"
#include "context.h"

NAMESPACE_BEGIN(AereGui);

class Widget
{
public:
    Widget();
    virtual void draw(GLContext* ctx);

    void setPos(Math::fvec2 pos) { m_box.pos = pos; };
    void setSize(Math::fvec2 size) { m_box.size = size; };

    virtual void addChild(Widget * widget);

    Widget* findWidget(const Math::fvec2& pos);
    bool contains(const Math::fvec2& pos);
    bool visible() { return m_visible; };

    void assignTexture(std::string tex);

    virtual void onFrameResizeEvent(int button, int action) {};
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

    TexEntry* m_texentry;

    Math::fbox m_box;
    Math::fvec4 m_text_color;
    float m_text_scale;

    bool m_inherit_pos;
    bool m_inherit_size;
};

NAMESPACE_END();
