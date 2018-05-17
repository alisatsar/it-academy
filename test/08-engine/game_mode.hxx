#pragma once
#include <memory>
#include "engine.hxx"
#include "pawn.hxx"
#include "character_imp.hxx"

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

class girl_game final : public lila
{
private:
	om::engine& engine;
	std::vector<om::texture*> all_textures;
	om::sound*   snd        = nullptr;

	character* he = nullptr;

	pawn* backgr = nullptr;

	om::vec2 position;
	hero_state hero_st;
	hero_state_render hero_st_ren = stay;
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
	position = pos;
    om::texture* tex = engine.create_texture("gitl.png");
    if (nullptr == tex)
    {
        engine.log << "failed load texture\n";
        return;
    }

    he = new hero(tex, 7);

    om::vec2 x0;
    x0.y = (pos.y - 1 / he->get_count_sprite() / 2);
    x0.x = pos.x - engine.get_pos_coor(32, 0).x;

    om::vec2 x1;
    x1.y = pos.y + engine.get_pos_coor(0, 32).y - x0.y;
    x1.x = pos.x + engine.get_pos_coor(32, 0).x - x0.x;

    he->set_collision_box(x0, x1);

    tex = engine.create_texture("forest-1.png");

    backgr = new background(tex);

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
		hero_st_ren = run;
		hero_st.run_frame = he->animate(hero_st.run_frame, 9, 3, 0.1, engine.get_time_from_init());
		on_render();
	}
	else if (engine.is_key_down(om::keys::up))
	{
		hero_st_ren = jump;
		hero_st.jump_frame = he->animate(hero_st.jump_frame, 16, 6, 0.1, engine.get_time_from_init());
		on_render();
	}
	else if (engine.is_key_down(om::keys::down))
	{
		hero_st_ren = trundle;
		hero_st.trundle_frame = he->animate(hero_st.trundle_frame, 21, 4, 0.1, engine.get_time_from_init());
		on_render();
	}
	else
	{
		hero_st_ren = stay;
		hero_st.run_frame = 6;
		hero_st.jump_frame = 10;
		hero_st.stay_frame = he->animate(hero_st.stay_frame, 5, 5, 0.2, engine.get_time_from_init());
		on_render();
	}
}

void girl_game::on_render()
{
	om::mat2x3 scale1 = om::mat2x3::scale(1);
	engine.render(*backgr->get_pawn_vbo(), backgr->get_pawn_texture(), scale1);

	switch (hero_st_ren){
	case stay:
		position.y = engine.get_pos_coor(50, 100).y;
		break;
	case run:
		position.y = engine.get_pos_coor(50, 100).y;
		position.x += 0.01f;
		break;
	case jump:
		position.x += 0.02f;
		if(hero_st.jump_frame >= 11 && hero_st.jump_frame <= 13)
		{
			position.y += 0.03f;
		}
		else if(hero_st.jump_frame >= 14 && hero_st.jump_frame <= 16)
		{
			position.y -= 0.02f;
		}
		break;
	case trundle:
		if(hero_st.trundle_frame == 17 || hero_st.trundle_frame == 21)
		{
			position.x += 0.0f;
		}
		else
		{
			position.x += 0.02f;
		}
		position.y = -0.55f;
		break;
	default:
		break;
	}

	om::mat2x3 scale = om::mat2x3::scale(0.2);
	om::mat2x3 move = om::mat2x3::move(position);
	om::mat2x3 m = scale * move;
	engine.render(*he->get_character_vbo(), he->get_character_texture(), m);
}



