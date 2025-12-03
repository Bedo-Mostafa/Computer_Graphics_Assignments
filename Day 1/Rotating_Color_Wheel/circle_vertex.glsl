#version 330 core

layout (location = 0) in vec2 inPosition;
out vec2 fragPosition;

uniform float rotationAngle;  // Rotation angle in radians

void main()
{
    // Compute rotation matrix in shader
    float cosA = cos(rotationAngle);
    float sinA = sin(rotationAngle);
    
    // Apply 2D rotation
    vec2 rotatedPos;
    rotatedPos.x = inPosition.x * cosA - inPosition.y * sinA;
    rotatedPos.y = inPosition.x * sinA + inPosition.y * cosA;
    
    gl_Position = vec4(rotatedPos, 0.0, 1.0);
    
    // CHANGED: Use original position for color, not rotated position
    // This makes colors rotate WITH the shape
    fragPosition = inPosition;
}