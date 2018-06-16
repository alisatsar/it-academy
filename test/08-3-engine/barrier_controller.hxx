#pragma once

#include <vector>

#include "controller.hxx"
#include "pawn.hxx"

class barrier_controller : public controller
{
private:
	std::vector<barrier*> barriers;
public:
	barrier_controller();
	~barrier_controller();

	void add_barrier(om::texture* tex, om::vec2& pos, om::vec2& size);
	size_t get_size() const { return barriers.size(); }
	barrier* get_barrier(uint32_t index) { return barriers.at(index); }
	void set_pos();

	void move_collision_box(float x, float y);
};

barrier_controller::barrier_controller()
{
	barriers.reserve(20);
}

void barrier_controller::add_barrier(om::texture* tex, om::vec2& pos, om::vec2& size)
{
	barriers.push_back(new barrier(tex, get_pos_coor(pos.x, pos.y), size));
}

void barrier_controller::move_collision_box(float x, float y)
{
	for(size_t i = 0; i < barriers.size(); ++i)
	{
		barriers[i]->col_box->move_x(x);
		barriers[i]->col_box->move_y(y);
	}
}

void barrier_controller::set_pos()
{
	for(size_t i = 0; i < barriers.size(); ++i)
	{
		barriers[i]->col_box->set_y(barriers[i]->col_box->get_position_state().y);
		barriers[i]->col_box->set_x(barriers[i]->col_box->get_position_state().x);
	}
}
barrier_controller::~barrier_controller()
{
	for(auto b : barriers)
	{
		delete b;
	}
}
