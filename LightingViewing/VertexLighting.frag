#version 150

out vec4 fragColor;

// Incoming color - interpolated
smooth in vec4 color;         
   
void main()
{
	fragColor = color;
}
