#pragma once

#include "e_math.hxx"

///v0.x = left, v0.y = bottom, v1.x = right, v1.y = top
class camera
{
private:
	om::vec2 v0;
	om::vec2 v1;
public:
	camera(float width, float height);
	om::vec2 get_pos() const;
	void update_camera(float x, float y);
};

camera::camera(float width, float height)
{
	v0.x = 0.0f;
	v0.y = 0.0f;
	v1.x = width;
	v1.y = height;
}

om::vec2 camera::get_pos() const
{
	return v0;
}

void camera::update_camera(float x, float y)
{
	v0.x += x;
	v0.y += y;
	v1.x += x;
	v1.y += y;
}
