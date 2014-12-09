// Simple vertex shader that transforms the incoming vertex position attribute
// to clip coordinates using the orthographic projection matrix provided via
// the ortho uniform variable
#version 150
in vec2 position;   // 2-D vertex position
uniform mat4 ortho;        // 4x4 orthographic projection matrix
void main()
{
	// Convert position to clip coordinates and pass along
	// Note that we need to make the 2D position a 4 component
	// vector so the 4x4 matrix can multiply it
	gl_Position = ortho * vec4(position, 0.0, 1.0);
}
