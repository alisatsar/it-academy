#include <cstdlib>
#include <iostream>
#include <memory>
//#include <string>
#include <fstream>

#include "engine.hxx"

te::vertex blend_vertex(const te::vertex& vl, const te::vertex& vr,
                        const float a)
{
    te::vertex r;
    r.vec.x = (1.0f - a) * vl.vec.x + a * vr.vec.x;
    r.vec.y = (1.0f - a) * vl.vec.y + a * vr.vec.y;
    return r;
}

te::triangle blend(const te::triangle& tl, const te::triangle& tr,
                   const float a)
{
    te::triangle r;
    r.v[0] = blend_vertex(tl.v[0], tr.v[0], a);
    r.v[1] = blend_vertex(tl.v[1], tr.v[1], a);
    r.v[2] = blend_vertex(tl.v[2], tr.v[2], a);
    return r;
}

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

	te::triangle t1;

	t1.v[0].vec.x = -0.5;
	t1.v[0].vec.y = 0.5;
	t1.v[0].tx = 0.0;
	t1.v[0].ty = 1.0;
	t1.v[1].vec.x = 0.5;
	t1.v[1].vec.y = 0.5;
	t1.v[1].tx = 1.0;
	t1.v[1].ty = 1.0;
	t1.v[2].vec.x = 0.5;
	t1.v[2].vec.y = -0.5;
	t1.v[2].tx = 1.0;
	t1.v[2].ty = 0.0;
	//m->scale_triangle(t1, 2);

	t1 = t1 * 2.f;


	te::triangle t2;

	t2.v[0].vec.x  = -0.5;
	t2.v[0].vec.y = 0.5;
	t2.v[0].tx = 0.0;
	t2.v[0].ty = 1.0;
	t2.v[1].vec.x = 0.5;
	t2.v[1].vec.y = -0.5;
	t2.v[1].tx = 1.0;
	t2.v[1].ty = 0.0;
	t2.v[2].vec.x = -0.5;
	t2.v[2].vec.y = -0.5;
	t2.v[2].tx = 0.0;
	t2.v[2].ty = 0.0;

	//t2 = t2 * 2.f;


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
		//std::ifstream file("vert_and_tex_coord.txt");

		m->render_triangle(t1);
		//m->render_triangle(t2);
		//m->clear_color();
		m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
