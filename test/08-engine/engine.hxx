#include <chrono>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "e_math.hxx"
#include "color.hxx"

#ifndef OM_DECLSPEC
#define OM_DECLSPEC
#endif

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

class OM_DECLSPEC vbo
{
public:
    virtual ~vbo();
    virtual const v2* data() const = 0;
    /// count of vertexes
    virtual size_t size() const = 0;
    virtual tri2 get_triangle(int index) const = 0;
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

class OM_DECLSPEC engine
{
public:
    explicit engine(std::string_view config);
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
    void start_animate(const vbo& buff, texture* tex, float count_sprite, float sec, const mat2x3&);
    void swap_buffers();
    void uninitialize();

    void exit(int return_code);

    std::ostream& log;
};


///struct location for screen
///x = 0, y = 0 top left angle screen
struct location
{
	uint32_t x;
	uint32_t y;
	location();
	location(uint32_t x_, uint32_t y_);
	vec2 get_sdl2_coordinate();
};

struct OM_DECLSPEC lila
{
    virtual ~lila();
    virtual void on_initialize()                                  = 0;
    virtual void on_event(om::event&)                             = 0;
    virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
    virtual void on_render() const                                = 0;
    virtual void on_animate(float count_sprite) = 0;
};

} // end namespace om

extern std::unique_ptr<om::lila> om_tat_sat(om::engine&);
