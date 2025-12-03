//#pragma region SinWave
//#include <iostream>
//#include <gl/glew/glew.h>
//
//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
//
//#include "Shader.h"    // Shader utilities
//#include "SinWave.h"   // SinWave class
//
//using namespace std;
//
//const GLint WIDTH = 800, HEIGHT = 600;
//GLuint shaderProgram;
//
//int Init()
//{
//    glViewport(0, 0, WIDTH, HEIGHT);
//
//    GLenum err = glewInit();
//    if (err != GLEW_OK)
//    {
//        cout << "Error: " << glewGetErrorString(err) << endl;
//        return 1;
//    }
//    cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
//    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
//    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
//    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
//    cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
//
//    // Load SinWave-specific shaders
//    shaderProgram = InitShader("sin_vertex.glsl", "sin_fragment.glsl");
//    if (shaderProgram == 0) {
//        cout << "Failed to initialize shaders!" << endl;
//        return 1;
//    }
//
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background
//    return 0;
//}
//
//void Render()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    glUseProgram(shaderProgram);
//}
//
//int main()
//{
//    sf::ContextSettings context;
//    context.depthBits = 24;
//    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sin Wave", sf::Style::Close, context);
//    if (Init()) return 1;
//
//    // Create and initialize the sine wave
//    SinWave wave(512);  // 512 points for smooth curve
//    wave.Init(shaderProgram);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            switch (event.type)
//            {
//            case sf::Event::Closed:
//                window.close();
//                break;
//            case sf::Event::Resized:
//                glViewport(0, 0, event.size.width, event.size.height);
//                break;
//            }
//        }
//
//        Render();
//        // Render the sine wave
//        wave.Render();
//
//        window.display();
//    }
//
//    return 0;
//}
//#pragma endregion


#pragma region SinWave
#include <iostream>
#include <gl/glew/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Shader.h"    // Shader utilities
#include "SinWave.h"   // SinWave class

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

    // Load SinWave-specific shaders
    shaderProgram = InitShader("sin_vertex.glsl", "sin_fragment.glsl");
    if (shaderProgram == 0) {
        cout << "Failed to initialize shaders!" << endl;
        return 1;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background
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
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Animated Sin Wave", sf::Style::Close, context);
    if (Init()) return 1;

    // Create and initialize the sine wave
    SinWave wave(512);  // 512 points for smooth curve
    wave.Init(shaderProgram);

    sf::Clock clock;  // For deltaTime

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

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

        // Update animation
        wave.Update(deltaTime.asSeconds());

        Render();
        // Render the sine wave
        wave.Render();

        window.display();
    }

    return 0;
}
#pragma endregion