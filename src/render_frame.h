#pragma once

#include "render_command.h"
#include "Program.h"

#include <list>
#include <stdexcept>
#include <memory>
#include <iostream>

static const int QUEUE_COUNT = 32;

class render_queue
{
public:
    render_queue() : run(true) {}
    
    inline void add(std::shared_ptr<render_command> command);
    inline void execute();
    
    bool run;
private:
    std::list<std::shared_ptr<render_command>> _commands;
};

class render_frame
{
public:
    template <int Q, typename T, typename... Args>
    inline void add(Args... args);
    inline void add(int queue, std::shared_ptr<render_command> command);
    inline void execute();
    inline void execute_queue(int queue);
    
    inline void set_current_program(std::shared_ptr<Ymir::Program> p) //TODO move to render_state
    {
        _current_program = p;
    }
    
    inline std::shared_ptr<Ymir::Program> current_program() //TODO move to render_state
    {
        return _current_program;
    }
    
    inline void ignore(int queue)
    {
        _queues[queue].run = false;
    }
    
private:
    render_queue _queues[QUEUE_COUNT];
    std::shared_ptr<Ymir::Program> _current_program; //TODO move to some kind of render_state class and pass into execute chain
};

//INLINE & TEMPLATE METHODS


template <int Q, typename T, typename... Args>
inline void render_frame::add(Args... args)
{
    add(Q, std::make_shared<T>(args...));
}

inline void render_queue::add(std::shared_ptr<render_command> command)
{
    _commands.push_back(command);
}

inline void render_frame::add(int queue, std::shared_ptr<render_command> command)
{
    if(queue >= QUEUE_COUNT || queue < 0)
        throw std::runtime_error("queue does not exist");
    
    command->set_frame(this);
    
    _queues[queue].add(command);
}

inline void render_queue::execute()
{
    for(auto command : _commands)
        command->execute();
}

inline void render_frame::execute()
{
    for(int i = 0; i < QUEUE_COUNT; ++i)
    {
        if(_queues[i].run)
            execute_queue(i);
    }
}

inline void render_frame::execute_queue(int queue)
{
    _queues[queue].execute();
}
