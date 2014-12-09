#version 150
in vec2 position;				// Vertex position
uniform float point_size;		// Point size
uniform mat4 ortho;				// Orthographic projection matrix
void main()
{
	// Set the point size and transform the vertex
    gl_PointSize = point_size;
    gl_Position  = ortho * vec4(position, 0.0, 1.0);
}