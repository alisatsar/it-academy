#pragma once

#include "collision_box.hxx"
#include "e_math.hxx"

///screen dimension compile-runtime
constexpr size_t window_width{640};
constexpr size_t window_height{480};
constexpr float tex_px_x = 1.0f / window_width;
constexpr float tex_px_y = 1.0f / window_height;
constexpr float pos_x0 = window_width / 2.0f;
constexpr float pos_y0 = window_height / 2.0f;
constexpr float pos_px_x = 1.0f / (window_width / 2.0f);
constexpr float pos_px_y = 1.0f / (window_height / 2.0f);

om::vec2 get_tex_coor(float px_x, float px_y)
{
	om::vec2 result;
	result.x = tex_px_x * px_x;
	result.y = tex_px_y * px_y;
	return result;
}

om::vec2 get_pos_coor(float px_x, float px_y)
{
	om::vec2 result;

	result.x = (px_x - pos_x0) * pos_px_x;

	if(px_x == pos_x0)
	{
		result.x = 0.0f;
	}

	result.y = (px_y - pos_y0) * pos_px_y;

	if(px_y == pos_y0)
	{
		result.y = 0.0f;
	}

	return result;
}

class controller
{
private:
	om::vec2 position;
	collision_box col_box;
public:
	controller() = default;
	controller(om::vec2 pos, collision_box col);
	controller(om::vec2 pos);
	void set_position(om::vec2 pos);
	void set_position_x(float x);
	void set_position_y(float y);
	om::vec2 get_position() const;
	void set_collision_box(om::vec2 v0, om::vec2 v1);
	collision_box get_collision_box() const;
	void move_col_box_x(float x);
	void move_col_box_y(float y);
};

controller::controller(om::vec2 pos, collision_box col) :
		position(pos), col_box(col)
{
}

controller::controller(om::vec2 pos)
{
	position = pos;
}

void controller::set_position(om::vec2 pos)
{
	position = pos;
}

void controller::set_position_x(float x)
{
	position.x = x;
}

void controller::set_position_y(float y)
{
	position.y = y;
}

om::vec2 controller::get_position() const
{
	return position;
}

void controller::set_collision_box(om::vec2 v0, om::vec2 v1)
{
	col_box.v0 = v0;
	col_box.v1 = v1;
}

collision_box controller::get_collision_box() const
{
	return col_box;
}

void controller::move_col_box_x(float x)
{
	col_box.move_x(x);
}

void controller::move_col_box_y(float y)
{
	col_box.move_y(y);
}
