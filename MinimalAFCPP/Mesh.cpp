#include "Mesh.h"
#include "helpers.h"

using namespace af;


void CreateVertexAttribPointer(GLuint index, int size, GLenum type, int offsetInArray) {
    glVertexAttribPointer(index, size, type, false, sizeof(Vertex), (const void*) offsetInArray);
    glEnableVertexAttribArray(index);
}

Mesh::Mesh(Vertex* data, uint numVerts, uint* indices, uint numIndices, bool stream) 
    : vertices(data), vertexCount(numVerts), vertexDrawCount(numVerts), 
    indices(indices), indexCount(numIndices), indexDrawCount(numIndices)
{
    int bufferUsage = stream ? GL_STREAM_DRAW : GL_STATIC_DRAW;

    // make vertex array object, specify vertex attributes
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // make vertex buffer object, give it our vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, bufferUsage);

    GLuint currentOffset = 0;
    CreateVertexAttribPointer(0, 3, GL_FLOAT, currentOffset);
    currentOffset += 3 * sizeof(float);
    CreateVertexAttribPointer(1, 2, GL_FLOAT, currentOffset);
    currentOffset += 2 * sizeof(float);

    // make element buffer object, give it our indices aka elements
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, bufferUsage);

    printArray(indices, "indices", 3);

    glBindVertexArray(0);
}

void Mesh::updateBuffers(uint newVertexCount, uint newIndexCount) {
    if (newIndexCount > indexCount|| newVertexCount > vertexCount) {
        err("The mesh buffer does not have this many vertices. You may only specify new index and vertex counts that are less than the amount initially allocated");
    }

    vertexDrawCount = newVertexCount;
    indexDrawCount = newIndexCount;

    glBindVertexArray(vao);

    // Update vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDrawCount * sizeof(Vertex), vertices);

    // Update index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexDrawCount * sizeof(uint), indices);
}

void Mesh::updateBuffers() {
    updateBuffers((uint)vertexCount, (uint)indexCount);
}


void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexDrawCount, GL_UNSIGNED_INT, NULL);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    delete vertices;
    delete indices;

    print("Mesh destructed");
}