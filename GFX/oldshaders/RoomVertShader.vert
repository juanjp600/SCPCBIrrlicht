uniform mat4 mWorld;
uniform vec3 cameraPos;

//uniform mat4 mInvWorld;
//uniform mat4 mTransWorld;

varying vec3 normal;

//out vec4 worldpos;

varying vec4 distFromCenter;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	normal = (vec4(gl_NormalMatrix*gl_Normal,0.0)).xyz;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	distFromCenter = ((mWorld * gl_Vertex)-vec4(cameraPos,1.0));
}
