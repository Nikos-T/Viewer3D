#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string.h>
//#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.inl>
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
    struct GLintDefault
    {
        GLint val = -2;
    };

    void loadMatrix4f(GLuint loc, glm::mat4 mat4);
    void loadVector3f(GLuint loc, GLfloat x, GLfloat y, GLfloat z);
    void loadVector2f(GLuint loc, GLfloat x, GLfloat y);
    void loadVector4f(GLuint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void loadFloat(GLuint loc, GLfloat x);
    void loadInt(GLuint loc, GLint x);
    void loadBoolean(GLuint loc, bool value);
    GLuint getUniformLocation(const std::string &name);
    void loadMatrix4f(const std::string &name, glm::mat4 mat4);
    void loadFloat(const std::string &name, GLfloat data);
    void loadInt(const std::string &name, GLint data);
    void loadBoolean(const std::string &name, bool value);
    void loadVector3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
    void loadVector2f(const std::string &name, GLfloat x, GLfloat y);
    void loadVector4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

private:
    unsigned int prepareShader(GLenum type, const std::string& shaderSource);
    std::string readFile(const std::string& filePath);
    GLuint programID = 0;
    std::unordered_map<std::string, GLintDefault> uniformLocs;
};

#endif // SHADERPROGRAM_H


