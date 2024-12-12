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

    inline void setPos(Math::fvec2 pos) { m_inner_box.translate(pos - m_box.pos); m_box.pos = pos; };
    inline void setSize(Math::fvec2 size) { m_inner_box.size = m_inner_box.size + (size - m_box.size); m_box.size = size; };

    inline Math::fvec2 getPos() { return m_box.pos; };
    inline Math::fvec2 getSize() { return m_box.size; };

    void setPadding(float px) { m_inner_box = Math::fbox::expand(m_box, -px); };

    virtual void addChild(Widget * widget);

    Widget* findWidget(Math::ivec2& pos);
    bool contains(const Math::fvec2& pos);
    bool visible() { return m_visible; };

    void assignTexture(std::string tex);

    void setActiveChild(Widget* widget) { if (m_parent) m_parent->setActiveChild(widget);
                                          m_active_child = widget; };

    virtual void onFrameResizeEvent(int button, int action) {};
    virtual void onMouseDownEvent(int button, int action);
    virtual void onCursorPosEvent(int x, int y);
    virtual void onCursorEnterEvent();
    virtual void onCursorExitEvent();
    virtual void onKeyEvent(int key, int scancode, int action, int mods) {};
    virtual void onCharEvent(unsigned int codepoint) {};

protected:
    Widget* m_parent = nullptr;
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

    Widget* m_active_child;

    bool m_inherit_pos;
    bool m_inherit_size;
};

NAMESPACE_END();
