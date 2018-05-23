#pragma once

#include "vertex_buffer.hxx"
#include "e_math.hxx"
#include "engine.hxx"

class actor
{
protected:
	vbo* ch_vbo = nullptr;
	om::texture* ch_tex = nullptr;
public:
	virtual ~actor() = 0;
	void set_actor_vbo(vbo* ch_vbo_);
	void change_vbo_coord_x(float x);
	vbo* get_actor_vbo() const;
	void set_actor_texture(om::texture* tex);
	om::texture* get_actor_texture() const;
};

void actor::set_actor_vbo(vbo* ch_vbo_)
{
	ch_vbo = ch_vbo_;
}

vbo* actor::get_actor_vbo() const
{
	return ch_vbo;
}

void actor::set_actor_texture(om::texture* tex)
{
	ch_tex = tex;
}

void actor::change_vbo_coord_x(float x)
{
	ch_vbo->change_x_coord(x);
}

om::texture* actor::get_actor_texture() const
{
	return ch_tex;
}

actor::~actor()
{
	delete ch_vbo;
	delete ch_tex;
}
