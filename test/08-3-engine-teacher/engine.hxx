#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>

#include "color.hxx"
#include "e_math.hxx"

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
	bool is_down;
};

struct hardware_data
{
	bool is_reset;
};

struct event
{
	std::variant<input_data, hardware_data> info;
	double timestamp;
	event_type type;
};

std::ostream& operator<<(std::ostream& stream, const input_data& data);
std::ostream& operator<<(std::ostream& stream, const hardware_data& data);
std::ostream& operator<<(std::ostream& stream, const event& e);

class texture
{
public:
	virtual ~texture();
	virtual std::uint32_t get_width() const = 0;
	virtual std::uint32_t get_height() const = 0;
};

class vbo
{
public:
	virtual ~vbo();
	virtual const v2* data() const = 0;
	virtual size_t size() const = 0;
};

class sound
{
public:
	enum class effect
	{
		once,
		looped
	};

	virtual ~sound();
	virtual void play(const effect ef) = 0;
	virtual bool is_playing() const = 0;
	virtual void stop() = 0;
};

class engine;

enum class primitives
{
    lines,
    line_strip,
    line_loop,
    triangls,
    trianglestrip,
    trianglfan
};

engine* create_engine();
void destroy_engine(engine* e);

class engine
{
public:
	explicit engine(std::string_view config);
	~engine();
	//engine& operator=(engine&& other);
	//engine(const engine&) = delete;

	float get_time_from_init();

	bool read_event(event& e);
	bool is_key_down(const enum keys);

	texture* create_texture(std::string_view path);
	void     destroy_texture(texture* t);

	vbo* create_vbo(const tri2*, std::size_t);
	void destroy_vbo(vbo*);

	sound* create_sound(std::string_view path);
	void   destroy_sound(sound*);

//	void render(const primitives primitiv, const vbo& vbo_,
//			const texture* tex, const matrix& m);

	void render(const tri0&, const color&);
	void render(const tri1&);
	void render(const tri2&, texture*);
	void render(const tri2&, texture*, const matrix& m);
	void render(const vbo&, texture*, const matrix&);

	void swap_buffers();
	void exit(int return_code);

	std::ostream& log_;
};

//void initialize(std::string_view title, const window_mode& desired_window_mode);

struct lila
{
	virtual ~lila();
	virtual void on_initialize() = 0;
	virtual void on_event(event& e) = 0;
	virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
	virtual void on_render() const = 0;
};

extern std::unique_ptr<lila> tat_sat(engine&);
