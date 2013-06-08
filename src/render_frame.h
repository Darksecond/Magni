#pragma once

#include "render_command.h"

#include <list>
#include <stdexcept>

static const int QUEUE_COUNT = 8;

class render_queue
{
public:
    inline void add(std::shared_ptr<render_command> command);
private:
    std::list<std::shared_ptr<render_command>> _commands;
};

class render_frame
{
public:
    inline void add(int queue, std::shared_ptr<render_command> command);
private:
    render_queue _queues[QUEUE_COUNT];
};

//INLINE & TEMPLATE METHODS

inline void render_queue::add(std::shared_ptr<render_command> command)
{
    _commands.push_back(command);
}

inline void render_frame::add(int queue, std::shared_ptr<render_command> command)
{
    if(queue >= QUEUE_COUNT || queue < 0)
        throw std::runtime_error("queue does not exist");
    
    _queues[queue].add(command);
}