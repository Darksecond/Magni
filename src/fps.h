#pragma once

class fps
{
public:
    static fps& instance();
    
    fps(fps&) = delete;
    fps(fps&&) = delete;
    fps& operator=(fps&) = delete;
    fps& operator=(fps&&) = delete;
    
    void set_title(const char* title);
    void update();
private:
    int _frames;
    double _start_time;
    
    const char* _title;
    fps();
};