#version 110

varying vec4 coords;

void main(void)
{
	coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = coords;
}
