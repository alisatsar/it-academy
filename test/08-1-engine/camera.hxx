#pragma once

#include "e_math.hxx"

///v0.x = left, v0.y = bottom
class camera
{
private:
	om::vec2 v0;
	om::mat2x3 move;
public:
	camera();
	om::vec2 get_pos() const;
	void update_camera(float x, float y);
	om::mat2x3 get_camera_matrix() const;
};

camera::camera()
{
	v0.x = 0.0f;
	v0.y = 0.0f;
	move = om::mat2x3::move(v0);
}

om::vec2 camera::get_pos() const
{
	return v0;
}

void camera::update_camera(float x, float y)
{
	v0.x += x;
	v0.y += y;
	move = om::mat2x3::move(v0);
}

om::mat2x3 camera::get_camera_matrix() const
{
	return move;
}
