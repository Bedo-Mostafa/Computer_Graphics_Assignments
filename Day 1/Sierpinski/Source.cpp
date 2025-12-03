#pragma region Sierpinski
#include <iostream>
#include <gl/glew/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Shader.h"              // Shader utilities
#include "SierpinskiTriangle.h"  // Sierpinski class

using namespace std;

const GLint WIDTH = 600, HEIGHT = 600;
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
    else
    {
        if (GLEW_VERSION_3_0)
            cout << "Driver supports OpenGL 3.0\nDetails:\n";
    }
    cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // Load Sierpinski-specific shaders
    shaderProgram = InitShader("sierp_vertex.glsl", "sierp_fragment.glsl");
    if (shaderProgram == 0) {
        cout << "Failed to initialize shaders!" << endl;
        return 1;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background to match screenshot
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
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sierpinski", sf::Style::Close, context);
    if (Init()) return 1;

    // Create and initialize the Sierpinski triangle (depth 5 for detail like screenshot)
    SierpinskiTriangle sierp(5, 1.0f);
    sierp.Init(shaderProgram);

    while (window.isOpen())
    {
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
            }
        }

        Render();
        // Render the Sierpinski triangle
        sierp.Render();

        window.display();
    }

    return 0;
}
#pragma endregion
