#version 110

varying vec4 vertexColor;

void main(void)
{
	vec4 coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = coords;
	vertexColor = gl_Color;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
