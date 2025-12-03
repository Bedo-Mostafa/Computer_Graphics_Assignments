#pragma once
#include <gl/glew/glew.h>
#include <vector>

class SierpinskiTriangle {
public:
    SierpinskiTriangle(int depth = 5, float size = 1.0f);
    ~SierpinskiTriangle();

    void Init(GLuint shaderProgram);
    void Render();

private:
    GLuint VAO, VBO;
    int depth;
    float size;
    std::vector<float> vertices;

    void GenerateVertices(float x1, float y1, float x2, float y2, float x3, float y3, int currentDepth);
};