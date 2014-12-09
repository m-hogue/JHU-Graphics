#version 150
   
in vec2 position;
uniform mat4 ortho;
   
void main()
{
	// Transform incoming vertex to clip coordinates
	gl_Position = ortho * vec4(position, 0.0, 1.0);
}