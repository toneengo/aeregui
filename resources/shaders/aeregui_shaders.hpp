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

struct Quad {
    vec4 vector;
    vec4 texBounds;
    int layer;
};

layout (std430, binding = 0) buffer textBuf {
    Character text[];
};

layout (std430, binding = 1) buffer quadBuf {
    Quad quads[];
};

layout (std140, binding = 0) uniform screenSzBuf {
    ivec2 screenSz;
};

layout (std140, binding = 1) uniform widgetPosBuf {
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

    vec4 rect = text[gl_InstanceID].vector;
    float scale = text[gl_InstanceID].scale;

    uv = quad[gl_VertexID];

    rect.zw /= scale;
    uv = vec2(0.0, rect.w / fontPx) +
         uv * (vec2(rect.z, -rect.w)) / fontPx;

    layer = text[gl_InstanceID].layer;
    colour = text[gl_InstanceID].col;

    vec2 size = rect.zw / vec2(screenSz.x/2.0, screenSz.y/2.0) * scale;
    vec2 pos = quad[gl_VertexID] * size + rect.xy / vec2(screenSz.x/2.0, screenSz.y/2.0) + (widgetPos);
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

inline const std::string QUADVERT = R"#(
out vec2 uv;
flat out int layer;

void main() {
    ivec2 quad[6] = ivec2[6](
        ivec2(0, 0),
        ivec2(1, 0),
        ivec2(0, 1),
        ivec2(1, 1),
        ivec2(0, 1),
        ivec2(1, 0)
    ); 

    int ATLAS_SIZE = 512;
    int pixelSz = 4;
    vec4 rect = quads[gl_InstanceID].vector;
    vec4 texBounds = quads[gl_InstanceID].texBounds;

    uv = quad[gl_VertexID];

    uv = vec2(texBounds.x, texBounds.y + texBounds.w) / ATLAS_SIZE +
         uv * (vec2(texBounds.z, -texBounds.w)) / ATLAS_SIZE;

    layer = quads[gl_InstanceID].layer;

    vec2 size = (texBounds.zw * pixelSz) / vec2(screenSz.x/2.0, screenSz.y/2.0);
    vec2 pos = quad[gl_VertexID] * size + rect.xy / vec2(screenSz.x/2.0, screenSz.y/2.0) + (widgetPos);
    gl_Position = vec4(pos, 0.0, 1.0);
}
)#";

inline const std::string QUADFRAG = R"#(
in vec2 uv;
flat in int layer;
out vec4 frag;

layout (binding = 1) uniform sampler2DArray texarray;

void main() {
    frag = texture(texarray, vec3(uv, layer));
    if (frag.a < 0.1)
        discard;
}
)#";

}
