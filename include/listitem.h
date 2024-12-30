#pragma once

#include "common.h"
#include "widget.h"

NAMESPACE_BEGIN(TexGui);

//image
class ListItem : public Widget
{
    friend class Widget;
public:
    ListItem(const char* image, const char* label, bool* binding);
    virtual void draw(GLContext* ctx) override;
    virtual void onMouseDownEvent(int button, int action) override;
protected:
    TexEntry* m_image_texentry;
    std::string m_label;
    int m_data;
    Math::fvec2 m_col_pos;
    bool m_wrap;

    bool* m_binding;
};

NAMESPACE_END(TexGui);
