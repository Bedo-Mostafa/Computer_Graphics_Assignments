#include "Mandelbrot.h"
#include <vector>
#include <cmath>

Mandelbrot::Mandelbrot(int maxIterations)
    : maxIterations(maxIterations), centerX(-0.5f), centerY(0.0f), scale(2.5f), animSpeed(0.5f) {
    GenerateQuadVertices();
    GeneratePalette();
}

Mandelbrot::~Mandelbrot() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &paletteTex);
}

void Mandelbrot::GenerateQuadVertices() {
    std::vector<float> vertices = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mandelbrot::GeneratePalette() {
    const int PALETTE_SIZE = 256;
    std::vector<unsigned char> paletteData(PALETTE_SIZE * 3);

    for (int i = 0; i < PALETTE_SIZE; ++i) {
        float t = float(i) / float(PALETTE_SIZE - 1);

        // Create a smooth, vibrant color palette
        // Using multiple sine waves for smooth color transitions
        float r = 0.5f + 0.5f * sinf(3.14159f * (t * 2.0f + 0.0f));
        float g = 0.5f + 0.5f * sinf(3.14159f * (t * 2.0f + 0.33f));
        float b = 0.5f + 0.5f * sinf(3.14159f * (t * 2.0f + 0.67f));

        paletteData[i * 3 + 0] = static_cast<unsigned char>(r * 255);
        paletteData[i * 3 + 1] = static_cast<unsigned char>(g * 255);
        paletteData[i * 3 + 2] = static_cast<unsigned char>(b * 255);
    }

    glGenTextures(1, &paletteTex);
    glBindTexture(GL_TEXTURE_1D, paletteTex);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, PALETTE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, paletteData.data());

    // Use GL_LINEAR for smooth color interpolation between palette entries
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Allow wrapping for cycling colors

    glBindTexture(GL_TEXTURE_1D, 0);
}

void Mandelbrot::Init(GLuint shaderProgram) {
    centerLoc = glGetUniformLocation(shaderProgram, "center");
    scaleLoc = glGetUniformLocation(shaderProgram, "scale");
    iterLoc = glGetUniformLocation(shaderProgram, "iter");
    timeLoc = glGetUniformLocation(shaderProgram, "time");
}

void Mandelbrot::Render(float currentTime) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, paletteTex);

    // Gentle animation (comment out for static view)
    float animY = 0.05f * sinf(currentTime * animSpeed * 0.3f);
    float animScale = scale * (1.0f + 0.03f * sinf(currentTime * animSpeed * 0.5f));

    glUniform2f(centerLoc, centerX, centerY + animY);
    glUniform1f(scaleLoc, animScale);
    glUniform1f(timeLoc, currentTime);
    glUniform1i(iterLoc, maxIterations);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_1D, 0);
}

void Mandelbrot::HandleMouseWheel(float delta) {
    float zoomFactor = (delta > 0) ? 0.9f : 1.1f;
    scale *= zoomFactor;
}

void Mandelbrot::HandleMouseDrag(float dx, float dy) {
    centerX -= dx * scale * 0.001f;
    centerY += dy * scale * 0.001f;  // Inverted Y for natural dragging
}