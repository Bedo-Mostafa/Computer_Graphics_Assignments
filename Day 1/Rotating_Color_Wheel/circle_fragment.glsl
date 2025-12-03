#version 330 core

in vec2 fragPosition;
out vec4 fragColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec2 pos = fragPosition;
    float angle = atan(pos.y, pos.x);
    if (angle < 0.0) {
        angle += 6.28318530718;  // 2 * PI
    }
    float hue = angle / 6.28318530718;
    vec3 rgb = hsv2rgb(vec3(hue, 1.0, 1.0));
    float intensity = length(pos);  // Fade to black at center
    fragColor = vec4(rgb * intensity, 1.0);
}