#pragma once

#include <memory>

class text;
enum class event_t
{
    renderer_add_text,
    renderer_remove_text,
    behaviour_add_key,
    behaviour_has_key,
};

struct text_event
{
    std::shared_ptr<text> _text;
};

struct key_event
{
    enum key_t
    {
        red = 0,
        blue,
    };
    key_t key;
    bool has_key;
};