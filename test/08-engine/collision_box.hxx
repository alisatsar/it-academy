#pragma once

#include "e_math.hxx"

///x0 = right; y0 = bottom; x1 = left; y1 = top;
class collision_box
{
public:
	om::vec2 v0 {0.0f, 0.0f};
	om::vec2 v1 {0.0f, 0.0f};

	collision_box() = default;
	collision_box(om::vec2 v0_, om::vec2 v1_);
};

collision_box::collision_box(om::vec2 v0_, om::vec2 v1_)
{
	v0 = v0_;
	v1 = v1_;
}

