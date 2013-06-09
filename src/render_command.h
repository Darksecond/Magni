#pragma once

class render_frame;
class render_command
{
public:
    virtual ~render_command() = default;
    virtual void execute() = 0;
    
    //TODO replace with pass of render_state into execute, or something similar
    inline void set_frame(render_frame* frame)
    {
        _frame = frame;
    }
protected:
    render_frame* _frame;
};