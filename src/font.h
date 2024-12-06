#pragma once

#include "common.h"

NAMESPACE_BEGIN(AereGui);

NAMESPACE_BEGIN(Math);

//Barebones version of glm vec2, vec3 and vec4.
template <typename T>
struct vec2
{
    union { T x, r; };
    union { T y, g; };

    vec2()
        : x(0),
          y(0)
    {};

    vec2(const vec2<T>& _xy)
        : x(static_cast<T>(_xy.x)),
          y(static_cast<T>(_xy.y))
    {};

    template <typename v>
    vec2(const v& _xy)
        : x(static_cast<T>(_xy.x)),
          y(static_cast<T>(_xy.y))
    {};

    template <typename v>
    vec2(v&& _xy)
        : x(static_cast<T>(_xy.x)),
          y(static_cast<T>(_xy.y))
    {};

    vec2(T _x, T _y)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_y))
    {};

    template <typename v>
    vec2<T> operator-(const v& vec) const
    {
        vec2<T> val(x - vec.x, y - vec.y);
        return val;
    }

    //#TODO: test if othe way works?
    template <typename v>
    vec2<T> operator+(const v& vec) const
    {
        vec2<T> val(x + vec.x, y + vec.y);
        return val;
    }

    template <typename v>
    void operator-(const v& vec)
    {
        x -= vec.x;
        y -= vec.y;
    }

};

template <typename T>
struct vec3
{
    union
    {
        struct 
        {
            union { T x, r; };
            union
            {
                struct
                {
                    union { T y, g; };
                    union { T z, b;};
                };
                union { vec2<T> yz, gb; };
            };
        };
        union { vec2<T> xy, rg; };
    };

    vec3(T _x, T _y, T _z)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_y)),
          z(static_cast<T>(_z))
    {};

    //#TODO: other initialisers. likely dont need them though
};

template <typename T>
struct vec4
{
    union {
        struct {
            union { T x, r; };
            union {
                struct {
                    union { T y, g; };
                    union {
                        struct {
                            union { T z, b; };
                            union { T w, a; };
                        };
                        union { vec2<T> zw, ba; };
                    };
                };
                union { vec3<T> yzw, gba; };
                union { vec2<T> yz, gb; };
            };
        };
        union { vec3<T> xyz, rgb; };
        union { vec2<T> xy, rg; };
    };

    vec4() {}
    
    vec4(T _x, T _y, T _z, T _w)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_y)),
          z(static_cast<T>(_z)),
          w(static_cast<T>(_w))
    {}

    vec4(const vec4<T>& _v)
        : x(static_cast<T>(_v.x)),
          y(static_cast<T>(_v.y)),
          z(static_cast<T>(_v.z)),
          w(static_cast<T>(_v.w))
    {}

    vec4(vec2<T> _xy, T _z, T _w)
        : x(static_cast<T>(_xy.x)),
          y(static_cast<T>(_xy.y)),
          z(static_cast<T>(_z)),
          w(static_cast<T>(_w))
    {}

    vec4(vec2<T> _xy, vec2<T> _zw)
        : x(static_cast<T>(_xy.x)),
          y(static_cast<T>(_xy.y)),
          z(static_cast<T>(_zw.x)),
          w(static_cast<T>(_zw.y))
    {}

    vec4(T _x, T _y, vec2<T> _zw)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_y)),
          z(static_cast<T>(_zw.x)),
          w(static_cast<T>(_zw.y))
    {}

    vec4(T _x, vec2<T> _yz, T _w)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_yz.x)),
          z(static_cast<T>(_yz.y)),
          w(static_cast<T>(_w))
    {}

    //#TODO: other initialisers. likely dont need them though. need to finish vec3 first
};

typedef vec4<float> fvec4;
typedef vec3<float> fvec3;
typedef vec2<float> fvec2;
typedef vec4<int> ivec4;
typedef vec3<int> ivec3;
typedef vec2<int> ivec2;
typedef vec2<unsigned int> uivec2;

// Template box type with xpos, ypos, width, and height, supporting vec2 containers.
template <typename T>
struct box
{
    union
    {
        struct
        {
            T x;
            T y;
        };
        union { vec2<T> pos, xy; };
    };
    union
    {
        struct
        {
            union { T w; T width; };
            union { T h; T height; };
        };
        union { vec2<T> size, wh; };
    };

    box() {}

    box(const box<T>& _box)
        : x(_box.x),
          y(_box.y),
          w(_box.w),
          h(_box.h)
    {}

    box(T _x, T _y, T _w, T _h)
        : x(static_cast<T>(_x)),
          y(static_cast<T>(_y)),
          w(static_cast<T>(_w)),
          h(static_cast<T>(_h))
    {}

    template <typename v>
    box(const v& _box)
        : x(static_cast<T>(_box.x)),
          y(static_cast<T>(_box.y)),
          w(static_cast<T>(_box.w)),
          h(static_cast<T>(_box.h))
    {}

    template <typename v>
    box(v&& _box)
        : x(static_cast<T>(_box.x)),
          y(static_cast<T>(_box.y)),
          w(static_cast<T>(_box.w)),
          h(static_cast<T>(_box.h))
    {}

};

typedef box<float> fbox;
typedef box<int> ibox;

NAMESPACE_END(Math);

// Renderable objects
struct alignas(16) Character
{
    Math::fbox rect; //xpos, ypos, width, height
    Math::fvec4 col;
    int layer;
    float scale;
};

struct alignas(16) Quad
{
    Math::fbox rect; //xpos, ypos, width, height
    Math::ibox texBounds; //xpos, ypos, width, height
    int layer;
};

// font information
struct CharInfo
{
    int layer;
    Math::uivec2 size;
    Math::ivec2 bearing;
    unsigned int advance;
};

struct TexEntry
{
    int layer;

    bool hover;
    bool press;
    bool active;

    float top;
    float right;
    float bottom;
    float left;

    Math::ibox bounds;

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
