#pragma once

#include <array>

#include "e_math.hxx"
#include "color.hxx"
#include "engine.hxx"
#include "vertex_buffer.hxx"

class character
{
private:
	uint32_t handler;
	vbo* ch_vbo;
	om::texture* ch_tex;
	size_t count_sprite;
	///count sprite inline
protected:
	virtual void animate() = 0;
public:
	virtual ~character() = 0;
	void set_ch_vbo(vbo* ch_vbo_);
	vbo* get_character_vbo() const;
	void set_character_texture(om::texture* tex);
	om::texture* get_character_texture() const;
	void set_count_sprite(size_t count_sp);
	size_t get_count_sprite() const;
};

void character::set_ch_vbo(vbo* ch_vbo_)
{
	ch_vbo = ch_vbo_;
}

vbo* character::get_character_vbo() const
{
	return ch_vbo;
}

void character::set_character_texture(om::texture* tex)
{
	ch_tex = tex;
}

om::texture* character::get_character_texture() const
{
	return ch_tex;
}

void character::set_count_sprite(size_t count_sp)
{
	count_sprite = count_sp;
}

size_t character::get_count_sprite() const
{
	return count_sprite;
}

class hero : public character
{
public:
	hero(om::texture* ch_tex_, float count);
	~hero();

};

character::~character()
{
	delete ch_vbo;
	delete ch_tex;
}

hero::hero(om::texture* ch_tex_, float count)
{
	om::tri2 t_left;
	om::tri2 t_right;

	float tex_step = 1.f / count;

	t_right.v[0].pos.x = -1.0;
	t_right.v[0].pos.y = -1.0;
	t_right.v[0].uv.x = 0.0;
	t_right.v[0].uv.y = 1.0 - tex_step;

	t_right.v[1].pos.x = 1.0;
	t_right.v[1].pos.y = -1.0;
	t_right.v[1].uv.x = tex_step;
	t_right.v[1].uv.y = 1.0 - tex_step;

	t_right.v[2].pos.x = 1.0;
	t_right.v[2].pos.y = 1.0;
	t_right.v[2].uv.x = tex_step;
	t_right.v[2].uv.y = 1.0;

	t_left.v[0].pos.x = -1.0;
	t_left.v[0].pos.y = -1.0;
	t_left.v[0].uv.x = tex_step;
	t_left.v[0].uv.y = 1.0;

	t_left.v[1].pos.x = -1.0;
	t_left.v[1].pos.y = 1.0;
	t_left.v[1].uv.x = 0.0;
	t_left.v[1].uv.y = 1.0;

	t_left.v[2].pos.x = -1.0;
	t_left.v[2].pos.y = -1.0;
	t_left.v[2].uv.x = 0.0;
	t_left.v[2].uv.y = 1.0 - tex_step;

	om::tri2 triangles[2];
	triangles[0] = t_left;
	triangles[1] = t_right;
	std::size_t size = 2;
	vbo* v = new vertex_buffer_impl(triangles, size);

	set_ch_vbo(v);
	set_character_texture(ch_tex_);
}


hero::~hero()
{
}
