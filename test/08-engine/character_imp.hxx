#pragma once

#include "actor.hxx"

#include "e_math.hxx"
#include "color.hxx"
#include "vertex_buffer.hxx"
#include "color.hxx"
#include "collision_box.hxx"

class character : public actor
{
private:
	size_t count_sprite;
	collision_box col_box;
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
	void set_count_sprite(size_t count_sp);
	size_t get_count_sprite() const;
	void set_collision_box(om::vec2 v0, om::vec2 v1);
	collision_box get_collision_box() const;
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

void character::set_count_sprite(size_t count_sp)
{
	count_sprite = count_sp;
}

size_t character::get_count_sprite() const
{
	return count_sprite;
}

void character::set_collision_box(om::vec2 v0, om::vec2 v1)
{
	col_box.v0 = v0;
	col_box.v1 = v1;
}

collision_box character::get_collision_box() const
{
	return col_box;
}

character::~character()
{
}

class hero : public character
{
public:
	hero(om::texture* ch_tex_, float count);
	~hero();
	uint16_t animate(uint16_t first_frame, uint16_t last_frame,  uint16_t count_frame,
			float delta_sec, float sec_now);
};

hero::hero(om::texture* ch_tex_, float count)
{
	om::tri2 t_left;
	om::tri2 t_right;

	om::color color(1.0, 1.0, 1.0, 1.0);

	float tex_step = 1.f / count;

	t_right.v[0].pos.x = -1.f;
	t_right.v[0].pos.y = -1.f;
	t_right.v[0].uv.x = 0.0;
	t_right.v[0].uv.y = 1.0 - tex_step;
	t_right.v[0].c = color;

	t_right.v[1].pos.x = 1.f;
	t_right.v[1].pos.y = -1.f;
	t_right.v[1].uv.x = tex_step;
	t_right.v[1].uv.y = 1.0 - tex_step;
	t_right.v[1].c = color;

	t_right.v[2].pos.x = 1.f;
	t_right.v[2].pos.y = 1.f;
	t_right.v[2].uv.x = tex_step;
	t_right.v[2].uv.y = 1.0;
	t_right.v[2].c = color;

	t_left.v[0].pos.x = 1.f;
	t_left.v[0].pos.y = 1.f;
	t_left.v[0].uv.x = tex_step;
	t_left.v[0].uv.y = 1.0;
	t_left.v[0].c = color;

	t_left.v[1].pos.x = -1.f;
	t_left.v[1].pos.y = 1.f;
	t_left.v[1].uv.x = 0.0;
	t_left.v[1].uv.y = 1.0;
	t_left.v[1].c = color;

	t_left.v[2].pos.x = -1.f;
	t_left.v[2].pos.y = -1.f;
	t_left.v[2].uv.x = 0.0;
	t_left.v[2].uv.y = 1.0 - tex_step;
	t_left.v[2].c = color;

	vbo* v = new vbo(t_right, t_left);

	set_ch_vbo(v);
	set_character_texture(ch_tex_);
	set_count_sprite(count);
}

uint16_t hero::animate(uint16_t first_frame, uint16_t last_frame, uint16_t count_frame,
		float delta_sec, float sec_now)
{
	const size_t count_sprite_line = get_count_sprite();

	uint16_t current_row = first_frame / count_sprite_line;

	static float cur_time = 0.f;
	static float last_time = 0.f;

	const float tex_step = 1.f / count_sprite_line;

	float left = first_frame * tex_step - current_row;
	float right = left + tex_step;

	float top = 1.f - tex_step * current_row;
	float botton = top - tex_step;

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
