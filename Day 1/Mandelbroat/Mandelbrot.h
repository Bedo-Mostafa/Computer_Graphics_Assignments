#pragma once
#include <gl/glew/glew.h>

class Mandelbrot {
public:
    Mandelbrot(int maxIterations = 100);
    ~Mandelbrot();

    void Init(GLuint shaderProgram);
    void Render(float currentTime);  // Now takes time for animation

    // Interactivity
    void HandleMouseWheel(float delta);
    void HandleMouseDrag(float dx, float dy);

private:
    GLuint VAO, VBO, paletteTex;
    GLint centerLoc, scaleLoc, iterLoc, timeLoc;
    int maxIterations;
    float centerX, centerY, scale;
    float animSpeed;  // For time-based animation

    void GenerateQuadVertices();
    void GeneratePalette();
};