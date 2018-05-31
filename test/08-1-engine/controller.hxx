#pragma once

#include "character.hxx"
#include "pawn.hxx"
#include "collision_box.hxx"
#include "e_math.hxx"
#include "camera.hxx"

struct hero_state
{
	uint16_t stay_frame = 0;
	uint16_t run_frame = 6;
	uint16_t jump_frame = 10;
	uint16_t trundle_frame = 17;
	uint16_t falling_frame = 29;
};

enum direction
{
	right,
	left
};

class controller
{
private:
	om::vec2 position;
	collision_box col_box;
public:
	controller() = default;
	controller(om::vec2 pos, collision_box col);
	controller(om::vec2 pos);
	void set_position(om::vec2 pos);
	void set_position_x(float x);
	void set_position_y(float y);
	om::vec2 get_position() const;
	void set_collision_box(om::vec2 v0, om::vec2 v1);
	collision_box get_collision_box() const;
	void move_col_box_x(float x);
	void move_col_box_y(float y);
};

controller::controller(om::vec2 pos, collision_box col) :
		position(pos), col_box(col)
{
}

controller::controller(om::vec2 pos)
{
	position = pos;
}

void controller::set_position(om::vec2 pos)
{
	position = pos;
}

void controller::set_position_x(float x)
{
	position.x = x;
}

void controller::set_position_y(float y)
{
	position.y = y;
}

om::vec2 controller::get_position() const
{
	return position;
}

void controller::set_collision_box(om::vec2 v0, om::vec2 v1)
{
	col_box.v0 = v0;
	col_box.v1 = v1;
}

collision_box controller::get_collision_box() const
{
	return col_box;
}

void controller::move_col_box_x(float x)
{
	col_box.move_x(x);
}

void controller::move_col_box_y(float y)
{
	col_box.move_y(y);
}

class hero_controller : public controller
{
private:
	character* my_hero = nullptr;
	hero_state hero_st;
	direction hero_direc = right;
	camera* cam = nullptr;
	om::vec2 current_offset{0.0f, 0.0f};
	om::vec2 position_stay;

public:
	hero_controller() = default;
	hero_controller(character* hero, om::vec2 pos, collision_box col);
	character* get_my_hero() const;

	om::vec2 get_current_offset() const { return current_offset; }

	camera* get_camera() const;

	//movement of hero
	void hero_run(float sec);
	void hero_stay(float sec);
	void hero_falling(float sec);
	void hero_jump(float sec);

	bool test_collision(collision_box* box);
	~hero_controller();
};

hero_controller::hero_controller(character* hero, om::vec2 pos, collision_box col) :
		controller(pos, col)
{
	position_stay = pos;
	cam = new camera();
	my_hero = hero;
}

character* hero_controller::get_my_hero() const
{
	return my_hero;
}

camera* hero_controller::get_camera() const
{
	return cam;
}

void hero_controller::hero_run(float sec)
{
	cam->update_camera(-0.004f, 0.0f);
	hero_st.run_frame = my_hero->animate(hero_st.run_frame, 3, 9, 0.1, sec);
}

void hero_controller::hero_jump(float sec)
{
	hero_st.stay_frame = 0;
	hero_st.run_frame = 6;
	hero_st.trundle_frame = 17;
	hero_st.falling_frame = 29;

	hero_st.jump_frame = my_hero->animate(hero_st.jump_frame, 8, 17, 0.1, sec);
	current_offset.y = 0.0f;

	cam->update_camera(-0.01f, current_offset.y);
	if(hero_st.jump_frame >= 11 && hero_st.jump_frame <= 13)
	{
		current_offset.y = -0.03f;
		cam->update_camera(0.0f, current_offset.y);
	}
	else if(hero_st.jump_frame >= 14 && hero_st.jump_frame <= 16)
	{
		current_offset.y = 0.03f;
		cam->update_camera(0.0f, current_offset.y);
	}
}

void hero_controller::hero_falling(float sec)
{
	if(hero_st.falling_frame == 34)
	{
		hero_st.falling_frame = my_hero->animate(34, 0, 34, 0.1, sec);
	}
	else
	{
		hero_st.falling_frame = my_hero->animate(hero_st.falling_frame, 5, 34, 0.1, sec);
	}
}

void hero_controller::hero_stay(float sec)
{
	hero_st.jump_frame = 10;
	hero_st.run_frame = 6;
	hero_st.trundle_frame = 17;
	hero_st.falling_frame = 29;
	hero_st.stay_frame = my_hero->animate(hero_st.stay_frame, 5, 5, 0.3, sec);
	//cam->set_pos(om::vec2(0.0f, position_stay.y));
}

bool hero_controller::test_collision(collision_box* box)
{
	collision_box c = this->get_collision_box();
	return (c.v1.x >= box->v0.x && c.v0.x <= box->v1.x) &&
			(c.v1.y >= box->v0.y && c.v0.y <= box->v1.y);
}

hero_controller::~hero_controller()
{
	delete my_hero;
}


