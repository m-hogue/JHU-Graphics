#version 150
out vec4 fragColor;			// Output color to framebuffer
void main() 
{
	// Discard pixels outside the "round" portion of the point. Otherwise
	// set the color based on the s,t to produce a smooth shade across the point 
    if (dot(gl_PointCoord-0.5, gl_PointCoord-0.5) > 0.25)
        discard;
    else
		fragColor = vec4(gl_PointCoord.st, 0, 1); 
}