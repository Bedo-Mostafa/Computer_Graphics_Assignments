#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Camera.h"

// Screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Helper to replace gluPerspective if you don't have GLU linked
void PerspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void DrawCube()
{
    glBegin(GL_QUADS);
    // Front Face (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back Face (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Top Face (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Bottom Face (Yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

int main()
{
    // 1. Setup SFML Window
    sf::ContextSettings settings;
    settings.depthBits = 24;

    sf::Window window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Legacy OpenGL FPS", sf::Style::Default, settings);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    // 2. Configure Legacy OpenGL State
    glEnable(GL_DEPTH_TEST); // Enable Z-buffer

    // Setup Projection Matrix (The Lens)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    PerspectiveGL(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // Switch back to ModelView (The World/Camera)
    glMatrixMode(GL_MODELVIEW);

    // 3. Camera Setup
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

    sf::Clock deltaClock;
    bool firstMouse = true;
    sf::Vector2i centerPos(SCR_WIDTH / 2, SCR_HEIGHT / 2);
    sf::Mouse::setPosition(centerPos, window);

    bool running = true;
    while (running)
    {
        float deltaTime = deltaClock.restart().asSeconds();

        // --- INPUT ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) running = false;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) running = false;

            // Resize event to fix aspect ratio if window size changes
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                PerspectiveGL(45.0f, (float)event.size.width / (float)event.size.height, 0.1f, 100.0f);
                glMatrixMode(GL_MODELVIEW);
            }
        }

        // Keyboard
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) camera.ProcessKeyboard(LEFT, deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) camera.ProcessKeyboard(RIGHT, deltaTime);

        // Mouse
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float xoffset = (float)(mousePos.x - centerPos.x);
        float yoffset = (float)(mousePos.y - centerPos.y);
        sf::Mouse::setPosition(centerPos, window);
        camera.ProcessMouseMovement(xoffset, yoffset);

        // --- RENDER ---
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. Reset Matrix
        glLoadIdentity();

        // 2. Apply Camera (View Matrix)
        camera.ApplyView();

        // 3. Draw Scene
        // We push matrix so transformations here don't affect other objects if we added more
        glPushMatrix();
        DrawCube();
        glPopMatrix();

        window.display();
    }

    return 0;
}