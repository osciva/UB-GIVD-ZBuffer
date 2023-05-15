#pragma once

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

#include <memory>
#include <QGLShaderProgram>
#include <QGLShader>

using namespace std;

class GLShader
{
public:
    typedef enum SHADER_TYPES {
        DEFAULT,
        COLOR
    } SHADER_TYPES;

    typedef enum SHADER_INDEX {
        DEFAULT_SHADER,
        COLOR_SHADER
    } SHADER_INDEX;

    GLShader(const char* vertexShaderFile, const char* fragmentShaderFile);
    void activateShader(shared_ptr<QGLShaderProgram> &program);
    void activateShaderTexture();
    shared_ptr<QGLShaderProgram> getProgram() const {return program;}

private:
    shared_ptr<QGLShaderProgram> program;
};
