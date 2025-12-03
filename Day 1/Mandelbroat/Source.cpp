#pragma region Mandelbrot
#include <iostream>
#include <gl/glew/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Shader.h"
#include "Mandelbrot.h"

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
GLuint shaderProgram;

int Init()
{
    glViewport(0, 0, WIDTH, HEIGHT);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error: " << glewGetErrorString(err) << endl;
        return 1;
    }
    cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    shaderProgram = InitShader("mandel_vertex.glsl", "mandel_fragment.glsl");
    if (shaderProgram == 0) {
        cout << "Failed to initialize shaders!" << endl;
        return 1;
    }

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return 0;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
}

int main()
{
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Dynamic Mandelbrot", sf::Style::Close, context);
    if (Init()) return 1;

    Mandelbrot mandel(100);
    mandel.Init(shaderProgram);

    sf::Clock clock;
    sf::Vector2f lastMousePos;
    bool isDragging = false;

    while (window.isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();  // Time for animation

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                glViewport(0, 0, event.size.width, event.size.height);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = true;
                    lastMousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
                break;
            case sf::Event::MouseMoved:
                if (isDragging) {
                    sf::Vector2f currentPos(event.mouseMove.x, event.mouseMove.y);
                    sf::Vector2f delta = currentPos - lastMousePos;
                    mandel.HandleMouseDrag(delta.x, delta.y);
                    lastMousePos = currentPos;
                }
                break;
            case sf::Event::MouseWheelScrolled:
                mandel.HandleMouseWheel(event.mouseWheelScroll.delta);
                break;
            }
        }

        Render();
        mandel.Render(currentTime);  // Pass time for dynamic animation

        window.display();
    }

    return 0;
}
#pragma endregion
