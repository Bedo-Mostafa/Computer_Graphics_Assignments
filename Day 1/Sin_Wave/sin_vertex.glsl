#version 330 core

layout (location = 0) in vec2 inPosition;

uniform float time;  // Time for animation

void main()
{
    float x = inPosition.x;
    // Animated wave: sin(x * frequency - time * speed)
    // The -time creates the flowing effect (wave moves right to left)
    float y = sin(x * 60.0 - time * 5.0) * 0.2;
    gl_Position = vec4(x, y, 0.0, 1.0);
}