#version 150
in vec2 position;			// Vertex position
in vec3 vertexColor;		// Vertex color
uniform mat4 ortho;			// Orthographic projection matrix
smooth out vec4 color;		// Smooth shaded output to fragment shader
void main()
{
	// Set the output color and transform the position
    color = vec4(vertexColor, 1.0);
    gl_Position = ortho * vec4(position, 0.0, 1.0);
}