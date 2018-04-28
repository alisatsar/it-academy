
#include <cstdlib>
#include <iostream>
#include <memory>

#include <fstream>

#include "engine.hxx"

int main()
{
	using namespace math;
	std::unique_ptr<te::engine, void(*)(te::engine*)> m(te::create_engine(), te::destroy_engine);

	std::cout << m->check_version();

	if(!m->initialize())
	{
		return EXIT_FAILURE;
	}

	if(!m->create_window("My window", 0, 0, 600, 600))
	{
		return EXIT_FAILURE;
	}

	my_texture* tex = m->load_texture("boy-320.png");
	m->create_my_shader(tex);

	math::triangle t1 = m->create_tex_triangles(0.4, 5, true);

	math::triangle t2 = m->create_tex_triangles(0.4, 5, false);

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
		m->render_triangle(t2);

		m->start_animate(t1, t2, 0.2);

		m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
