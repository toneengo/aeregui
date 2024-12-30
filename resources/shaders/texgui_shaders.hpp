#pragma once

#include <string>

namespace TexGui
{

inline const std::string VERSION_HEADER = R"#(
#version 460 core
)#";

inline const std::string BUFFERS = R"#(
struct Character {
    vec4 rect;
    vec4 col;
    int layer;
    float scale;
};

struct Quad {
    vec4 rect;
    ivec4 texBounds;
    int layer;
    int pixelSize;
};

struct ColQuad {
    vec4 rect;
    vec4 col;
    int padding;
};

layout (std140, binding = 0) uniform screenSzBuf {
    ivec2 screenSz;
};

layout (std140, binding = 1) uniform indexBuf {
    int index;
};
)#";

inline const std::string TEXTVERT = R"#(
layout (std430, binding = 0) buffer objectBuf {
    Character text[];
};

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

    vec4 rect = text[index].rect;
    rect.x -= screenSz.x / 2;
    rect.y = screenSz.y / 2 - rect.y - rect.w;

    float scale = text[index].scale;

    uv = quad[gl_VertexID];

    rect.zw /= scale;
    uv = vec2(0.0, rect.w / fontPx) +
         uv * (vec2(rect.z, -rect.w)) / fontPx;

    layer = text[index].layer;
    colour = text[index].col;

    vec2 size = rect.zw / round(vec2(screenSz.x/2.0, screenSz.y/2.0)) * scale;
    vec2 pos = quad[gl_VertexID] * size + rect.xy / round(vec2(screenSz.x/2.0, screenSz.y/2.0));
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

inline const std::string COLQUADVERT = R"#(
layout (std430, binding = 0) buffer objectBuf {
    ColQuad colquads[];
};
out vec4 col;

void main() {
    ivec2 quad[6] = ivec2[6](
        ivec2(0, 0),
        ivec2(1, 0),
        ivec2(0, 1),
        ivec2(1, 1),
        ivec2(0, 1),
        ivec2(1, 0)
    ); 

    vec4 rect = colquads[index].rect;
    rect.x -= screenSz.x / 2;
    rect.y = screenSz.y / 2 - rect.y - rect.w;

    col = colquads[index].col;

    vec2 size = rect.zw / vec2(screenSz.x/2.0, screenSz.y/2.0);
    vec2 pos = quad[gl_VertexID] * size + rect.xy / vec2(screenSz.x/2.0, screenSz.y/2.0);
    gl_Position = vec4(pos, 0.0, 1.0);
}
)#";

inline const std::string QUADVERT = R"#(
layout (std430, binding = 0) buffer objectBuf {
    Quad quads[];
};

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
    vec4 rect = quads[index].rect;
    rect.x -= screenSz.x / 2;
    rect.y = screenSz.y / 2 - rect.y - rect.w;

    vec4 texBounds = quads[index].texBounds;

    uv = quad[gl_VertexID];

    uv = vec2(texBounds.x, texBounds.y) / ATLAS_SIZE +
         uv * (vec2(texBounds.z, -texBounds.w)) / ATLAS_SIZE;

    layer = quads[index].layer;

    vec2 size = rect.zw / vec2(screenSz.x/2.0, screenSz.y/2.0);
    vec2 pos = quad[gl_VertexID] * size + rect.xy / vec2(screenSz.x/2.0, screenSz.y/2.0);
    gl_Position = vec4(pos, 0.0, 1.0);
}
)#";

inline const std::string QUAD9SLICEVERT = R"#(
layout (std430, binding = 0) buffer objectBuf {
    Quad quads[];
};

out vec2 uv;
flat out int layer;

struct Slices
{
    float top;
    float right;
    float bottom;
    float left;
};

uniform vec4 slices;

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
    int px = quads[index].pixelSize;
    vec4 rect = quads[index].rect;
    rect.x -= screenSz.x / 2;
    rect.y = screenSz.y / 2 - rect.y - rect.w;

    vec4 texBounds = quads[index].texBounds;

    float top = slices.x;
    float right = slices.y;
    float bottom = slices.z;
    float left = slices.w;

    if (gl_InstanceID % 3 == 0)
    {
        rect.z = left * px;
        texBounds.z = left;
    }
    else if (gl_InstanceID % 3 == 1)
    {
        rect.x += left * px;
        rect.z -= (left + right) * px;
        texBounds.x += left;
        texBounds.z -= left + right;
    }
    else if (gl_InstanceID % 3 == 2)
    {
        rect.x += rect.z - right * px;
        rect.z = right * px;
        texBounds.x += texBounds.z - right; 
        texBounds.z = right;
    }

    if (gl_InstanceID % 9 / 3 == 0)
    {
        rect.w = bottom * px;
        texBounds.w = bottom;
    }
    else if (gl_InstanceID % 9 / 3 == 1)
    {
        rect.y += bottom * px;
        rect.w -= (top + bottom) * px;
        texBounds.y -= bottom;
        texBounds.w -= top + bottom;
    }
    else if (gl_InstanceID % 9 / 3 == 2)
    {
        rect.y += rect.w - top * px;
        rect.w = top * px;
        texBounds.y -= texBounds.w - top;
        texBounds.w = top;
    }

    uv = quad[gl_VertexID];

    uv = vec2(texBounds.x, texBounds.y) / ATLAS_SIZE +
         uv * (vec2(texBounds.z, -texBounds.w)) / ATLAS_SIZE;

    layer = quads[index].layer;

    vec2 size = rect.zw / round(vec2(screenSz.x/2.0, screenSz.y/2.0));
    vec2 pos = quad[gl_VertexID] * size + rect.xy / round(vec2(screenSz.x/2.0, screenSz.y/2.0));
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

inline const std::string BASICFRAG = R"#(
in vec4 col;
out vec4 frag;

void main() {
    frag = col;
    if (frag.a < 0.1)
        discard;
}
)#";

inline const std::string BLUR1 = R"#(
//kawase downscale

out vec4 frag;

in vec2 uv;

uniform vec2 halfpixel;
uniform float radius;

uniform sampler2D image;

void main() {
    vec2 uv2 = uv * 2.0;
    frag = texture(image, uv2) * 4.0;
    frag += texture(image, uv2 - halfpixel.xy * radius);
    frag += texture(image, uv2 + halfpixel.xy * radius);
    frag += texture(image, uv2 + vec2(halfpixel.x, -halfpixel.y) * radius);
    frag += texture(image, uv2 - vec2(halfpixel.x, -halfpixel.y) * radius);

    frag = frag / 8.0;
}
)#";

inline const std::string BLUR2 = R"#(
//kawase upscale

out vec4 frag;

in vec2 uv;

uniform vec2 halfpixel;
uniform float radius;

uniform sampler2D image;

void main() {
    vec2 uv2 = uv / 2.0;
    frag = texture(image, uv2 + vec2(-halfpixel.x * 2.0, 0.0) * radius);
    frag += texture(image, uv2 + vec2(-halfpixel.x, halfpixel.y) * radius) * 2.0;
    frag += texture(image, uv2 + vec2(0.0, halfpixel.y * 2.0) * radius);
    frag += texture(image, uv2 + vec2(halfpixel.x, halfpixel.y) * radius) * 2.0;
    frag += texture(image, uv2 + vec2(halfpixel.x * 2.0, 0.0) * radius);
    frag += texture(image, uv2 + vec2(halfpixel.x, -halfpixel.y) * radius) * 2.0;
    frag += texture(image, uv2 + vec2(0.0, -halfpixel.y * 2.0) * radius);
    frag += texture(image, uv2 + vec2(-halfpixel.x, -halfpixel.y) * radius) * 2.0;

    frag = frag / 12.0;
}
)#";

}
