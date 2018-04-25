#include <cstdlib>
#include <iostream>
#include <memory>

#include <fstream>

#include "engine.hxx"

int main()
{
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
	m->create_my_shader();

	math::triangle t1;

	t1.vert[0].vec.x = -0.2;
	t1.vert[0].vec.y = -0.2;
	t1.vert[0].text_vec.x = 0.0;
	t1.vert[0].text_vec.y = 0.0;
	t1.vert[1].vec.x = 0.2;
	t1.vert[1].vec.y = -0.2;
	t1.vert[1].text_vec.x = 0.2;
	t1.vert[1].text_vec.y = 0.0;
	t1.vert[2].vec.x = 0.2;
	t1.vert[2].vec.y = 0.2;
	t1.vert[2].text_vec.x = 0.2;
	t1.vert[2].text_vec.y = 0.2;

	math::triangle t2;

	t2.vert[0].vec.x = 0.2;
	t2.vert[0].vec.y = 0.2;
	t2.vert[0].text_vec.x = 0.2;
	t2.vert[0].text_vec.y = 0.2;
	t2.vert[1].vec.x = -0.2;
	t2.vert[1].vec.y = 0.2;
	t2.vert[1].text_vec.x = 0.0;
	t2.vert[1].text_vec.y = 0.2;
	t2.vert[2].vec.x = -0.2;
	t2.vert[2].vec.y = -0.2;
	t2.vert[2].text_vec.x = 0.0;
	t2.vert[2].text_vec.y = 0.0;

	bool continue_loop = true;

	float a = 0.f;
	float b = 0.f;

	float left = 0.0;
	float right = 0.2;

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

		if((a - b) >= 0.2f)
		{
			if(left < 0.8)
			{
				left += 0.2;
			}
			else
			{
				left = 0.0;
			}
			if(right < 1.f)
			{
				right += 0.2;
			}
			else
			{
				right = 0.2;
			}

			t1.vert[0].text_vec.x = left;
			t2.vert[1].text_vec.x = left;
			t2.vert[2].text_vec.x = left;

			t1.vert[1].text_vec.x = right;
			t1.vert[2].text_vec.x = right;
			t2.vert[0].text_vec.x = right;

			b = a;
		}

		m->render_triangle(t1);
		m->render_triangle(t2);
		//m->render_triangle(t3);
		//m->render_triangle(t4);

		m->swap_buffers();

		a = m->get_time_from_init();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
