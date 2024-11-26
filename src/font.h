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
    int texID;
};

// font information
struct CharInfo
{
    int layer;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct RenderData {
    std::vector<Quad> quad;
    std::vector<Character> text;
};

int loadFont(const char* font);
void addText(std::vector<Character>& vec, const char * text);

NAMESPACE_END(AereGui);
