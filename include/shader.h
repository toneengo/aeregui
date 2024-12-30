#pragma once

#include "common.h"
#include <string>

NAMESPACE_BEGIN(TexGui);
struct uniformInfo
{ 
    char * key;

    struct Value
    {
	    int32_t location;
	    uint32_t count;
    };
    Value value;
};

struct Shader
{
    uint32_t id;
    uniformInfo * uniforms;

    void use();
    uint32_t getLocation(const char * uniform_name);

    uint32_t fontPx = -1;
    int slices;
};

void createShader(Shader* shader, const std::string& vertexShader, const std::string& fragmentShader);
NAMESPACE_END(TexGui);
