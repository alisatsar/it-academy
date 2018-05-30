#pragma once

#include <cstdlib>
#include <memory>
#include <string_view>
#include <vector>

#include "pawn.hxx"
#include "engine.hxx"

class world
{
private:
	std::vector<pawn*> game_backgrounds;
public:
	world();
	~world() = default;
};

world::world()
{
}
