#pragma once

#include "actor.hxx"

#include "e_math.hxx"
#include "color.hxx"
#include "vertex_buffer.hxx"
#include "color.hxx"
#include "engine.hxx"

class pawn : public actor
{
public:
	virtual ~pawn() = 0;
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
public:
	background(om::texture* ch_tex_);
	~background();
};

background::background(om::texture* ch_tex_)
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


background::~background()
{
}
