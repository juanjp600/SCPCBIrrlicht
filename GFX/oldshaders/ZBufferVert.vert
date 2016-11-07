#version 110

uniform mat4 mWorld;

varying vec4 coords;

void main(void)
{
	coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = coords;
}
