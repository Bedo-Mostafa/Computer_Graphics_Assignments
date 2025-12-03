//#pragma once
//#include <gl/glew/glew.h>
//#include <vector>
//#include <cmath>
//
//class Circle {
//public:
//    Circle(int numSegments = 128, float radius = 1.0f);
//    ~Circle();
//
//    void Init(GLuint shaderProgram);
//    void Update(float deltaTime);
//    void Render();
//
//private:
//    GLuint VAO, VBO;
//    GLint transformLoc;
//    float rotationAngle;
//    int numSegments;
//    float radius;
//    std::vector<float> vertices;
//
//    void GenerateVertices();
//};

#pragma once
#include <gl/glew/glew.h>
#include <vector>
#include <cmath>

class Circle {
public:
    Circle(int numSegments = 128, float radius = 1.0f);
    ~Circle();

    void Init(GLuint shaderProgram);
    void Update(float deltaTime);
    void Render();

private:
    GLuint VAO, VBO;
    GLint rotationAngleLoc;  // Changed: now stores location for rotation angle uniform
    float rotationAngle;
    int numSegments;
    float radius;
    std::vector<float> vertices;

    void GenerateVertices();
};