#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform sampler1D tex;
uniform vec2 center;
uniform float scale;
uniform int iter;
uniform float time;

void main() {
    vec2 z, c;

    // Map screen coordinates to complex plane
    c.x = 1.3333 * (fragUV.x - 0.5) * scale - center.x;
    c.y = (fragUV.y - 0.5) * scale - center.y;

    int i;
    z = c;
    
    for(i = 0; i < iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    // Smooth coloring using continuous iteration count
    float t;
    if(i < iter) {
        // Calculate smooth iteration count
        float log_zn = log(z.x * z.x + z.y * z.y) / 2.0;
        float nu = log(log_zn / log(2.0)) / log(2.0);
        
        // Smooth iteration count
        t = float(i) + 1.0 - nu;
        
        // Normalize to texture coordinates (adjust divisor for color cycling speed)
        t = t / 100.0;
        
        // Optional: Add time-based color cycling
        t = fract(t + time * 0.05);
    } else {
        // Inside the set - black
        t = 0.0;
    }

    fragColor = texture(tex, t);
}