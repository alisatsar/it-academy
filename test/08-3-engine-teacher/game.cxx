#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"


class girl final : public lila
{
public:
    explicit girl(engine& eng)
        : tiny_engine(eng)
    {
    }

    void on_initialize() final;
    void on_event(event&) final;
    void on_update(std::chrono::milliseconds frame_delta) final;
    void on_render() const final;

private:
    engine&  tiny_engine;
    texture* tex    = nullptr;
    vbo*     vertex_buf = nullptr;
    sound*   snd        = nullptr;

    vec2               current_tank_pos       = vec2(0.f, 0.f);
    static constexpr float pi                     = 3.1415926f;
};

std::unique_ptr<lila> tat_sat(engine& e)
{
    e.log_ << "creating main game object..." << std::endl;
    auto game = std::make_unique<girl>(e);
    e.log_ << "finish creating main game object" << std::endl;
    return game;
}

void girl::on_initialize()
{
	tex = tiny_engine.create_texture("tank.png");
    if (nullptr == tex)
    {
    	tiny_engine.log_ << "failed load texture\n";
        return;
    }

    vertex_buf = nullptr;

    std::ifstream file("vert_tex_color.txt");
    if (!file)
    {
    	tiny_engine.log_ << "can't load vert_tex_color.txt\n";
        return;
    }
    else
    {
        std::array<tri2, 2> tr;

        vertex_buf = tiny_engine.create_vbo(&tr[0], tr.size());
        if (vertex_buf == nullptr)
        {
        	tiny_engine.log_ << "can't create vertex buffer\n";
            return;
        }
    }

    snd = tiny_engine.create_sound("t2_no_problemo.wav");
}

void girl::on_event(event& event)
{
    std::cout << event << std::endl;
    switch (event.type)
    {
        case event_type::hardware:
            if (std::get<hardware_data>(event.info).is_reset)
            {
            	tiny_engine.exit(EXIT_SUCCESS);
            }
            break;
        case event_type::input_key:
        {
            const auto& key_data = std::get<input_data>(event.info);
            if (key_data.is_down)
            {
                if (key_data.key == keys::button1)
                {
                    snd->play(sound::effect::once);
                }
                else if (key_data.key == keys::button2)
                {
                    snd->play(sound::effect::looped);
                }
            }
        }
        break;
    }
}

lila::~lila(){}

void girl::on_update(std::chrono::milliseconds /*frame_delta*/)
{
    if (tiny_engine.is_key_down(keys::left))
    {
        current_tank_pos.x -= 0.01f;
    }
    else if (tiny_engine.is_key_down(keys::right))
    {
        current_tank_pos.x += 0.01f;
    }
    else if (tiny_engine.is_key_down(keys::up))
    {
        current_tank_pos.y += 0.01f;
    }
    else if (tiny_engine.is_key_down(keys::down))
    {
        current_tank_pos.y -= 0.01f;
    }
}

void girl::on_render() const
{
    matrix move   = matrix::move(current_tank_pos);
    matrix aspect = matrix::scale(1, 640.f / 480.f);
    matrix rot    = matrix::rotation(0.5);
    matrix m      = rot * move * aspect;

    tiny_engine.render(*vertex_buf, tex, m);
}

int initialize_and_start_main_loop()
{
    engine eng("");

    std::unique_ptr<lila> game = tat_sat(eng);

    game->on_initialize();

    while (true)
    {
        event event;

        while (eng.read_event(event))
        {
            game->on_event(event);
        }

        game->on_update(std::chrono::milliseconds(1));
        game->on_render();

        eng.swap_buffers();
    }

    return EXIT_SUCCESS;
}

int main(int /*argc*/, char* /*argv*/ [])
{
    try
    {
        return initialize_and_start_main_loop();
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return EXIT_FAILURE;
}
