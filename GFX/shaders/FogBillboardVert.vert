#version 110

varying vec4 vertexColor;
varying float depth;

void main(void)
{
	vec4 coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = coords;
	depth = min(coords.z/40.0,1.0);
	vertexColor = gl_Color;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
