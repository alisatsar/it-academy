#pragma once

#include <memory>

#include "controller.hxx"
#include "engine.hxx"
#include "character.hxx"

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
	std::vector<om::texture*> all_textures;
	om::sound*   snd        = nullptr;

	background* backgr = nullptr;

	hero_controller* hero_contr = nullptr;
	rock_controller* r_controller = nullptr;
	float x;
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

    character* he = new hero(tex, 7, 7, 90);

    float x_px = 100;
    float y_px = 100;
    om::vec2 pos = engine.get_pos_coor(x_px, y_px);

    ///now create collision box 64x64 px
    om::vec2 x0;

    x0.x = x_px - 32;
    x0.y = y_px - he->get_size_sprite_px() / 2;

    om::vec2 x1;

    x1.x = x_px + 32;
    x1.y = x0.y + 64;

    om::vec2 pos0 = engine.get_pos_coor(x0.x, x0.y);
    om::vec2 pos1 = engine.get_pos_coor(x1.x, x1.y);

    hero_contr = new hero_controller(he, pos, collision_box(pos0, pos1),
    		engine.get_tex_coor(window_width / 3, window_height));

    tex = engine.create_texture("level.png");

    pos0 = engine.get_tex_coor(0.0f, 0.0f);
    pos1 = engine.get_tex_coor(window_width / 3,  window_height);

    backgr = new background(tex, pos0, pos1);

    tex = engine.create_texture("r.png");

    rock* roc = new rock(tex);

    pos = engine.get_pos_coor(180, 130);

    r_controller = new rock_controller(roc, pos);

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
	if(hero_contr->test_collision(r_controller->get_collision_box()))
	{
		std::cout << "COLLISION" << std::endl;
	}

	if (engine.is_key_down(om::keys::left))
	{
		on_render();
	}
	else if (engine.is_key_down(om::keys::right))
	{
		hero_contr->hero_run(engine.get_time_from_init(), 0.01f);
		backgr->set_tex_pos(hero_contr->get_camera_pos());
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
}

void girl_game::on_render()
{
	om::mat2x3 scale1 = om::mat2x3::scale(1);
	engine.render(*backgr->get_pawn_vbo(), backgr->get_pawn_texture(), scale1);

	om::mat2x3 scale = om::mat2x3::scale(0.2f);
	//om::mat2x3 reflection = om::mat2x3::reflection_x();
	om::mat2x3 move = om::mat2x3::move(hero_contr->get_position());
	om::mat2x3 m = scale * move;
	engine.render(*hero_contr->get_my_hero()->get_character_vbo(),
			hero_contr->get_my_hero()->get_character_texture(), m);

	om::mat2x3 scale_r = om::mat2x3::scale(0.1f);
	om::mat2x3 move_r = om::mat2x3::move(r_controller->get_position());
	om::mat2x3 m_r = scale_r * move_r;
	engine.render(*r_controller->get_rock()->get_pawn_vbo(),
			r_controller->get_rock()->get_pawn_texture(), m_r);
}



