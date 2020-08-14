#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Cube
{
public:
    Cube(const int x, const int y, const int z);
    ~Cube() = default;

    //Render the block
    void render() const;

    GLuint getVAO() const { return _vao; }
    static size_t getNumObjects() { return _translations.size(); }
    static std::vector<glm::vec3> _translations;

private:
    //OpenGL data
    const GLfloat _blockSize = 1.0f;
    GLuint _vao = 1; //Vertex array object, the main handle for geometry    
    GLuint _vbo; //Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint _ebo;  // BufferID to bind to GL_ELEMENT_ARRAY_BUFFER    
};

