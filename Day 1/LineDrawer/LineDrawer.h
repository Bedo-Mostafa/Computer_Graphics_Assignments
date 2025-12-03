#pragma once
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <vector>

class LineDrawer {
public:
    LineDrawer(float lineThickness = 0.005f);
    ~LineDrawer();

    void Init(GLuint shaderProgram);
    void HandleEvent(const sf::Event& event, const sf::Window& window);
    void Render();
    void Clear();

private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
    GLint positionLoc;
    std::vector<float> vertices;
    bool isDrawing;
    sf::Vector2f lastPoint;
    float thickness;

    void UpdateVBO();
    void AddThickSegment(sf::Vector2f start, sf::Vector2f end);
};