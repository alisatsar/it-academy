#pragma once

#include "controller.hxx"
#include "character.hxx"
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

class hero_controller : public controller
{
private:
	character* my_hero = nullptr;
	hero_state hero_st;
	direction hero_direc = right;
	camera* cam = nullptr;
	om::vec2 current_offset{0.0f, 0.0f};
	bool big_col_box_flag = true;

public:
	hero_controller() = default;
	hero_controller(om::texture* tex);
	character* get_my_hero() const;

	om::vec2 get_current_offset() const { return current_offset; }

	camera* get_camera() const;

	void change_col_box();

	//movement of hero
	void hero_run(float sec);
	void hero_stay(float sec);
	void hero_falling(float sec);
	void hero_jump(float sec);
	void hero_trundle(float sec);

	bool test_collision(collision_box* box);
	~hero_controller();
	void check();
};

hero_controller::hero_controller(om::texture* tex)
{
	//create hero
	my_hero = new character(tex, 7, 7, 90);

	//create position in world
	float x_px = 100;
	float y_px = 170;

	om::vec2 pos_in_world = get_pos_coor(x_px, y_px);

	///now create collision box 64x64 px for our hero
	om::vec2 x0;

	x0.x = x_px - 32;
	x0.y = y_px - my_hero->get_size_sprite_px() / 2;

	om::vec2 x1;

	x1.x = x_px + 32;
	x1.y = x0.y + 64;

	set_collision_box(get_pos_coor(x0.x, x0.y), get_pos_coor(x1.x, x1.y));

	set_position(pos_in_world);

	cam = new camera();
	big_col_box_flag = true;
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
	check();
	//move camera to y = 0.0 position
	cam->set_pos_y(0.0f);

	//set big collision box
	big_col_box_flag = true;
	change_col_box();

	//change state
	hero_st.stay_frame = 0;
	hero_st.jump_frame = 10;
	hero_st.trundle_frame = 17;
	hero_st.falling_frame = 29;

	current_offset.y = 0.00f;
	//update matrix of camera (move x)
	cam->update_camera(-0.008f, 0.0f);
	//start animation hero state run
	hero_st.run_frame = my_hero->animate(hero_st.run_frame, 3, 9, 0.1, sec);
}

void hero_controller::check()
{
	if(hero_st.jump_frame == 11)
	{
		current_offset.y = 0.03f;
	}
	else if(hero_st.jump_frame == 12)
	{
		current_offset.y = 0.06f;
	}
	else if(hero_st.jump_frame == 13)
	{
		current_offset.y = 0.09f;
	}
	else if(hero_st.jump_frame == 14)
	{
		current_offset.y = -0.09f;
	}
	else if(hero_st.jump_frame == 14)
	{
		current_offset.y = -0.06f;
	}
	else if(hero_st.jump_frame == 14)
	{
		current_offset.y = -0.3f;
	}
}

void hero_controller::hero_jump(float sec)
{
	//change size of collision box
	big_col_box_flag = true;
	change_col_box();

	//return first state for animation
	hero_st.stay_frame = 0;
	hero_st.run_frame = 6;
	hero_st.trundle_frame = 17;
	hero_st.falling_frame = 29;

	//start animate jump state of our hero
	hero_st.jump_frame = my_hero->animate(hero_st.jump_frame, 8, 16, 0.1, sec);
	current_offset.x = -0.02f;
	current_offset.y = 0.0f;

	cam->update_camera(current_offset.x, current_offset.y);
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
		cam->set_pos_y(0.0f);
	}
	else
	{
		hero_st.falling_frame = my_hero->animate(hero_st.falling_frame, 5, 34, 0.1, sec);
	}
}

void hero_controller::hero_stay(float sec)
{
	big_col_box_flag = true;
	change_col_box();
	hero_st.jump_frame = 10;
	hero_st.run_frame = 6;
	hero_st.trundle_frame = 17;
	hero_st.falling_frame = 29;
	hero_st.stay_frame = my_hero->animate(hero_st.stay_frame, 5, 5, 0.3, sec);
	cam->set_pos_y(0.0f);
}

void hero_controller::hero_trundle(float sec)
{
	hero_st.jump_frame = 10;
	hero_st.run_frame = 6;
	hero_st.stay_frame = 0;
	hero_st.falling_frame = 29;

	hero_st.trundle_frame = my_hero->animate(hero_st.trundle_frame, 5, 21, 0.2, sec);
	current_offset.x = -0.015f;
	current_offset.y = 0.0f;

	cam->update_camera(current_offset.x, current_offset.y);
	big_col_box_flag = false;
	change_col_box();
}

bool hero_controller::test_collision(collision_box* box)
{
	collision_box c = this->get_collision_box();
	return (c.v1.x >= box->v0.x && c.v0.x <= box->v1.x) &&
			(c.v1.y >= box->v0.y && c.v0.y <= box->v1.y);
}

void hero_controller::change_col_box()
{
	static collision_box tmp = get_collision_box();
	om::vec2 pos2 = tmp.v1;
	pos2.x = tmp.v1.x;
	if(big_col_box_flag)
	{
		pos2.y = tmp.v1.y;
	}
	else
	{
		pos2.y = tmp.v1.y - 0.1;
	}

	set_collision_box(tmp.v0, pos2);
}

hero_controller::~hero_controller()
{
	delete my_hero;
	delete cam;
}
