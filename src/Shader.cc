#include "Shader.h"

#include <fstream>
#include <sstream>

Shader Shader::shaderFromFile(const std::string& file, GLenum shaderType)
{
    std::ifstream f;
    f.open(file.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open())
        throw std::runtime_error(std::string("Failed to open file:") + file);
    
    std::stringstream buff;
    buff << f.rdbuf();
    
    return Shader(buff.str(), shaderType);
}

Shader::Shader(const std::string& code, GLenum shaderType)
{
    _object = glCreateShader(shaderType);
    if(_object == 0)
        throw std::runtime_error("glCreateShader failed");
    
    const char* c_code = code.c_str();
    const GLchar** gl_code = static_cast<const GLchar**>(&c_code);
    glShaderSource(_object, 1, gl_code, NULL);
    
    glCompileShader(_object);
    
    //check for compile errors
    GLint status;
    glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(_object);
        _object = 0;
        throw std::runtime_error(msg);
    }
}

Shader::Shader(Shader&& other) : _object{other._object}
{
    other._object = 0;
}

Shader::~Shader()
{
    glDeleteShader(_object);
    _object = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    if(_object)
        glDeleteShader(_object);
    
    _object = other._object;
    other._object = 0;
    return *this;
}

GLuint Shader::object() const
{
    return _object;
}