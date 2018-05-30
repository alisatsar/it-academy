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
	pawn(om::texture* tex_);
	pawn(om::texture* tex_, om::vec2 position, om::vec2 size);
	~pawn();
	void change_pos_x(float x);
};

pawn::pawn(om::texture* tex_)
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

	set_actor_vbo(t_right, t_left);

	set_actor_texture(tex_);
}

///position - left and bottom point
pawn::pawn(om::texture* tex_, om::vec2 position, om::vec2 size)
{
	om::tri2 t_left;
	om::tri2 t_right;

	om::color color(1.0, 1.0, 1.0, 1.0);

	t_right.v[0].pos.x = position.x;
	t_right.v[0].pos.y = position.y;
	t_right.v[0].uv.x = 0.0f;
	t_right.v[0].uv.y = 0.0f;
	t_right.v[0].c = color;

	t_right.v[1].pos.x = position.x + size.x;
	t_right.v[1].pos.y = position.y;
	t_right.v[1].uv.x = 1.0f;
	t_right.v[1].uv.y = 0.0f;
	t_right.v[1].c = color;

	t_right.v[2].pos.x = position.x + size.x;
	t_right.v[2].pos.y = position.y + size.y;
	t_right.v[2].uv.x = 1.0f;
	t_right.v[2].uv.y = 1.0f;
	t_right.v[2].c = color;

	t_left.v[0].pos.x = position.x + size.x;
	t_left.v[0].pos.y = position.y + size.y;
	t_left.v[0].uv.x = 1.0f;
	t_left.v[0].uv.y = 1.0f;
	t_left.v[0].c = color;

	t_left.v[1].pos.x = position.x;
	t_left.v[1].pos.y = position.y + size.y;
	t_left.v[1].uv.x = 0.0f;
	t_left.v[1].uv.y = 1.0f;
	t_left.v[1].c = color;

	t_left.v[2].pos.x = position.x;
	t_left.v[2].pos.y = position.y;
	t_left.v[2].uv.x = 0.0f;
	t_left.v[2].uv.y = 0.0f;
	t_left.v[2].c = color;

	set_actor_vbo(t_right, t_left);

	set_actor_texture(tex_);
}


void pawn::change_pos_x(float x)
{
	change_vbo_coord_x(x);
}

pawn::~pawn(){}

class barrier : public pawn
{
public:
	collision_box* col_box;
	om::mat2x3 matrix;
	barrier(om::texture* tex_, om::vec2 position, om::vec2 size);
	collision_box* get_colision_box() const;
	om::mat2x3 get_matrix() const { return matrix; }
	void move_coolision_box(om::mat2x3 matrix);
};

barrier::barrier(om::texture* tex_, om::vec2 position, om::vec2 size) :
		pawn(tex_, position, size)
{
	om::mat2x3 scale = om::mat2x3::scale(size.x);
	om::mat2x3 move = om::mat2x3::move(position);

	matrix =  scale * move;

	col_box = new collision_box(position, om::vec2(position.x + size.x, position.y + size.y));
}

collision_box* barrier::get_colision_box() const
{
	return col_box;
}

void barrier::move_coolision_box(om::mat2x3 matrix)
{
	col_box->move_x(matrix.delta.x);
}
