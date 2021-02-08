#include "shaderprogram.h"
#include <iostream>
#include <fstream>

GLuint ShaderProgram::currentProg = 0;

ShaderProgram::ShaderProgram(){

}

void ShaderProgram::init(std::string shaderName)
{
    if (programID != 0) std::cout << "ShaderProgram already initialized!" << std::endl;
    initializeOpenGLFunctions();
    unsigned int vertexShader = prepareShader(GL_VERTEX_SHADER, "shaders/" +shaderName+ ".vert");
    unsigned int fragmentShader = prepareShader(GL_FRAGMENT_SHADER, "shaders/" + shaderName +".frag");

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    //If needed -> setting attributes always before linking, example:
    //glBindAttribLocation(programID, 1, "position");

    glLinkProgram(programID);
    glValidateProgram(programID);

    // Print linking errors if any
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        printf ("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    else{
        std::cout << "Loaded shader ->" << shaderName << std::endl;
    }
    //cleanup
    glDetachShader(programID, vertexShader);
    glDetachShader(programID, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint ShaderProgram::prepareShader(GLenum type, const std::string& shaderPath){
    std::string shaderStr = readFile(shaderPath);
    const char *shaderSrc = shaderStr.c_str();

    unsigned int shader = glCreateShader(type);
    //attach the source and compile
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n");
    }
    return shader;
}

void ShaderProgram::start(){
    if (ShaderProgram::currentProg != programID){
        glUseProgram(programID);
        ShaderProgram::currentProg = programID;
    }

}

void ShaderProgram::stop(){
    glUseProgram(0);
    ShaderProgram::currentProg = 0;
}

std::string ShaderProgram::readFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
        //std::cout << line << std::endl;
    }

    fileStream.close();
    return content;
}

ShaderProgram::~ShaderProgram()
{
    stop();
    glDeleteProgram(programID);
}

