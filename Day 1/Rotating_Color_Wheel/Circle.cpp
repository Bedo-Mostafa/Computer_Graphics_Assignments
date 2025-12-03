#include "Circle.h"
#include <iostream>

Circle::Circle(int numSegments, float radius)
    : numSegments(numSegments), radius(radius), rotationAngle(0.0f) {
    GenerateVertices();
}

Circle::~Circle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Circle::GenerateVertices() {
    vertices.clear();
    vertices.push_back(0.0f);  // Center
    vertices.push_back(0.0f);

    const float PI = acosf(-1.0f);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * PI * float(i) / float(numSegments);
        vertices.push_back(cosf(angle) * radius);
        vertices.push_back(sinf(angle) * radius);
    }
}

void Circle::Init(GLuint shaderProgram) {
    // Get uniform location for rotationAngle instead of transform matrix
    rotationAngleLoc = glGetUniformLocation(shaderProgram, "rotationAngle");

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

void Circle::Update(float deltaTime) {
    // Update rotation angle (adjust speed as needed)
    rotationAngle += 1.0f * deltaTime;  // 1.0 radian per second

    const float TWO_PI = 2.0f * acosf(-1.0f);
    if (rotationAngle > TWO_PI) {
        rotationAngle -= TWO_PI;  // Wrap around
    }
}

void Circle::Render() {
    // Send single rotation angle to shader
    glUniform1f(rotationAngleLoc, rotationAngle);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);
    glBindVertexArray(0);
}