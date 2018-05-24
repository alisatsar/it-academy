#pragma once

#include <cassert>
#include <vector>
#include "e_math.hxx"

class vbo
{
public:
    std::vector<om::tri2> triangles;
public:
    vbo(const om::tri2* tri, std::size_t n)
        : triangles(n)
    {
        assert(tri != nullptr);
        for (size_t i = 0; i < n; ++i)
        {
            triangles[i] = tri[i];
        }
    }

    vbo(om::tri2 t1, om::tri2 t2) : triangles(2)
    {
    	triangles[0] = t1;
    	triangles[1] = t2;
    }

    ~vbo(){};

    const om::v2*      data() const  { return &triangles.data()->v[0]; }
    size_t size() const  { return triangles.size() * 3; }
    om::tri2 get_triangle(int index) const
    {
    	return triangles[index];
    }

    void change_x_coord(float x)
    {
    	triangles[0].v[0].pos.x += x;
    	triangles[1].v[1].pos.x += x;
    	triangles[1].v[2].pos.x += x;

    	triangles[0].v[1].pos.x += x;
    	triangles[0].v[2].pos.x += x;
    	triangles[1].v[0].pos.x += x;
    }
};
