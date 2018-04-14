#include <cstdlib>
#include <iostream>
#include <memory>
//#include <string>

#include "engine.hxx"

int main()
{
	std::unique_ptr<te::engine, void(*)(te::engine*)> m(te::create_engine(), te::destroy_engine);

	std::cout << m->check_version();

	if(!m->initialize())
	{
		return EXIT_FAILURE;
	}

	if(!m->create_window("My window", 0, 0, 300, 300))
	{
		return EXIT_FAILURE;
	}

	bool continue_loop = true;

	while(continue_loop)
	{
		te::event event;

		while(m->read_input(event))
		{
			switch(event)
			{
			case te::event::up_pressed:
				std::cout << "up" << std::endl;
				break;
			case te::event::turn_off:
				continue_loop = false;
				break;
			}
		}
	};

	te::triangle tr{};
	m->render_triagnle(tr);

	m->unintialize();

	return EXIT_SUCCESS;
}
