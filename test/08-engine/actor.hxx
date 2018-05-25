#pragma once

#include "vertex_buffer.hxx"
#include "e_math.hxx"
#include "engine.hxx"

class actor
{
protected:
	vbo* actor_vbo = nullptr;
	om::texture* tex = nullptr;
public:
	virtual ~actor() = 0;
	//set
	void set_actor_vbo(om::tri2 t_right, om::tri2 t_left);
	void set_actor_texture(om::texture* tex_);
	//get
	vbo* get_actor_vbo() const;
	om::texture* get_actor_texture() const;
	//change_vbo
	void change_vbo_coord_x(float x);
	void change_vbo_coord_y(float y);
	void changet_texture_coord(float left, float right, float top, float bottom);
};

void actor::set_actor_vbo(om::tri2 t_right, om::tri2 t_left)
{
	actor_vbo = new vbo(t_right, t_left);
}

void actor::set_actor_texture(om::texture* tex_)
{
	tex = tex_;
}

vbo* actor::get_actor_vbo() const
{
	return actor_vbo;
}

om::texture* actor::get_actor_texture() const
{
	return tex;
}

void actor::change_vbo_coord_x(float x)
{
	actor_vbo->change_x_coord(x);
}

void actor::change_vbo_coord_y(float y)
{
	actor_vbo->change_y_coord(y);
}

void actor::changet_texture_coord(float left, float right, float top, float bottom)
{
	actor_vbo->change_vbo_tex(left, right, top, bottom);
}


actor::~actor()
{
	delete actor_vbo;
	delete tex;
}
