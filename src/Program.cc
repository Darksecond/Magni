#include "Program.h"

Program::Program(const std::vector<Shader>& shaders)
{
    if(shaders.size() == 0)
        throw std::runtime_error("No Shaders provided");
    
    _object = glCreateProgram();
    if(_object == 0)
        throw std::runtime_error("glCreateProgram failed");
    
    for(auto& shader : shaders)
        glAttachShader(_object, shader.object());
    
    glLinkProgram(_object);
    
    for(auto& shader : shaders)
        glDetachShader(_object, shader.object());
    
    GLint status;
    glGetProgramiv(_object, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(_object); _object = 0;
        throw std::runtime_error(msg);
    }
}

Program::Program(Program&& other) : _object{other._object}
{
    other._object = 0;
}

Program::~Program()
{
    if(_object)
        glDeleteProgram(_object);
}

Program& Program::operator=(Program&& other)
{
    if(_object)
        glDeleteProgram(_object);
    
    _object = other._object;
    other._object = 0;
    return *this;
}

GLuint Program::object() const
{
    return _object;
}

GLint Program::attrib(const GLchar* attribName) const
{
    if(!attribName)
        throw std::runtime_error("attribName was NULL");
    
    GLint attrib = glGetAttribLocation(_object, attribName);
    if(attrib == -1)
        throw std::runtime_error(std::string("Program attribute not found:") + attribName);
    
    return attrib;
}

GLint Program::uniform(const GLchar* uniformName) const
{
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(_object, uniformName);
    if(uniform == -1)
        throw std::runtime_error(std::string("Program uniform not found:") + uniformName);
    
    return uniform;
}

void Program::bind() const
{
    glUseProgram(_object);
}

void Program::unbind() const
{
    glUseProgram(0);
}