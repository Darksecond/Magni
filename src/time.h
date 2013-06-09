#pragma once

class time
{
public:
    static time& instance();
    
    time(time&) = delete;
    time(time&&) = delete;
    time& operator=(time&) = delete;
    time& operator=(time&&) = delete;
    
    void step();
    double delta();
    double now();
private:
    double _delta;
    double _current_time;
    time();
};