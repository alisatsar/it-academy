#pragma once
#include <string>

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

struct vertex
{
	float x;
	float y;
	vertex() : x(0.f), y(0.f){}
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
	virtual void clear_color() = 0;
	virtual void unintialize() = 0;
	virtual void swap_buffers() = 0;
	virtual void render_triangle(triangle& t) = 0;
	virtual void create_my_shader() = 0;
};

engine* create_engine();
void destroy_engine(engine* e);
}
