#pragma once

#include <memory>
#include <vector>

#include "controller.hxx"
#include "engine.hxx"
#include "character.hxx"
#include "world.hxx"

///screen dimension compile-runtime
constexpr size_t window_width{640};
constexpr size_t window_height{480};

///dimension of the level
constexpr size_t level_width{1280};
constexpr size_t level_height{480};

struct lila
{
    virtual ~lila();
    virtual void on_initialize()                 			      = 0;
    virtual void on_event(om::event&)                             = 0;
    virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
    virtual void on_render()   = 0;
};

extern std::unique_ptr<lila> om_tat_sat(om::engine&);

lila::~lila() = default;

class girl_game final : public lila
{
private:
	om::engine& engine;
	om::sound*   snd        = nullptr;

	std::vector<pawn*> backgrounds_x;

	std::vector<barrier*> rocks;

	hero_controller* hero_contr = nullptr;
	camera* cam = nullptr;
	om::texture* t = nullptr;

	world* game_world = nullptr;
public:

    explicit girl_game(om::engine& engine)
        : engine(engine)
    {
    }

    void on_initialize() final;
    void on_event(om::event&) final;
    void on_update(std::chrono::milliseconds frame_delta) final;
    void on_render() final;
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

    cam = new camera();

    //create our hero
    character* hero = new character(tex, 7, 7, 90);

    //create position in world
    float x_px = 100;
    float y_px = 100;
    om::vec2 pos = engine.get_pos_coor(x_px, y_px);

    ///now create collision box 64x64 px for our hero
    om::vec2 x0;

    x0.x = x_px - 32;
    x0.y = y_px - hero->get_size_sprite_px() / 2;

    om::vec2 x1;

    x1.x = x_px + 32;
    x1.y = x0.y + 64;

    om::vec2 pos0 = engine.get_pos_coor(x0.x, x0.y);
    om::vec2 pos1 = engine.get_pos_coor(x1.x, x1.y);

    hero_contr = new hero_controller(hero, pos, collision_box(pos0, pos1));

   	std::vector<backgrounds> all_backgrounds;
   	std::uint32_t count_of_backgrounds;

   	std::ifstream file("background.txt");
   	assert(!!file);

   	file >> count_of_backgrounds;

   	backgrounds b;
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
    	backgrounds_x.push_back(new pawn(tex, i.position));
    }

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

   	barrier* rock;
   	om::vec2 size;
    for (std::uint32_t i = 0; i < count_of_barriers; ++i)
   	{
    	file2 >> pos.x >> pos.y >> size.x >> size.y;
   		rock = new barrier(tex, engine.get_pos_coor(pos.x, pos.y), size);
   		rocks.push_back(rock);
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
	if (engine.is_key_down(om::keys::left))
	{
		on_render();
	}
	else if (engine.is_key_down(om::keys::right))
	{
		//call controller
		hero_contr->hero_run(engine.get_time_from_init());
		cam->update_camera(-0.004f, 0.0f);
		on_render();
	}
	else if (engine.is_key_down(om::keys::up))
	{
		//hero_st.jump_frame = he->animate(hero_st.jump_frame, 16, 6, 0.1, engine.get_time_from_init());
		on_render();
	}
	else if (engine.is_key_down(om::keys::down))
	{
		//hero_st.trundle_frame = he->animate(hero_st.trundle_frame, 21, 4, 0.1, engine.get_time_from_init());
		on_render();
	}
	else
	{
		//hero_st.stay_frame = he->animate(hero_st.stay_frame, 5, 5, 0.2, engine.get_time_from_init());
		on_render();
	}
	for(size_t i = 0; i < rocks.size(); ++i)
	{
		if(hero_contr->test_collision(rocks[i]->col_box))
		{
			std::cout << "col" << std::endl;
		}
	}
}

void girl_game::on_render()
{
	om::mat2x3 scale_backg = om::mat2x3::scale(1.0f);
	om::mat2x3 move_camera = cam->get_camera_matrix();
	om::mat2x3 b = scale_backg * move_camera;
	//matrixes for background
	for(size_t i = 0; i < backgrounds_x.size(); ++i)
	{
		engine.render(*backgrounds_x[i]->get_actor_vbo(), backgrounds_x[i]->get_actor_texture(), b);
	}
	//matrixes for background

	om::mat2x3 mat_roc;
	for(size_t i = 0; i < rocks.size(); ++i)
	{
		mat_roc = rocks[i]->get_matrix();
		engine.render(*rocks[i]->get_actor_vbo(), rocks[i]->get_actor_texture(),  mat_roc * move_camera);
	}

	//hero
	om::mat2x3 hero_scale = om::mat2x3::scale(0.2f);
	om::mat2x3 hero_move = om::mat2x3::move(hero_contr->get_position());
	om::mat2x3 hero_matrix = hero_scale * hero_move;
	engine.render(*hero_contr->get_my_hero()->get_character_vbo(),
			hero_contr->get_my_hero()->get_character_texture(), hero_matrix);

}

