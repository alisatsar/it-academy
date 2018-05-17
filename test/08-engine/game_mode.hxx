#pragma once

#include <memory>

#include "controller.hxx"
#include "engine.hxx"
#include "character.hxx"

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

	pawn* backgr = nullptr;

	rock* roc = nullptr;

	hero_controller* hero_contr = nullptr;
	float x;
	hero_state hero_st;
	hero_state_render hero_st_ren = stay;
	direction hero_dir = right;
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
	om::vec2 pos = engine.get_pos_coor(50, 100);
    om::texture* tex = engine.create_texture("girl.png");
    if (nullptr == tex)
    {
        engine.log << "failed load texture\n";
        return;
    }

    character* he = new hero(tex, 7, 7, 90);

    om::vec2 x0;
    x0.y = (pos.y - 1 / he->get_count_sprite_width() / 2);
    x0.x = pos.x - engine.get_pos_coor(32, 0).x;

    om::vec2 x1;
    x1.y = pos.y + engine.get_pos_coor(0, 32).y - x0.y;
    x1.x = pos.x + engine.get_pos_coor(32, 0).x - x0.x;

    hero_contr = new hero_controller(he, pos, collision_box(x0, x1));

    tex = engine.create_texture("forest-1.png");

    backgr = new background(tex);

    tex = engine.create_texture("r.png");

    roc = new rock(tex);

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
		hero_dir = left;
		on_render();
	}
	else if (engine.is_key_down(om::keys::right))
	{
		hero_dir = right;
		hero_st_ren = run;
		hero_contr->hero_run(engine.get_time_from_init(), 0.01f);
		on_render();
	}
	else if (engine.is_key_down(om::keys::up))
	{
		hero_st_ren = jump;
		//hero.
		//hero_st.jump_frame = he->animate(hero_st.jump_frame, 16, 6, 0.1, engine.get_time_from_init());
		on_render();
	}
	else if (engine.is_key_down(om::keys::down))
	{
		hero_st_ren = trundle;
		//hero_st.trundle_frame = he->animate(hero_st.trundle_frame, 21, 4, 0.1, engine.get_time_from_init());
		on_render();
	}
	else
	{
		hero_st_ren = stay;
		hero_st.run_frame = 6;
		hero_st.jump_frame = 10;
		//hero_st.stay_frame = he->animate(hero_st.stay_frame, 5, 5, 0.2, engine.get_time_from_init());
		on_render();
	}
}

void girl_game::on_render()
{
	om::mat2x3 scale1 = om::mat2x3::scale(1);
	engine.render(*backgr->get_pawn_vbo(), backgr->get_pawn_texture(), scale1);

	switch (hero_st_ren){
	case stay:
		hero_contr->set_position(engine.get_pos_coor(50, 100));
		break;
	case run:

		break;
	case jump:
//		position.x += 0.02f;
//		if(hero_st.jump_frame >= 11 && hero_st.jump_frame <= 13)
//		{
//			position.y += 0.03f;
//		}
//		else if(hero_st.jump_frame >= 14 && hero_st.jump_frame <= 16)
//		{
//			position.y -= 0.02f;
//		}
		break;
	case trundle:
//		if(hero_st.trundle_frame == 17 || hero_st.trundle_frame == 21)
//		{
//			position.x += 0.0f;
//		}
//		else
//		{
//			position.x += 0.02f;
//		}
//		position.y = -0.55f;
		break;
	default:
		break;
	}

	om::mat2x3 scale = om::mat2x3::scale(0.2f);
	//om::mat2x3 reflection = om::mat2x3::reflection_x();
	om::mat2x3 move = om::mat2x3::move(hero_contr->get_position());
	om::mat2x3 m = scale * move;
	engine.render(*hero_contr->get_my_hero()->get_character_vbo(),
			hero_contr->get_my_hero()->get_character_texture(), m);

	om::mat2x3 scale2 = om::mat2x3::scale(0.1f);	;
	om::mat2x3 move2 = om::mat2x3::move(engine.get_pos_coor(300, 100));
	om::mat2x3 m_rock = scale2 * move2;
	engine.render(*roc->get_pawn_vbo(), roc->get_pawn_texture(), m_rock);
}



