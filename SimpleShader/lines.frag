// Simple fragment shader that sets the pixel color to the color specified via 
// the color uniform variable
#version 150
uniform vec4 color;
out vec4 fragColor;       // Output color to the frame buffer
void main() 
{
    fragColor = color;
}