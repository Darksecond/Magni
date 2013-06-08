#pragma once

class render_command
{
public:
    virtual ~render_command() = 0;
    virtual void execute() = 0;
};