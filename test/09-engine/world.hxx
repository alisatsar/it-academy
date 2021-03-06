#pragma once

#include <map>
#include <cstdint>
#include <memory>
#include "pawn.hxx"

class world
{
private:
	std::map<uint32_t, std::shared_ptr<pawn>> pawns;
public:
	world() = default;
	~world() = default;
	bool add_pawn(uint32_t key, pawn* pawn_);
};

bool world::add_pawn(uint32_t key, pawn* pawn_)
{
	pawns[key] = pawn_;
}
