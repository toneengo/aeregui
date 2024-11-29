#pragma once

#include "common.h"

NAMESPACE_BEGIN(AereGui);

struct alignas(16) Character
{
    glm::vec4 vector; //xpos, ypos, width, height
    glm::vec4 col;
    int layer;
    float scale;
};

struct alignas(16) Quad
{
    glm::vec4 vector; //xpos, ypos, width, height
    glm::vec4 texBounds; //xpos, ypos, width, height
    int layer;
};

// font information
struct CharInfo
{
    int layer;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct TexEntry
{
    int layer;

    bool hover;
    bool press;
    bool active;

    bool tl;
    bool tc;
    bool tr;
    bool l;
    bool c;
    bool r;
    bool bl;
    bool bc;
    bool br;

    int x;
    int y;
    int width;
    int height;

    unsigned char* data;
    unsigned char* _hover;
    unsigned char* _press;
    unsigned char* _active;
};

struct RenderData {
    std::vector<Quad> quad;
    std::vector<Character> text;
};

int loadFont(const char* font);
void addText(std::vector<Character>& vec, const char * text);

NAMESPACE_END(AereGui);
