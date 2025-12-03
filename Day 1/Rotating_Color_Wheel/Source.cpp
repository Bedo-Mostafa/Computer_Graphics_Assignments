# pragma region Circal
#include <iostream>
#include <gl/glew/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Shader.h"  // Shader utilities
#include "Circle.h"  // Circle class

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

	// Load shaders
	shaderProgram = InitShader("circle_vertex.glsl", "circle_fragment.glsl");
	if (shaderProgram == 0) {
		cout << "Failed to initialize shaders!" << endl;
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
}

void Update(sf::Time deltaTime)
{
	// TODO: Update other objects here
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Rotating Color Wheel", sf::Style::Close, context);
	if (Init()) return 1;

	// Create and initialize the circle
	Circle circle(128);  // 128 segments for smoothness
	circle.Init(shaderProgram);

	sf::Clock clock;  // For frame-rate independent updates

	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();  // Delta time in seconds

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
		Update(deltaTime);
		circle.Update(deltaTime.asSeconds());  // Pass delta time to circle

		Render();
		// Render the circle (now in its own method)
		glUseProgram(shaderProgram);
		circle.Render();

		window.display();
	}

	return 0;
}
#pragma endregion
