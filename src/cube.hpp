#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Cube
{
public:
    Cube(const int x, const int y, const int z);
    ~Cube()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    };

    //Render the block
    void render() const;

    GLuint getVAO() const { return _vao; }

    static GLfloat getBlockSizeOffset() { return 2.f * Cube::_blockSize; }
    static size_t getNumObjects() { return _translations.size(); }
    static std::vector<glm::mat4> _translations;
private:
    //OpenGL data
    static const GLfloat _blockSize;
    GLuint _vao = 1; //Vertex array object, the main handle for geometry    
    GLuint _vbo; //Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint _ebo;  // BufferID to bind to GL_ELEMENT_ARRAY_BUFFER    
};

