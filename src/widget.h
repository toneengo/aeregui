#pragma once

#include <vector>
#include "types.h"
#include "context.h"
#include "stdio.h"

NAMESPACE_BEGIN(AereGui);

class Widget
{
public:
    Widget();
    virtual void draw(GLContext* ctx);

    void setPos(Math::fvec2 pos) { m_inner_box.translate(pos - m_box.pos); m_box.pos = pos; };
    void setSize(Math::fvec2 size) { m_inner_box.size = m_inner_box.size + (size - m_box.size); m_box.size = size; };
    void setPadding(float px) { m_inner_box = Math::fbox::expand(m_box, -px); };

    virtual void addChild(Widget * widget);

    Widget* findWidget(const Math::fvec2& pos);
    bool contains(const Math::fvec2& pos);
    bool visible() { return m_visible; };

    void assignTexture(std::string tex);

    virtual void onFrameResizeEvent(int button, int action) {};
    virtual void onMouseDownEvent(int button, int action);
    virtual void onCursorPosEvent(int x, int y);
    virtual void onKeyEvent(int key, int scancode, int action, int mods) {};
    virtual void onCharEvent(unsigned int codepoint) {};

protected:
    Widget* m_parent;
    std::vector<Widget*> m_children;
    RenderData m_render_data;

    bool m_visible;

    unsigned int m_state;

    TexEntry* m_texentry;

    Math::fbox m_box;
    Math::fbox m_inner_box;
    
    Math::fvec4 m_padding;

    Math::fvec4 m_text_color;

    Math::ivec2 m_cursor_pos;

    float m_text_scale;

    bool m_inherit_pos;
    bool m_inherit_size;
};

NAMESPACE_END();
