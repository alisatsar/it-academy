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
};

enum hero_state_render
{
	stay,
	run,
	jump,
	trundle
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
	hero_state_render hero_st_ren = stay;
	direction hero_direc = right;
	//camera* cam = nullptr;

public:
	hero_controller() = default;
	hero_controller(character* hero, om::vec2 pos, collision_box col);
	character* get_my_hero() const;
	void hero_run(float sec, float offset_x);
	bool test_collision(collision_box box);
	glm::mat4 get_camera_pos() const;
	~hero_controller();
};

hero_controller::hero_controller(character* hero, om::vec2 pos,
		collision_box col) :
		controller(pos, col)
{
	my_hero = hero;
}

character* hero_controller::get_my_hero() const
{
	return my_hero;
}

void hero_controller::hero_run(float sec, float offset_x)
{
	hero_st.run_frame = my_hero->animate(hero_st.run_frame, 9, 3, 0.1, sec);
	set_position_x(get_position().x + offset_x);
	move_col_box_x(offset_x);
}

bool hero_controller::test_collision(collision_box box)
{
	collision_box c = this->get_collision_box();
	return (c.v1.x >= box.v0.x && c.v0.x <= box.v1.x) &&
			(c.v1.y >= box.v0.y && c.v0.y <= box.v1.y);
}

hero_controller::~hero_controller()
{
	delete my_hero;
}

class rock_controller : public controller
{
private:
	rock* my_rock = nullptr;
	float size = 0.1f;
public:
	~rock_controller();
	rock_controller(rock* r, om::vec2 pos);
	rock* get_rock() const;
};

rock_controller::rock_controller(rock* r, om::vec2 pos)
	: controller(pos)
{
	my_rock = r;
	om::vec2 v0;
	v0.x = get_position().x - size / 2.0f;
	v0.y = get_position().y - size / 2.0f;

	om::vec2 v1;
	v1.x = get_position().x + size / 2.0f;
	v1.y = get_position().y + size / 2.0f;

	set_collision_box(v0, v1);
}

rock* rock_controller::get_rock() const
{
	return my_rock;
}

rock_controller::~rock_controller()
{
}
