#include "LineDrawer.h"
#include <iostream>
#include <cmath>

LineDrawer::LineDrawer(float lineThickness)
	: isDrawing(false), thickness(lineThickness), shaderProgram(0) {
}

LineDrawer::~LineDrawer() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void LineDrawer::Init(GLuint shaderProg) {
	this->shaderProgram = shaderProg;  // Store shader program
	positionLoc = glGetUniformLocation(shaderProgram, "transform");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Don't allocate buffer yet, wait until we have data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LineDrawer::HandleEvent(const sf::Event& event, const sf::Window& window) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos(
		static_cast<float>(mousePos.x) / window.getSize().x * 2.0f - 1.0f,
		1.0f - static_cast<float>(mousePos.y) / window.getSize().y * 2.0f
	);

	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left) {
			isDrawing = true;
			lastPoint = worldPos;
			std::cout << "Start drawing at (" << worldPos.x << ", " << worldPos.y << ")" << std::endl;
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			isDrawing = false;
			std::cout << "Stopped drawing. Total vertices: " << vertices.size() << std::endl;
		}
		break;
	case sf::Event::MouseMoved:
		if (isDrawing) {
			AddThickSegment(lastPoint, worldPos);
			lastPoint = worldPos;
			UpdateVBO();
		}
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Space) {
			Clear();
			std::cout << "Cleared drawing." << std::endl;
		}
		break;
	default:
		break;
	}
}

void LineDrawer::AddThickSegment(sf::Vector2f start, sf::Vector2f end) {
	sf::Vector2f dir = end - start;
	float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (len == 0.0f) return;

	dir /= len;
	sf::Vector2f perp = { -dir.y * thickness, dir.x * thickness };

	// Two triangles for thick line quad
	vertices.insert(vertices.end(), {
		start.x + perp.x, start.y + perp.y,
		end.x + perp.x, end.y + perp.y,
		end.x - perp.x, end.y - perp.y
		});
	vertices.insert(vertices.end(), {
		start.x - perp.x, start.y - perp.y,
		start.x + perp.x, start.y + perp.y,
		end.x - perp.x, end.y - perp.y
		});
}

void LineDrawer::UpdateVBO() {
	if (vertices.empty()) return;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Use glBufferData to allocate AND update buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LineDrawer::Render() {
	if (vertices.empty()) return;

	// Activate shader program before drawing!
	glUseProgram(shaderProgram);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 2));
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Check for errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "GL Error in render: " << error << std::endl;
	}
}

void LineDrawer::Clear() {
	vertices.clear();
	isDrawing = false;
}