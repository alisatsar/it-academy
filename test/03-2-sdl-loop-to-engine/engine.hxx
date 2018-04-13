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

class engine
{
public:
	virtual ~engine();
	virtual std::string check_version() = 0;
	virtual bool initialize() = 0;
	virtual bool create_window(
			const char* title, int pos_x, int pos_y, int width, int height) = 0;
	virtual bool read_input(event& e) = 0;
	virtual void unintialize() = 0;
};

engine* create_engine();
void destroy_engine(engine* e);
}
