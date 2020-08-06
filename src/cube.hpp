#pragma once

#include <glad/glad.h>

class Cube
{
public:
    Cube(const int x, const int y, const int z);
    ~Cube() = default;

    //Render the block
    void render() const;

private:
    //OpenGL data
    GLuint vao = 1; // Vertex array object, the main handle for geometry
    int nverts; // Number of vertices in the vertex array
    int ntris;  // Number of triangles in the index array (may be zero)
    GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
};