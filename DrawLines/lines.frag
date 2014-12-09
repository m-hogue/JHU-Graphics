#version 150 
smooth in vec4 color;		// Smooth shaded input color
out vec4 fragColor;			// Output color to framebuffer  
void main() 
{
    fragColor = color;
}