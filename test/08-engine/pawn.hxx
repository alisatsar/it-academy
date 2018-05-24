#pragma once

#include "actor.hxx"

#include "e_math.hxx"
#include "color.hxx"
#include "vertex_buffer.hxx"
#include "color.hxx"
#include "engine.hxx"
#include "collision_box.hxx"

class pawn : public actor
{
public:
	~pawn();
	void set_pawn_vbo(vbo* ch_vbo_);
	vbo* get_pawn_vbo() const;
	void set_pawn_texture(om::texture* tex);
	om::texture* get_pawn_texture() const;
};

void pawn::set_pawn_vbo(vbo* ch_vbo_)
{
	set_actor_vbo(ch_vbo_);
}

vbo* pawn::get_pawn_vbo() const
{
	return get_actor_vbo();
}

void pawn::set_pawn_texture(om::texture* tex)
{
	set_actor_texture(tex);
}

om::texture* pawn::get_pawn_texture() const
{
	return get_actor_texture();
}

pawn::~pawn()
{
}

class background : public pawn
{
private:
	om::vec2 position_l_b;
	om::vec2 position_r_t;
	om::vec2 tex_pos_l_b;
	om::vec2 tex_pos_r_t;
public:
	background(om::texture* ch_tex);
	void set_tex_pos(om::vec2 tex_pos);
	om::vec2 get_tex_pos_l_b() const;
	om::vec2 get_tex_pos_r_t() const;
	om::vec2 get_pos_l_b() const;
	~background();
};

background::background(om::texture* ch_tex_)
{
	om::tri2 t_left;
	om::tri2 t_right;

	om::color color(1.0, 1.0, 1.0, 1.0);

	t_right.v[0].pos.x = -1.0f;
	t_right.v[0].pos.y = -1.0f;
	t_right.v[0].uv.x = 0.0f;
	t_right.v[0].uv.y = 0.0f;
	t_right.v[0].c = color;

	t_right.v[1].pos.x = 1.0f;
	t_right.v[1].pos.y = -1.0f;
	t_right.v[1].uv.x = 1.0f;
	t_right.v[1].uv.y = 0.0f;
	t_right.v[1].c = color;

	t_right.v[2].pos.x = 1.0f;
	t_right.v[2].pos.y = 1.0f;
	t_right.v[2].uv.x = 1.0f;
	t_right.v[2].uv.y = 1.0f;
	t_right.v[2].c = color;

	t_left.v[0].pos.x = 1.0f;
	t_left.v[0].pos.y = 1.0f;
	t_left.v[0].uv.x = 1.0f;
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
	t_left.v[2].uv.y = 0.0f;
	t_left.v[2].c = color;

	vbo* v = new vbo(t_right, t_left);

	set_pawn_vbo(v);
	set_pawn_texture(ch_tex_);
}

void background::set_tex_pos(om::vec2 tex_pos)
{
	tex_pos_l_b = tex_pos;
	change_vbo_coord_x(tex_pos_l_b.x);
}

om::vec2 background::get_tex_pos_l_b() const
{
	return tex_pos_l_b;
}

om::vec2 background::get_tex_pos_r_t() const
{
	return tex_pos_r_t;
}

om::vec2 background::get_pos_l_b() const
{
	return position_l_b;
}
background::~background()
{
}

class rock : public pawn
{
public:
	collision_box col_box;
	rock(om::texture* ch_tex_);
	~rock();
};

rock::rock(om::texture* ch_tex_)
{
	om::tri2 t_left;
	om::tri2 t_right;

	om::color color(1.0, 1.0, 1.0, 1.0);

	t_right.v[0].pos.x = -1.f;
	t_right.v[0].pos.y = -1.f;
	t_right.v[0].uv.x = 0.f;
	t_right.v[0].uv.y = 0.f;
	t_right.v[0].c = color;

	t_right.v[1].pos.x = 1.f;
	t_right.v[1].pos.y = -1.f;
	t_right.v[1].uv.x = 1.f;
	t_right.v[1].uv.y = 0.f;
	t_right.v[1].c = color;

	t_right.v[2].pos.x = 1.f;
	t_right.v[2].pos.y = 1.f;
	t_right.v[2].uv.x = 1.f;
	t_right.v[2].uv.y = 1.f;
	t_right.v[2].c = color;

	t_left.v[0].pos.x = 1.f;
	t_left.v[0].pos.y = 1.f;
	t_left.v[0].uv.x = 1.f;
	t_left.v[0].uv.y = 1.f;
	t_left.v[0].c = color;

	t_left.v[1].pos.x = -1.f;
	t_left.v[1].pos.y = 1.f;
	t_left.v[1].uv.x = 0.0;
	t_left.v[1].uv.y = 1.0;
	t_left.v[1].c = color;

	t_left.v[2].pos.x = -1.f;
	t_left.v[2].pos.y = -1.f;
	t_left.v[2].uv.x = 0.f;
	t_left.v[2].uv.y = 0.f;
	t_left.v[2].c = color;

	vbo* v = new vbo(t_right, t_left);

	set_pawn_vbo(v);
	set_pawn_texture(ch_tex_);
}

rock::~rock()
{

}
