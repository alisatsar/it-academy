#pragma once

#include "e_math.hxx"

///x0 = left; y0 = bottom; x1 = right; y1 = top;
class collision_box
{
public:
	om::vec2 position_state{0.0f, 0.0f};
	om::vec2 v0 {0.0f, 0.0f};
	om::vec2 v1 {0.0f, 0.0f};
	om::mat2x3 matrix;

	collision_box() = default;
	collision_box(om::vec2 v0_, om::vec2 v1_);
	collision_box(om::mat2x3 matrix);

	om::vec2 get_position_state() const { return position_state; }

	void set_x(float x);
	void set_y(float y);
	void move_x(float x);
	void move_y(float y);
};

collision_box::collision_box(om::vec2 v0_, om::vec2 v1_)
{
	position_state = v0_;
	v0 = v0_;
	v1 = v1_;
}

void collision_box::set_x(float x)
{
	v0.x = x;
	v1.x = x;
}

void collision_box::set_y(float y)
{
	float delta = v1.y - v0.y;
	v0.y = y;
	v1.y = y + delta;
}

void collision_box::move_x(float x)
{
	v0.x += x;
	v1.x += x;
}

void collision_box::move_y(float y)
{
	v0.y += y;
	v1.y += y;
}

collision_box::collision_box(om::mat2x3 matrix_) :
		matrix(matrix_)
{
}
