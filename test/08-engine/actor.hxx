#pragma once

#include "e_math.hxx"
#include "engine.hxx"

class actor
{
protected:
	uint32_t handler;
	vbo* ch_vbo;
	om::texture* ch_tex;
public:
	virtual ~actor() = 0;
	void set_actor_vbo(vbo* ch_vbo_);
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

om::texture* actor::get_actor_texture() const
{
	return ch_tex;
}

actor::~actor()
{
	delete ch_vbo;
	delete ch_tex;
}
