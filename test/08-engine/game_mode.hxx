#pragma once
#include <memory>
#include "engine.hxx"
#include "character_imp.hxx"

struct lila
{
    virtual ~lila();
    virtual void on_initialize()                 			      = 0;
    virtual void on_event(om::event&)                             = 0;
    virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
    virtual void on_render() const                                = 0;
    virtual void on_animate(float count_sprite) = 0;
};

extern std::unique_ptr<lila> om_tat_sat(om::engine&);

lila::~lila() = default;
