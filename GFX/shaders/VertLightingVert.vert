uniform mat4 mWorld;
uniform vec3 cameraPos;

varying vec4 vertexColor;

varying vec4 distFromCenter;

void main(void)
{
	vec4 coords = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = coords;
	vertexColor = gl_Color;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	distFromCenter = ((mWorld * gl_Vertex)-vec4(cameraPos,1.0));
}
