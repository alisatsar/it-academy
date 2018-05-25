#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <string_view>
#include "pawn.hxx"
#include "engine.hxx"

class world
{
private:
	std::vector<pawn*> backgrounds;
public:
	world();
	~world() = default;
//	void add_background(om::texture* texture, float position);
//	size_t get_size_backgrounds() const;
//	pawn* get_background(size_t index);
};

world::world()
{
}

//void world::add_background(om::texture* texture, float position)
//{
//	om::vec2 size;
//	size.x = texture->get_width();
//	size.y = texture->get_height();
//
//	pawn* background = new pawn(texture);
//
//	background->change_pos_x(position);
//
//	backgrounds.push_back(background);
//}
//
//size_t world::get_size_backgrounds() const
//{
//	return backgrounds.size();
//}
//
//pawn* world::get_background(size_t index)
//{
//	if (index < backgrounds.size())
//	{
//		return backgrounds[index];
//	}
//	return nullptr;
//}
