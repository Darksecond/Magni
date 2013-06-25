#pragma once

#include <memory>

class text;
enum class event_t
{
    renderer_add_text,
    renderer_remove_text,
};

struct text_event
{
    std::shared_ptr<text> _text;
};