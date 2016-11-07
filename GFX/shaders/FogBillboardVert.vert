varying vec4 vertexColor;
varying float depth;

void main(void)
{
	vec4 coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	depth = coords.z;
	vertexColor = gl_Color;
	
	coords.z = 0.3;	
	gl_Position = coords;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
