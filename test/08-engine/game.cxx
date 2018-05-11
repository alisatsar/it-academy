#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>

#include "game_mode.hxx"

int initialize_and_start_main_loop()
{
	om::window_size window_size(640, 480);

	om::engine engine("", window_size);

    std::unique_ptr<lila> game = om_tat_sat(engine);

    game->on_initialize();

    while (true)
    {
        om::event event;

        while (engine.read_event(event))
        {
            game->on_event(event);
        }

        game->on_update(std::chrono::milliseconds(1));
        game->on_animate();

        engine.swap_buffers();
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
