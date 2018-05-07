#pragma once

#include <cstdint>
#include <iostream>

#ifndef OM_DECLSPEC
#define OM_DECLSPEC
#endif

namespace om
{
class OM_DECLSPEC color
{
public:
    color() = default;
    explicit color(std::uint32_t rgba_);
    color(float r, float g, float b, float a);

    float get_r() const;
    float get_g() const;
    float get_b() const;
    float get_a() const;

    void set_r(const float r);
    void set_g(const float g);
    void set_b(const float b);
    void set_a(const float a);

private:
    std::uint32_t rgba = 0;
};

OM_DECLSPEC std::istream& operator>>(std::istream& is, color&);
}
