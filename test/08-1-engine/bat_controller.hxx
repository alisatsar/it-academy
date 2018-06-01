#pragma once

#include "controller.hxx"
#include "character.hxx"

struct bat_state
{
	uint16_t stay_frame = 0;
	uint16_t attack_frame = 5;
	uint16_t falling_frame = 8;
};

class bat_controller : public controller
{
private:
	character* bat = nullptr;
	bat_state bat_st;
public:
	bat_controller(om::texture* tex);
	~bat_controller();

	character* get_bat() const { return bat; }

	void bat_stay(float sec);
};

bat_controller::bat_controller(om::texture* tex)
{
	bat = new character(tex, 4, 4, 90);

	//create position in world
	float x_px = 200;
	float y_px = 200;

	om::vec2 pos_in_world = get_pos_coor(x_px, y_px);

	om::vec2 x0;

	x0.x = x_px - 32;
	x0.y = x_px - 32;

	om::vec2 x1;

	x1.x = x_px + 32;
	x1.y = x0.y + 32;

	set_collision_box(get_pos_coor(x0.x, x0.y), get_pos_coor(x1.x, x1.y));

	set_position(pos_in_world);
}

void bat_controller::bat_stay(float sec)
{
	bat_st.stay_frame = bat->animate(bat_st.stay_frame, 4, 4, 0.05, sec);
}

bat_controller::~bat_controller()
{
	delete bat;
}


