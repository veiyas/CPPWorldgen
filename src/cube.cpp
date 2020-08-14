#include "cube.hpp"

std::vector<glm::mat4> Cube::_translations;
const GLfloat Cube::_blockSize = 1.0f;

Cube::Cube(const int x, const int y, const int z)
{    
    // The data array contains 8 floats per vertex:
    // coordinate xyz, normal xyz, texcoords st
    GLfloat vertex_array_data[] = {
        //Back face
       -_blockSize, -_blockSize, -_blockSize,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f / 3.0f,  // Vertex 0
        _blockSize, -_blockSize, -_blockSize,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f / 3.0f,  // Vertex 1
       -_blockSize,  _blockSize, -_blockSize,   0.0f, 0.0f,-1.0f,   0.0f, 2.0f / 3.0f,  // Vertex 2
        _blockSize,  _blockSize, -_blockSize,   0.0f, 0.0f,-1.0f,   1.0f, 2.0f / 3.0f,  // Vertex 3
         //Front face
       -_blockSize, -_blockSize,  _blockSize,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f / 3.0f,  // Vertex 4
        _blockSize, -_blockSize,  _blockSize,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f / 3.0f,  // Vertex 5
       -_blockSize,  _blockSize,  _blockSize,   0.0f, 0.0f, 1.0f,   0.0f, 2.0f / 3.0f,  // Vertex 6
        _blockSize,  _blockSize,  _blockSize,   0.0f, 0.0f, 1.0f,   1.0f, 2.0f / 3.0f,  // Vertex 7
         //Right face
        _blockSize, -_blockSize,  _blockSize,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f / 3.0f,  // Vertex 5 - 8
        _blockSize, -_blockSize, -_blockSize,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f / 3.0f,  // Vertex 1 - 9
        _blockSize,  _blockSize,  _blockSize,   1.0f, 0.0f, 0.0f,   0.0f, 2.0f / 3.0f,  // Vertex 7 - 10
        _blockSize,  _blockSize, -_blockSize,   1.0f, 0.0f, 0.0f,   1.0f, 2.0f / 3.0f,  // Vertex 3 - 11
         //Left face
       -_blockSize, -_blockSize, -_blockSize,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f / 3.0f,  // Vertex 0 - 12
       -_blockSize, -_blockSize,  _blockSize,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f / 3.0f,  // Vertex 4 - 13
       -_blockSize,  _blockSize, -_blockSize,  -1.0f, 0.0f, 0.0f,   0.0f, 2.0f / 3.0f,  // Vertex 2 - 14
       -_blockSize,  _blockSize,  _blockSize,  -1.0f, 0.0f, 0.0f,   1.0f, 2.0f / 3.0f,  // Vertex 6 - 15
         //Top face
       -_blockSize,  _blockSize,  _blockSize,   0.0f, 1.0f, 0.0f,   0.0f,  2.0f / 3.0f, // Vertex 6 - 16
        _blockSize,  _blockSize,  _blockSize,   0.0f, 1.0f, 0.0f,   1.0f,  2.0f / 3.0f, // Vertex 7 - 17
       -_blockSize,  _blockSize, -_blockSize,   0.0f, 1.0f, 0.0f,   0.0f,  1.0f,      // Vertex 2 - 18
        _blockSize,  _blockSize, -_blockSize,   0.0f, 1.0f, 0.0f,   1.0f,  1.0f,      // Vertex 3 - 19
         //Bottom face
       -_blockSize, -_blockSize, -_blockSize,   0.0f,-1.0f, 0.0f,   0.0f,  0.0f,      // Vertex 0 - 20
        _blockSize, -_blockSize, -_blockSize,   0.0f,-1.0f, 0.0f,   1.0f,  0.0f,      // Vertex 1 - 21
       -_blockSize, -_blockSize,  _blockSize,   0.0f,-1.0f, 0.0f,   0.0f,  1.0f / 3.0f, // Vertex 4 - 22
        _blockSize, -_blockSize,  _blockSize,   0.0f,-1.0f, 0.0f,   1.0f,  1.0f / 3.0f, // Vertex 5 - 23
    };
    const GLuint index_array_data[] = {
        //Back face
        0,2,1,
        1,2,3,
        //Front face
        4,5,6,
        5,7,6,
        //Right face
        8,9,10,
        9,11,10,
        //Left face
        12,15,14,
        13,15,12,
        //Top face
        16,19,18,
        17,19,16,
        //Bottom face
        20,21,23,
        20,23,22
    };

    int nverts = 24; // Number of vertices in the vertex array
    int ntris = 12;  // Number of triangles in the index array (may be zero)
    size_t stride = 8;

    glm::mat4 tempTransMat{ 1.f };
    tempTransMat = glm::translate(tempTransMat, glm::vec3(x, y, z));
    Cube::_translations.push_back(tempTransMat);

    // Generate one vertex array object (VAO) and bind it
    glGenVertexArrays(1, &(_vao));
    glBindVertexArray(_vao);

    // Generate two buffer IDs
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Activate the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // Present our vertex coordinates to OpenGL
    glBufferData(GL_ARRAY_BUFFER, 8 * nverts * sizeof(GLfloat), vertex_array_data, GL_STATIC_DRAW);
    // Specify how many attribute arrays we have in our VAO
    glEnableVertexAttribArray(0); // Vertex coordinates
    glEnableVertexAttribArray(1); // Normals
    glEnableVertexAttribArray(2); // Texcoords
    // Specify how OpenGL should interpret the vertex buffer data:
    // Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
    // Number of dimensions (3 means vec3 in the shader, 2 means vec2)
    // Type GL_FLOAT
    // Not normalized (GL_FALSE)
    // Stride 11 floats (interleaved array with 8 floats per vertex)
    // Array buffer offset 0, 3 or 6 floats (offset into first vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), nullptr); // xyz coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords
    
    // Activate the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * ntris * sizeof(GLuint), index_array_data, GL_STATIC_DRAW);



    // Deactivate (unbind) the VAO and the buffers again.
    // Do NOT unbind the index buffer while the VAO is still bound.
    // The index buffer is an essential part of the VAO state.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::render() const
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindBuffer(0, 0);
}
