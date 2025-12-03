//#pragma once
//#include <gl/glew/glew.h>
//#include <vector>
//
//class SinWave {
//public:
//    SinWave(int numPoints = 512);  // Higher = smoother wave
//    ~SinWave();
//
//    void Init(GLuint shaderProgram);
//    void Render();
//
//private:
//    GLuint VAO, VBO;
//    std::vector<float> vertices;  // x, y (y=0 base; displaced in shader)
//    int numPoints;
//    float frequency;  // For sin(x * freq)
//
//    void GenerateVertices();
//};

#pragma once
#include <gl/glew/glew.h>
#include <vector>

class SinWave {
public:
    SinWave(int numPoints = 512);  // Higher = smoother wave
    ~SinWave();

    void Init(GLuint shaderProgram);
    void Update(float deltaTime);  // NEW: Update animation
    void Render();

private:
    GLuint VAO, VBO;
    GLint timeLoc;  // NEW: Uniform location for time
    std::vector<float> vertices;  // x, y (y=0 base; displaced in shader)
    int numPoints;
    float frequency;  // For sin(x * freq)
    float time;  // NEW: Current animation time

    void GenerateVertices();
};