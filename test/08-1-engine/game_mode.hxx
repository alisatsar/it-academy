#pragma once

#include <memory>
#include <vector>

#include "hero_controller.hxx"
#include "bat_controller.hxx"
#include "barrier_controller.hxx"
#include "pawn.hxx"
#include "engine.hxx"
#include "character.hxx"

struct lila
{
    virtual ~lila();
    virtual void on_initialize()                 			      = 0;
    virtual void on_event(om::event&)                             = 0;
    virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
    virtual void on_render()   = 0;
    virtual void on_clear() = 0;
};

extern std::unique_ptr<lila> om_tat_sat(om::engine&);

lila::~lila() = default;

class girl_game final : public lila
{
private:
	om::engine& engine;
	om::sound*   snd        = nullptr;

	//all backgrounds in game
	std::vector<pawn*> backgrounds;

	hero_controller* hero_contr = nullptr;
	bat_controller* bat_contr = nullptr;
	barrier_controller* barrier_contr = nullptr;

	om::texture* t = nullptr;
	float time;
public:

    explicit girl_game(om::engine& engine) : engine(engine){}

    void on_initialize() final;
    void on_event(om::event&) final;
    void on_update(std::chrono::milliseconds frame_delta) final;
    void on_render() final;
    void on_clear() final;
};

std::unique_ptr<lila> om_tat_sat(om::engine& e)
{
    e.log << "creating main game object..." << std::endl;
    auto game = std::make_unique<girl_game>(e);
    e.log << "finish creating main game object" << std::endl;
    return game;
}

void girl_game::on_initialize()
{
	om::texture* tex = engine.create_texture("girl.png");
    if (nullptr == tex)
    {
        engine.log << "failed load texture\n";
        return;
    }

    hero_contr = new hero_controller(tex);

    tex = engine.create_texture("bat-90x90.png");
    if (nullptr == tex)
    {
        engine.log << "failed load texture\n";
        return;
    }

    bat_contr = new bat_controller(tex);

   	std::vector<background> all_backgrounds;
   	std::uint32_t count_of_backgrounds;

   	std::ifstream file("background.txt");
   	assert(!!file);

   	file >> count_of_backgrounds;

   	background b;
    for (std::uint32_t i = 0; i < count_of_backgrounds; ++i)
   	{
   		file >> b;
        all_backgrounds.push_back(b);
   	}

    for(auto i : all_backgrounds)
    {
    	tex = engine.create_texture(i.name);
    	if (nullptr == tex)
    	{
    	    engine.log << "failed load texture\n";
    	    return;
    	}
    	backgrounds.push_back(new pawn(tex, i.position, om::vec2(2.0f, 2.0f)));
    }

    barrier_contr = new barrier_controller();
   	std::uint32_t count_of_barriers;

   	std::ifstream file2("barrier.txt");
   	assert(!!file2);

   	file2 >> count_of_barriers;

   	std::string name;

   	file2 >> name;

   	tex = engine.create_texture(name);
   	if (nullptr == tex)
   	{
   		engine.log << "failed load texture\n";
   		return;
	}

   	om::vec2 pos;
   	om::vec2 size;
   	for(std::uint32_t i = 0; i < count_of_barriers; ++i)
   	{
   		file2 >> pos.x >> pos.y >> size.x >> size.y;
   		barrier_contr->add_barrier(tex, pos, size);
   	}

    snd = engine.create_sound("t2_no_problemo.wav");
}

void girl_game::on_event(om::event& event)
{
    //std::cout << event << std::endl;
    switch (event.type)
    {
        case om::event_type::hardware:
            if (std::get<om::hardware_data>(event.info).is_reset)
            {
                engine.exit(EXIT_SUCCESS);
            }
            break;
        case om::event_type::input_key:
        {
            const auto& key_data = std::get<om::input_data>(event.info);
            if (key_data.is_down)
            {
                if (key_data.key == om::keys::button1)
                {
                    snd->play(om::sound::effect::once);
                }
                else if (key_data.key == om::keys::button2)
                {
                    snd->play(om::sound::effect::looped);
                }
            }
        }
        break;
    }
}

void girl_game::on_update(std::chrono::milliseconds /*frame_delta*/)
{
	time = engine.get_time_from_init();
	for(size_t i = 0; i < barrier_contr->get_size(); ++i)
	{
		if(hero_contr->test_collision(barrier_contr->get_barrier(i)->col_box))
		{
			hero_contr->hero_falling(time);
			on_render();
			return;
		}
	}
	if (engine.is_key_down(om::keys::left))
	{
		on_render();
	}
	else if (engine.is_key_down(om::keys::up))
	{
		hero_contr->hero_jump(time);
		barrier_contr->move_collision_box(hero_contr->get_current_offset().x,
				hero_contr->get_current_offset().y);
		on_render();
	}
	else if (engine.is_key_down(om::keys::down))
	{
		hero_contr->hero_trundle(time);
		barrier_contr->move_collision_box(hero_contr->get_current_offset().x,
						hero_contr->get_current_offset().y);
		on_render();
	}
	else
	{
		hero_contr->hero_run(time);
		barrier_contr->move_collision_box(-0.008f,
						hero_contr->get_current_offset().y);
		on_render();
	}
}

void girl_game::on_render()
{
	om::mat2x3 scale_backg = om::mat2x3::scale(1.0f);
	om::mat2x3 move_camera = hero_contr->get_camera()->get_camera_matrix();
	om::mat2x3 b = scale_backg * move_camera;
	//matrixes for background
	for(size_t i = 0; i < backgrounds.size(); ++i)
	{
		engine.render(*backgrounds[i]->get_actor_vbo(), backgrounds[i]->get_actor_texture(), b);
	}
	//matrixes for background

	om::mat2x3 mat_roc;
	for(size_t i = 0; i < barrier_contr->get_size(); ++i)
	{
		mat_roc = barrier_contr->get_barrier(i)->get_matrix();
		engine.render(*barrier_contr->get_barrier(i)->get_actor_vbo(),
				barrier_contr->get_barrier(i)->get_actor_texture(),  move_camera);
	}

	//hero
	om::mat2x3 hero_scale = om::mat2x3::scale(0.2f);
	om::mat2x3 hero_move = om::mat2x3::move(hero_contr->get_position_stay());
	om::mat2x3 hero_matrix = hero_scale * hero_move;
	engine.render(*hero_contr->get_my_hero()->get_character_vbo(),
			hero_contr->get_my_hero()->get_character_texture(), hero_matrix);

	om::mat2x3 bat_move = om::mat2x3::move(bat_contr->get_position());
	engine.render(*bat_contr->get_bat()->get_character_vbo(),
			bat_contr->get_bat()->get_character_texture(), hero_scale * bat_move * move_camera);

	//for testing render col box
	om::tri0 t;
	t.v[0].pos = hero_contr->get_collision_box().v0;
	t.v[1].pos.x = hero_contr->get_collision_box().v1.x;
	t.v[1].pos.y = hero_contr->get_collision_box().v0.y;
	t.v[2].pos = hero_contr->get_collision_box().v1;
	om::color c{1.0, 0.0, 0.0, 1.0};
	engine.render(t, c);

	om::tri0 t2;
	for(size_t i = 0; i < barrier_contr->get_size(); ++i)
	{
	t2.v[0].pos = barrier_contr->get_barrier(i)->get_colision_box()->v0;
	t2.v[1].pos.x = barrier_contr->get_barrier(i)->get_colision_box()->v1.x;
	t2.v[1].pos.y = barrier_contr->get_barrier(i)->get_colision_box()->v0.y;
	t2.v[2].pos = barrier_contr->get_barrier(i)->get_colision_box()->v1;
	engine.render(t2, c);
	}

//	om::tri0 t3;
//
//	for(size_t i = 0; i < rocks.size(); ++i)
//	{
//		t3.v[0].pos = rocks[i]->get_actor_vbo()->get_left_bottom();
//		t3.v[1].pos.x = rocks[i]->get_actor_vbo()->get_left_bottom().x;
//		t3.v[1].pos.y = rocks[i]->get_actor_vbo()->get_right_top().y;
//		t3.v[2].pos = rocks[i]->get_actor_vbo()->get_right_top();
//		engine.render(t3, c);
//	}

	//for testing render col box
	om::tri0 t4;
	t4.v[0].pos = bat_contr->get_collision_box().v0;
	t4.v[1].pos.x = bat_contr->get_collision_box().v1.x;
	t4.v[1].pos.y = bat_contr->get_collision_box().v0.y;
	t4.v[2].pos = bat_contr->get_collision_box().v1;
	engine.render(t4, c);

}

void girl_game::on_clear()
{
	for ( auto background : backgrounds ) { delete background; }
	delete barrier_contr;
	delete hero_contr;
	delete t;
	delete snd;
}

