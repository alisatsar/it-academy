#pragma once

#include <cassert>
#include <vector>
#include "e_math.hxx"

class vbo
{
public:
    virtual ~vbo();
    virtual const om::v2* data() const = 0;
    /// count of vertexes
    virtual size_t size() const = 0;
    virtual om::tri2 get_triangle(int index) const = 0;
};

vbo::~vbo() {}

class vertex_buffer_impl final : public vbo
{
public:
    vertex_buffer_impl(const om::tri2* tri, std::size_t n)
        : triangles(n)
    {
        assert(tri != nullptr);
        for (size_t i = 0; i < n; ++i)
        {
            triangles[i] = tri[i];
        }
    }
    ~vertex_buffer_impl() final;

    const om::v2*      data() const final { return &triangles.data()->v[0]; }
    size_t size() const final { return triangles.size() * 3; }
    om::tri2 get_triangle(int index) const
    {
    	return triangles[index];
    }

private:
    std::vector<om::tri2> triangles;
};
