#version 150

smooth in vec4 color;         // Incoming color - interpolated
   
void main() 
{
   gl_FragColor = color;
}