#pragma once

#include "actor.hxx"
#include "engine.hxx"
#include "e_math.hxx"
#include "color.hxx"
#include "vertex_buffer.hxx"
#include "color.hxx"
#include "collision_box.hxx"

class character : public actor
{
private:
	size_t count_sprite_width;
	size_t count_sprite_height;
	size_t size_sprite_px;
	///count sprite inline
public:
	virtual uint16_t animate(uint16_t first_frame, uint16_t count_frame,
			uint16_t last_frame, float delta_sec, float sec_now) = 0;
public:
	virtual ~character() = 0;
	void set_ch_vbo(vbo* ch_vbo_);
	vbo* get_character_vbo() const;
	void set_character_texture(om::texture* tex);
	om::texture* get_character_texture() const;
	void set_count_sprite_width(size_t width);
	size_t get_count_sprite_width() const;
	void set_count_sprite_height(size_t height);
	size_t get_count_sprite_height() const;
	void set_size_sprite_px(size_t px);
	size_t get_size_sprite_px() const;
};

void character::set_ch_vbo(vbo* ch_vbo_)
{
	set_actor_vbo(ch_vbo_);
}

vbo* character::get_character_vbo() const
{
	return get_actor_vbo();
}

void character::set_character_texture(om::texture* tex)
{
	set_actor_texture(tex);
}

om::texture* character::get_character_texture() const
{
	return get_actor_texture();
}

void character::set_count_sprite_width(size_t width)
{
	count_sprite_width = width;
}

size_t character::get_count_sprite_width() const
{
	return count_sprite_width;
}

void character::set_count_sprite_height(size_t height)
{
	count_sprite_height = height;
}

size_t character::get_count_sprite_height() const
{
	return count_sprite_height;
}

void character::set_size_sprite_px(size_t px)
{
	size_sprite_px = px;
}

size_t character::get_size_sprite_px() const
{
	return size_sprite_px;
}

character::~character()
{
}

class hero : public character
{
public:
	hero(om::texture* ch_tex_, size_t count_sp_width,
			size_t count_sp_height, size_t px);
	~hero();
	uint16_t animate(uint16_t first_frame, uint16_t last_frame,  uint16_t count_frame,
			float delta_sec, float sec_now);
};

hero::hero(om::texture* ch_tex_, size_t count_sp_width,
		size_t count_sp_height, size_t px)
{
	om::tri2 t_left;
	om::tri2 t_right;

	om::color color(1.0f, 1.0f, 1.0f, 1.0f);

	float tex_step_x = 1.0f / count_sp_width;
	float tex_step_y = 1.0f / count_sp_height;

	t_right.v[0].pos.x = -1.0f;
	t_right.v[0].pos.y = -1.0f;
	t_right.v[0].uv.x = 0.0f;
	t_right.v[0].uv.y = 1.0f - tex_step_y;
	t_right.v[0].c = color;

	t_right.v[1].pos.x = 1.0f;
	t_right.v[1].pos.y = -1.0f;
	t_right.v[1].uv.x = tex_step_x;
	t_right.v[1].uv.y = 1.0f - tex_step_y;
	t_right.v[1].c = color;

	t_right.v[2].pos.x = 1.0f;
	t_right.v[2].pos.y = 1.0f;
	t_right.v[2].uv.x = tex_step_x;
	t_right.v[2].uv.y = 1.0f;
	t_right.v[2].c = color;

	t_left.v[0].pos.x = 1.0f;
	t_left.v[0].pos.y = 1.0f;
	t_left.v[0].uv.x = tex_step_x;
	t_left.v[0].uv.y = 1.0f;
	t_left.v[0].c = color;

	t_left.v[1].pos.x = -1.0f;
	t_left.v[1].pos.y = 1.0f;
	t_left.v[1].uv.x = 0.0f;
	t_left.v[1].uv.y = 1.0f;
	t_left.v[1].c = color;

	t_left.v[2].pos.x = -1.0f;
	t_left.v[2].pos.y = -1.0f;
	t_left.v[2].uv.x = 0.0f;
	t_left.v[2].uv.y = 1.0f - tex_step_y;
	t_left.v[2].c = color;

	vbo* v = new vbo(t_right, t_left);

	set_ch_vbo(v);
	set_character_texture(ch_tex_);
	set_count_sprite_width(count_sp_width);
	set_count_sprite_height(count_sp_height);
	set_size_sprite_px(px);
}

uint16_t hero::animate(uint16_t first_frame, uint16_t last_frame, uint16_t count_frame,
		float delta_sec, float sec_now)
{
	const size_t count_sprite_line = get_count_sprite_width();

	uint16_t current_row = first_frame / count_sprite_line;

	static float cur_time = 0.f;
	static float last_time = 0.f;

	float tex_step_x = 1.0f / count_sprite_line;
	float tex_step_y = 1.0f / get_count_sprite_height();

	float left = first_frame * tex_step_x - current_row;
	float right = left + tex_step_x;

	float top = 1.f - tex_step_y * current_row;
	float botton = top - tex_step_y;

	vbo* v = get_character_vbo();

	if((cur_time - last_time) >= delta_sec)
	{
		if(first_frame <= last_frame)
		{
			v->triangles[0].v[0].uv.x = left;
			v->triangles[0].v[0].uv.y = botton;
			v->triangles[0].v[1].uv.x = right;
			v->triangles[0].v[1].uv.y = botton;
			v->triangles[0].v[2].uv.x = right;
			v->triangles[0].v[2].uv.y = top;

			v->triangles[1].v[0].uv.x = right;
			v->triangles[1].v[0].uv.y = top;
			v->triangles[1].v[1].uv.x = left;
			v->triangles[1].v[1].uv.y = top;
			v->triangles[1].v[2].uv.x = left;
			v->triangles[1].v[2].uv.y = botton;

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

hero::~hero()
{
}
