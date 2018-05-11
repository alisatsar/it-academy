#pragma once

#include <cmath>

#include "color.hxx"

struct vec2
{
	float x;
	float y;
	vec2() : x(0.f), y(0.f){}
	vec2(float x_, float y_) : x(x_), y(y_){}

};

vec2 operator+(const vec2& left, const vec2& right)
{
	vec2 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	return result;
}

struct v0
{
	vec2 pos;
	color c;
};

struct v1
{
    vec2  pos;
    color c;
};

struct v2
{
    vec2  pos;
    vec2  uv;
    color c;
};

struct tri0
{
    tri0();
    v0 v[3];
};

struct tri1
{
    tri1();
    v1 v[3];
};

struct tri2
{
    tri2();
    v2 v[3];
};

tri0::tri0()
    : v{ v0(), v0(), v0() }
{
}

tri1::tri1()
    : v{ v1(), v1(), v1() }
{
}

tri2::tri2()
    : v{ v2(), v2(), v2() }
{
}

struct vertex
{
	vec2 position;
	vec2 uniform_;
	color color_;
};

struct matrix
{
	vec2 row0;
	vec2 row1;
	vec2 row2;
	matrix()
	: row0(1.f, 0.f),
	  row1(0.f, 1.f),
	  row2(0.f, 0.f)
	{
	}

	static matrix identity()
	{
		return matrix::scale(1.f);
	}

	static matrix scale(float scale)
	{
		matrix result;
		result.row0.x = scale;
		result.row1.y = scale;
		return result;
	}

	static matrix scale(float sx, float sy)
	{
		matrix r;
		r.row0.x = sx;
		r.row1.y = sy;
		return r;
	}

	static matrix rotation(float thetha)
	{
		matrix result;

		result.row0.x = std::cos(thetha);
		result.row0.y = std::sin(thetha);

		result.row1.x = -std::sin(thetha);
		result.row1.y = std::cos(thetha);

		return result;
	}

	static matrix move(const vec2& delta)
	{
		matrix r = matrix::identity();
		r.row2   = delta;
		return r;
	}
};

vec2 operator*(const vec2& v, const matrix& m)
{
    vec2 result;
    result.x = v.x * m.row0.x + v.y * m.row0.y + m.row2.x;
    result.y = v.x * m.row1.x + v.y * m.row1.y + m.row2.y;
    return result;
}

matrix operator*(const matrix& m1, const matrix& m2)
{
    matrix r;

    r.row0.x = m1.row0.x * m2.row0.x + m1.row1.x * m2.row0.y;
    r.row1.x = m1.row0.x * m2.row1.x + m1.row1.x * m2.row1.y;
    r.row0.y = m1.row0.y * m2.row0.x + m1.row1.y * m2.row0.y;
    r.row1.y = m1.row0.y * m2.row1.x + m1.row1.y * m2.row1.y;

    r.row2.x = m1.row2.x * m2.row0.x + m1.row2.y * m2.row0.y + m2.row2.x;
    r.row2.y = m1.row2.x * m2.row1.x + m1.row2.y * m2.row1.y + m2.row2.y;

    return r;
}
