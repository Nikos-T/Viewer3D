#ifndef MESH_H
#define MESH_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <QOpenGLExtraFunctions>
struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

struct ModelTextureDef {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh : protected QOpenGLExtraFunctions
{
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<ModelTextureDef> textures;
    std::string name;
    Mesh();
    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<ModelTextureDef> textures);
    ~Mesh();
    //void Draw(ShaderProgram *shader);
    /*  Render data  */
    GLuint VAO, VBO, EBO;
private:


    void setupMesh();

};

#endif // MESH_H
