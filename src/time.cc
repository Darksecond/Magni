#include "time.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32

time::time() : _delta(0), _current_time(0)
{
    step();
}

class time& time::instance()
{
    static time _instance;
    return _instance;
}

double time::delta()
{
    return _delta;
}

void time::step()
{
    float now = glfwGetTime();
    _delta = now - _current_time;
    if(_delta > 0.05) //limit maximum delta
        _delta = 0.05;
    _current_time = now;
}

double time::now()
{
    return _current_time;
}