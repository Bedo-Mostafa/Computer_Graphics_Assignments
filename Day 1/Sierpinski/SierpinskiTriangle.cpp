#include "SierpinskiTriangle.h"
#include <cmath>

SierpinskiTriangle::SierpinskiTriangle(int depth, float size)
    : depth(depth), size(size) {
    // Start with base equilateral triangle (point up, centered at origin)
    float halfSize = size / 2.0f;
    float height = (sqrtf(3.0f) / 2.0f) * size;
    GenerateVertices(-halfSize, -height / 2.0f,  // Bottom-left
        halfSize, -height / 2.0f,   // Bottom-right
        0.0f, height / 2.0f,        // Top
        depth);
}

SierpinskiTriangle::~SierpinskiTriangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void SierpinskiTriangle::GenerateVertices(float x1, float y1, float x2, float y2, float x3, float y3, int currentDepth) {
    if (currentDepth == 0) {
        // Base case: Add the three vertices for this triangle (repeated for GL_TRIANGLES)
        vertices.insert(vertices.end(), { x1, y1, x2, y2, x3, y3 });
        return;
    }

    // Compute midpoints
    float mx1 = (x1 + x2) / 2.0f;
    float my1 = (y1 + y2) / 2.0f;
    float mx2 = (x2 + x3) / 2.0f;
    float my2 = (y2 + y3) / 2.0f;
    float mx3 = (x3 + x1) / 2.0f;
    float my3 = (y3 + y1) / 2.0f;

    // Recurse on three sub-triangles (remove the center one)
    GenerateVertices(x1, y1, mx1, my1, mx3, my3, currentDepth - 1);
    GenerateVertices(mx1, my1, x2, y2, mx2, my2, currentDepth - 1);
    GenerateVertices(mx3, my3, mx2, my2, x3, y3, currentDepth - 1);
}

void SierpinskiTriangle::Init(GLuint shaderProgram) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SierpinskiTriangle::Render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);  // Each triangle: 3 verts * 2 floats
    glBindVertexArray(0);
}