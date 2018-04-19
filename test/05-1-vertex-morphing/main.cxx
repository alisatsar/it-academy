#include <cstdlib>
#include <iostream>
#include <memory>
#include <cmath>
//#include <string>

#include "engine.hxx"

te::vertex blend_vertex(const te::vertex& vl, const te::vertex& vr,
                        const float a)
{
	te::vertex r;
    r.x = (1.0f - a) * vl.x + a * vr.x;
    r.y = (1.0f - a) * vl.y + a * vr.y;
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

	if(!m->create_window("My window", 0, 0, 300, 300))
	{
		return EXIT_FAILURE;
	}
	m->create_my_shader();

	te::triangle t1q;
	t1q.v[0].x = -1.0;
	t1q.v[0].y = 0.5;
	t1q.v[1].x = 0.5;
	t1q.v[1].y = -0.5;
	t1q.v[2].x = -0.5;
	t1q.v[2].y = -0.5;

	te::triangle t2q;
	t2q.v[0].x = -1.0;
	t2q.v[0].y = 0.5;
	t2q.v[1].x = 1.0;
	t2q.v[1].y = 0.5;
	t2q.v[2].x = 0.5;
	t2q.v[2].y = -0.5;

	te::triangle t1t;
	t1t.v[0].x = -0.0;
	t1t.v[0].y = 0.5;
	t1t.v[1].x = 0.5;
	t1t.v[1].y = -0.5;
	t1t.v[2].x = -0.5;
	t1t.v[2].y = -0.5;

	te::triangle t2t;
	t2t.v[0].x = 0.0;
	t2t.v[0].y = 0.5;
	t2t.v[1].x = 0.0;
	t2t.v[1].y = 0.5;
	t2t.v[2].x = 0.5;
	t2t.v[2].y = -0.5;

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

		float alpha = (std::sin(m->get_time_from_init()) * 0.5f) + 0.5f;

		te::triangle tr1 = blend(t1q, t1t, alpha);
		te::triangle tr2 = blend(t2q, t2t, alpha);

		m->render_triangle(tr1);
		m->render_triangle(tr2);
		m->swap_buffers();
	};

	m->unintialize();

	return EXIT_SUCCESS;
}
