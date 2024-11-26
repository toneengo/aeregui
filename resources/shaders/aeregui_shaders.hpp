#pragma once

#include <string>

namespace AereGui
{

inline const std::string VERSION_HEADER = R"#(
#version 460 core
)#";

inline const std::string BUFFERS = R"#(
struct Character {
    vec4 vector;
    vec4 col;
    int layer;
    float scale;
};

layout (std430, binding = 1) buffer textBuf {
    Character text[];
};

layout (std140, binding = 2) uniform screenSzBuf {
    ivec2 screenSz;
};

layout (std140, binding = 3) uniform widgetPosBuf {
    vec2 widgetPos;
};
)#";

inline const std::string TEXTVERT = R"#(
out vec2 uv;
flat out vec4 colour;
flat out int layer;

uniform float fontPx;

void main() {
    ivec2 quad[6] = ivec2[6](
        ivec2(0, 0),
        ivec2(1, 0),
        ivec2(0, 1),
        ivec2(1, 1),
        ivec2(0, 1),
        ivec2(1, 0)
    ); 

    vec4 texBounds = text[gl_InstanceID].vector;
    float scale = text[gl_InstanceID].scale;

    uv = quad[gl_VertexID];

    texBounds.zw /= scale;
    uv = vec2(0.0, texBounds.w / fontPx) +
         uv * (vec2(texBounds.z, -texBounds.w)) / fontPx;

    layer = text[gl_InstanceID].layer;
    colour = text[gl_InstanceID].col;

    vec2 size = texBounds.zw / vec2(screenSz.x/2.0, screenSz.y/2.0) * scale;
    vec2 pos = quad[gl_VertexID] * size + texBounds.xy / vec2(screenSz.x/2.0, screenSz.y/2.0) + (widgetPos);
    gl_Position = vec4(pos, 0.0, 1.0);
}
)#";

inline const std::string TEXTFRAG = R"#(
in vec2 uv;
flat in vec4 colour;
flat in int layer;
out vec4 frag;

layout (binding = 0) uniform sampler2DArray font;

void main() {
    frag = vec4(1.0, 1.0, 1.0, texture(font, vec3(uv, layer)).r);
    frag *= colour;

    if (frag.a < 0.1)
        discard;
}
)#";

}
