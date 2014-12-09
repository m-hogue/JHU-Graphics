#version 150

// Per veretx attributes
in vec2 position;
in vec2 offset;        // A unit vector to offset the vertex

// Uniform
uniform mat4 ortho;
uniform float width;

void main()
{
	// Offset the vertex by specified distance in direction of the offset vector
	vec2 offsetPt = position + offset * width;
	gl_Position = ortho * vec4(offsetPt, 0.0, 1.0);
}