#pragma once

#include <chrono>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "e_math.hxx"
#include "color.hxx"
#include "vertex_buffer.hxx"

void delay(int sec);

#ifndef OM_DECLSPEC
#define OM_DECLSPEC
#endif

struct background
{
	std::string name;
	om::vec2 position;
};

std::istream& operator>>(std::istream& is, background&);

namespace om
{

/// dendy gamepad emulation events
enum class event_type
{
    input_key,
    hardware
};

enum class keys
{
    left,
    right,
    up,
    down,
    select,
    start,
    button1,
    button2
};

struct input_data
{
    enum keys key;
    bool      is_down;
};

struct hardware_data
{
    bool is_reset;
};

struct event
{
    std::variant<input_data, hardware_data> info;
    double                                  timestamp;
    event_type                              type;
};

class engine;

/// return not null on success
OM_DECLSPEC engine* create_engine();
OM_DECLSPEC void    destroy_engine(engine* e);


OM_DECLSPEC std::ostream& operator<<(std::ostream& stream, const input_data&);
OM_DECLSPEC std::ostream& operator<<(std::ostream& stream,
                                     const hardware_data&);
//OM_DECLSPEC std::ostream& operator<<(std::ostream& stream, const event& e);




class OM_DECLSPEC texture
{
public:
    virtual ~texture();
    virtual std::uint32_t get_width() const  = 0;
    virtual std::uint32_t get_height() const = 0;
};

class OM_DECLSPEC sound
{
public:
    enum class effect
    {
        once,
        looped
    };

    virtual ~sound();
    virtual void play(const effect) = 0;
    virtual bool is_playing() const = 0;
    virtual void stop()             = 0;
};

struct window_size
{
	uint32_t window_width = 0;
	uint32_t window_height = 0;
	window_size() = default;
	window_size(uint32_t w, uint32_t h) : window_width(w), window_height(h){}
	window_size(window_size const& w)
	{
		window_width = w.window_width;
		window_height = w.window_height;
	}
};

window_size win_size;

class OM_DECLSPEC engine
{
public:
    explicit engine(std::string_view, om::window_size window_size);
    ~engine();
    engine& operator      =(engine&& other); // move assignment
    engine(const engine&) = delete;

    /// return seconds from initialization
    float get_time_from_init();
    /// pool event from input queue
    bool read_event(event& e);
    bool is_key_down(const enum keys);

    texture* create_texture(std::string_view path);
    void     destroy_texture(texture* t);

    vbo* create_vbo(const tri2*, std::size_t);
    void destroy_vbo(vbo*);

    sound* create_sound(std::string_view path);
    void   destroy_sound(sound*);

    void render(const tri0&, const color&);
    void render(const tri1&);
    void render(const tri2&, texture*);
    void render(const tri2&, texture*, const mat2x3& m);
    void render(const vbo&, texture*, const mat2x3&);
    void swap_buffers();
    void uninitialize();

    void exit(int return_code);

    std::ostream& log;
};

} // end namespace om


