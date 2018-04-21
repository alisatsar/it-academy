#pragma once
#include <string>
#include <iosfwd>
#include <string_view>

///this means tiny_engine
namespace te
{
///this is events for tiny_engine
enum class event
{
	left_pressed,
	left_released,
	right_pressed,
	right_released,
	up_pressed,
	up_released,
	down_pressed,
	down_released,
	select_pressed,
	select_released,
	start_pressed,
	start_released,
	button_a_pressed,
	button_a_released,
	button_b_pressed,
	button_b_released,
	turn_off
};

struct vector
{
	float x;
	float y;
	vector() : x(0.f), y(0.f){};
};

struct matrix
{
	float m00 = 0.f;
	float m01 = 0.f;
	float m02 = 0.f;
	float m10 = 0.f;
	float m11 = 0.f;
	float m12 = 0.f;
	float m20 = 0.f;
	float m21 = 0.f;
	float m22 = 0.f;
	matrix()
	{
		m00 = 0.f;
		m01 = 0.f;
		m02 = 0.f;
		m10 = 0.f;
		m11 = 0.f;
		m12 = 0.f;
		m20 = 0.f;
		m21 = 0.f;
		m22 = 0.f;
	}

	matrix operator=(matrix const& r)
	{
		m00 = r.m00;
		m01 = r.m01;
		m02 = r.m02;
		m10 = r.m10;
		m11 = r.m11;
		m12 = r.m12;
		m20 = r.m20;
		m21 = r.m21;
		m22 = r.m22;

		return *this;
	}

	matrix operator*(float n)
	{
		m00 = m00 * n;
		m10 = m10 * n;

		return *this;
	}

	matrix operator+(vector const& v)
	{
		m00 = 1;
		m11 = 1;
		m02 = v.x;
		m22 = 1;
		m12 = v.y;

		return *this;
	}

	matrix operator*(matrix const& r)
	{
		matrix l = *this;
		l.m00 = m00 * r.m00 + m01 * r.m10 + m02 * r.m20;
		l.m01 = m00 * r.m01 + m01 * r.m11 + m02 * r.m21;
		l.m02 = m00 * r.m02 + m01 * r.m12 + m02 * r.m22;

		l.m10 = m10 * r.m00 + m11 * r.m10 + m12 * r.m20;
		l.m11 = m10 * r.m01 + m11 * r.m11 + m12 * r.m21;
		l.m12 = m10 * r.m02 + m11 * r.m12 + m12 * r.m22;

		l.m20 = m20 * r.m00 + m21 * r.m10 + m22 * r.m20;
		l.m21 = m20 * r.m01 + m21 * r.m11 + m22 * r.m21;
		l.m22 = m20 * r.m02 + m21 * r.m12 + m22 * r.m22;

		return l;
	}

	matrix(vector const& v)
	{
		m00 = v.x;
		m10 = v.y;
	}
};



struct vertex
{
	vector vec;

	float tx;
	float ty;
	vertex()
	{
		tx = 0.f;
		ty = 0.f;
		vec.x = 0.f;
		vec.y = 0.f;
	}

};

struct triangle
{
	vertex v[3];
	triangle()
	{
		v[0] = vertex();
		v[1] = vertex();
		v[2] = vertex();
	}

	triangle operator*(float n)
	{
		matrix m1(this->v[0].vec);
		m1 = m1 * n;

		matrix m2(this->v[1].vec);
		m2 = m2 * n;

		matrix m3(this->v[2].vec);
		m3 = m3 * n;

		triangle t(m1, m2, m3);

		return t;
	}

	triangle(matrix const& m1, matrix const& m2, matrix const& m3)
	{
		v[0].vec.x = m1.m00;
		v[0].vec.y = m1.m10;
		v[1].vec.x = m2.m00;
		v[1].vec.y = m2.m10;
		v[2].vec.x = m3.m00;
		v[2].vec.y = m3.m10;
	}
};

class engine
{
public:
	virtual ~engine();
	virtual std::string check_version() = 0;
	virtual bool initialize() = 0;
	virtual bool create_window(
			const char* title, int pos_x, int pos_y, int width, int height) = 0;
	virtual bool read_input(event& e) = 0;
	virtual float get_time_from_init() = 0;
	virtual bool load_texture(std::string_view path) = 0;
	virtual void clear_color() = 0;
	virtual void unintialize() = 0;
	virtual void swap_buffers() = 0;
	virtual void render_triangle(triangle& t) = 0;
	virtual void create_my_shader() = 0;
	//virtual void scale_triangle(triangle& t, float scale) = 0;
};

engine* create_engine();
void destroy_engine(engine* e);
}
