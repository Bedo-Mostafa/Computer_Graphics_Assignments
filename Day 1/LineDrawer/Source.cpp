#pragma region LineDrawer
#include <iostream>
#include <gl/glew/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Shader.h"
#include "LineDrawer.h"

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

    shaderProgram = InitShader("line_vertex.glsl", "line_fragment.glsl");
    if (shaderProgram == 0) {
        cout << "Failed to initialize shaders!" << endl;
        return 1;
    }

    glDisable(GL_DEPTH_TEST);  // Global for 2D
    glDisable(GL_CULL_FACE);   // For Intel compatibility
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background
    cout << "Line Drawer Ready! Drag to draw (red test line should appear). Space to clear." << endl;
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
    context.antialiasingLevel = 0;  // Disable AA for crisp lines on Intel
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Line Drawer (Intel Fixed)", sf::Style::Close, context);
    if (Init()) return 1;

    LineDrawer drawer(0.005f);  // Thicker: ~6px for visibility on UHD
    drawer.Init(shaderProgram);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            drawer.HandleEvent(event, window);

            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                glViewport(0, 0, event.size.width, event.size.height);
                break;
            }
        }

        Render();
        drawer.Render();

        window.display();
    }

    return 0;
}
#pragma endregion
