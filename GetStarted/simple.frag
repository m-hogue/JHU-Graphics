#version 150

uniform vec4 color;    // Constant input color
out vec4 fragColor;    // Output fragment color

void main() 
{
	fragColor = color;
}