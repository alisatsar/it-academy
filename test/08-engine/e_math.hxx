#pragma once

#include "color.hxx"

#include <iostream>

#ifndef OM_DECLSPEC
#define OM_DECLSPEC
#endif

namespace om
{

struct OM_DECLSPEC vec2
{
    vec2();
    vec2(float x, float y);
    float x = 0;
    float y = 0;
};

OM_DECLSPEC vec2 operator+(const vec2& l, const vec2& r);

struct OM_DECLSPEC mat2x3
{
    mat2x3();
    static mat2x3 identiry();
    static mat2x3 scale(float scale);
    static mat2x3 scale(float sx, float sy);
    static mat2x3 rotation(float thetha);
    static mat2x3 move(const vec2& delta);
    vec2          col0;
    vec2          col1;
    vec2          delta;
};

OM_DECLSPEC vec2 operator*(const vec2& v, const mat2x3& m);
OM_DECLSPEC mat2x3 operator*(const mat2x3& m1, const mat2x3& m2);

/// vertex with position only
struct OM_DECLSPEC v0
{
    vec2 pos;
};

/// vertex with position and texture coordinate
struct OM_DECLSPEC v1
{
    vec2  pos;
    color c;
};

/// vertex position + color + texture coordinate
struct OM_DECLSPEC v2
{
    vec2  pos;
    vec2  uv;
    color c;
};

/// triangle with positions only
struct OM_DECLSPEC tri0
{
    tri0();
    v0 v[3];
};

/// triangle with positions and color
struct OM_DECLSPEC tri1
{
    tri1();
    v1 v[3];
};

/// triangle with positions color and texture coordinate
struct OM_DECLSPEC tri2
{
    tri2();
    v2 v[3];
};

OM_DECLSPEC std::istream& operator>>(std::istream& is, mat2x3&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, vec2&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, v0&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, v1&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, v2&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, tri0&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, tri1&);
OM_DECLSPEC std::istream& operator>>(std::istream& is, tri2&);

}
