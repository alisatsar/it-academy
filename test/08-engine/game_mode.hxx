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
    virtual void on_render() const                                = 0;
    virtual void on_animate() = 0;
    virtual void clear_animate() = 0;
};

extern std::unique_ptr<lila> om_tat_sat(om::engine&);

lila::~lila() = default;

class girl_game final : public lila
{
private:
	om::engine& engine;
	std::vector<om::texture*> all_textures;
	om::sound*   snd        = nullptr;

	character* he;

	pawn* backgr;
	uint32_t state;

	om::vec2 position = om::vec2(-0.7, -0.55);
public:
    explicit girl_game(om::engine& engine)
        : engine(engine)
    {
    }

    void on_initialize() final;
    void on_event(om::event&) final;
    void on_update(std::chrono::milliseconds frame_delta) final;
    void on_render() const final;
    void on_animate() final;
    void clear_animate() final;
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

    he = new hero(tex, 7);

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

	}
	else if (engine.is_key_down(om::keys::right))
	{

	}
	else if (engine.is_key_down(om::keys::up))
	{
		//he->set_first_position(0);
		//he->animate(0, 5, 0.2, engine.get_time_from_init());
	}
	else if (engine.is_key_down(om::keys::down))
	{
		//he->set_first_position(6);
		//he->animate(6, 9, 0.2, engine.get_time_from_init());
	}
}

void girl_game::on_render() const
{
	om::mat2x3 scale1 = om::mat2x3::scale(1);
	engine.render(*backgr->get_pawn_vbo(), backgr->get_pawn_texture(), scale1);
	om::mat2x3 scale = om::mat2x3::scale(0.2);
	om::mat2x3 move = om::mat2x3::move(position);
	om::mat2x3 m = scale * move;
	engine.render(*he->get_character_vbo(), he->get_character_texture(), m);
}

void girl_game::on_animate()
{
	he->animate(6, 9, 0.2, engine.get_time_from_init());
	on_render();
}

void girl_game::clear_animate()
{
	he->animate(0, 5, 0.2, engine.get_time_from_init());
}

