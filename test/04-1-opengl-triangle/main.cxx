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
	m->create_my_shader();

	te::triangle t1;

	t1.v[0].x = -0.5;
	t1.v[0].y = -0.5;
	t1.v[1].x = 0.5;
	t1.v[1].y = -0.5;
	t1.v[2].x = 0;
	t1.v[2].y = 0.5;

	std::cout << sizeof(t1);
	std::cout << sizeof(te::triangle);

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
		m->render_triangle(t1);
		//m->clear_color();
		m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
