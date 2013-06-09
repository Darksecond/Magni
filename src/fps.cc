#include "fps.h"

#include "time.h"

#ifdef __APPLE__
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
#endif // _WIN32
#include <sstream>

fps::fps() : _frames(0), _start_time(0)
{
    set_title("Magni");
}

fps& fps::instance()
{
    static fps _instance;
    return _instance;
}

void fps::set_title(const char* title)
{
    _title = title;
}

void fps::update()
{
    double now = time::instance().now();
    if( (now - _start_time) > 1.0 || _frames == 0 )
    {
        double fps = (double)_frames / (now - _start_time);
        std::stringstream output_text;
        output_text << _title << " | " << fps;
        glfwSetWindowTitle(output_text.str().c_str());
        _start_time = now;
        _frames = 0;
    }
    _frames++;
}