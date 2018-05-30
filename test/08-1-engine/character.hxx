#pragma once

#include "actor.hxx"
#include "engine.hxx"
#include "e_math.hxx"
#include "color.hxx"

class character : public actor
{
private:
	std::uint16_t count_sprite_width;
	std::uint16_t count_sprite_height;
	std::uint16_t size_sprite_px;
public:
	character(om::texture* ch_tex_, std::uint16_t count_sp_width,
			std::uint16_t count_sp_height, std::uint16_t px);
	~character();
	std::uint16_t animate(std::uint16_t first_frame,
			std::uint16_t count_frame, uint16_t last_frame,
			float delta_sec, float sec_now);
	std::uint16_t get_size_sprite_px() const;
	vbo* get_character_vbo() const;
	om::texture* get_character_texture() const;
};

character::character(om::texture* ch_tex_, std::uint16_t count_sp_width,
			std::uint16_t count_sp_height, std::uint16_t px) :
			count_sprite_width(count_sp_width),
			count_sprite_height(count_sp_height),
			size_sprite_px(px)
{
	om::tri2 tri_left;
	om::tri2 tri_right;

	om::color color(1.0f, 1.0f, 1.0f, 1.0f);

	float tex_step_x = 1.0f / count_sp_width;
	float tex_step_y = 1.0f / count_sp_height;

	tri_right.v[0].pos.x = -1.0f;
	tri_right.v[0].pos.y = -1.0f;
	tri_right.v[0].uv.x = 0.0f;
	tri_right.v[0].uv.y = 1.0f - tex_step_y;
	tri_right.v[0].c = color;

	tri_right.v[1].pos.x = 1.0f;
	tri_right.v[1].pos.y = -1.0f;
	tri_right.v[1].uv.x = tex_step_x;
	tri_right.v[1].uv.y = 1.0f - tex_step_y;
	tri_right.v[1].c = color;

	tri_right.v[2].pos.x = 1.0f;
	tri_right.v[2].pos.y = 1.0f;
	tri_right.v[2].uv.x = tex_step_x;
	tri_right.v[2].uv.y = 1.0f;
	tri_right.v[2].c = color;

	tri_left.v[0].pos.x = 1.0f;
	tri_left.v[0].pos.y = 1.0f;
	tri_left.v[0].uv.x = tex_step_x;
	tri_left.v[0].uv.y = 1.0f;
	tri_left.v[0].c = color;

	tri_left.v[1].pos.x = -1.0f;
	tri_left.v[1].pos.y = 1.0f;
	tri_left.v[1].uv.x = 0.0f;
	tri_left.v[1].uv.y = 1.0f;
	tri_left.v[1].c = color;

	tri_left.v[2].pos.x = -1.0f;
	tri_left.v[2].pos.y = -1.0f;
	tri_left.v[2].uv.x = 0.0f;
	tri_left.v[2].uv.y = 1.0f - tex_step_y;
	tri_left.v[2].c = color;

	set_actor_vbo(tri_right, tri_left);
	set_actor_texture(ch_tex_);
}

std::uint16_t character::animate(std::uint16_t first_frame,
			std::uint16_t count_frame, uint16_t last_frame,
			float delta_sec, float sec_now)
{
	uint16_t current_row = first_frame / count_sprite_width;

	static float cur_time = 0.0f;
	static float last_time = 0.0f;

	float tex_step_x = 1.0f / count_sprite_width;
	float tex_step_y = 1.0f / count_sprite_height;

	float left = first_frame * tex_step_x - current_row;
	float right = left + tex_step_x;

	float top = 1.f - tex_step_y * current_row;
	float bottom = top - tex_step_y;

	if((cur_time - last_time) >= delta_sec)
	{
		if(first_frame <= last_frame)
		{
			changet_texture_coord(left, right, top, bottom);
			++first_frame;
			last_time = cur_time;
		}
	}
	if(first_frame == last_frame + 1)
	{
		first_frame = last_frame - count_frame;
	}

	cur_time = sec_now;

	return first_frame;
}

std::uint16_t character::get_size_sprite_px() const
{
	return size_sprite_px;
}

vbo* character::get_character_vbo() const
{
	return get_actor_vbo();
}

om::texture* character::get_character_texture() const
{
	return get_actor_texture();
}

character::~character(){}
