//#include "SinWave.h"
//#include <cmath>
//
//SinWave::SinWave(int numPoints)
//    : numPoints(numPoints), frequency(20.0f) {  // ~3 waves across screen
//    GenerateVertices();
//}
//
//SinWave::~SinWave() {
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//}
//
//void SinWave::GenerateVertices() {
//    vertices.clear();
//    float step = 2.0f / (numPoints - 1);  // x from -1 to 1
//    for (int i = 0; i < numPoints; ++i) {
//        float x = -1.0f + i * step;
//        vertices.push_back(x);
//        vertices.push_back(0.0f);  // Base y=0; sin() in shader
//    }
//}
//
//void SinWave::Init(GLuint shaderProgram) {
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//}
//
//void SinWave::Render() {
//    glLineWidth(2.0f);  // Thickness for visibility
//    glBindVertexArray(VAO);
//    glDrawArrays(GL_LINE_STRIP, 0, numPoints);  // Connect points into wave
//    glBindVertexArray(0);
//}
#include "SinWave.h"
#include <cmath>

SinWave::SinWave(int numPoints)
	: numPoints(numPoints), frequency(20.0f), time(0.0f) {
	GenerateVertices();
}

SinWave::~SinWave() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void SinWave::GenerateVertices() {
	vertices.clear();
	float step = 2.0f / (numPoints - 1);  // x from -1 to 1
	for (int i = 0; i < numPoints; ++i) {
		float x = -1.0f + i * step;
		vertices.push_back(x);
		vertices.push_back(0.0f);  // Base y=0; sin() computed in shader
	}
}

void SinWave::Init(GLuint shaderProgram) {
	// Get uniform location for time
	timeLoc = glGetUniformLocation(shaderProgram, "time");

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

void SinWave::Update(float deltaTime) {
	// Update time for animation
	time += deltaTime;
}

void SinWave::Render() {
	// Send current time to shader for animation
	glUniform1f(timeLoc, time);

	glLineWidth(2.0f);  // Thickness for visibility
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, numPoints);
	glBindVertexArray(0);
}