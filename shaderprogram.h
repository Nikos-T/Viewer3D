#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string.h>
//#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>


class ShaderProgram : protected QOpenGLExtraFunctions
{
public:
    static GLuint currentProg;
    ShaderProgram();
    void init(std::string shaderName);
    ~ShaderProgram();
    void start();
    void stop();
    GLuint getID();

private:
    unsigned int prepareShader(GLenum type, const std::string& shaderSource);
    std::string readFile(const std::string& filePath);
    GLuint programID = 0;
};

#endif // SHADERPROGRAM_H


