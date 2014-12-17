#version 110

uniform mat4 mWorld;
//uniform mat4 mInvWorld;
//uniform mat4 mTransWorld;

varying vec3 normal;

//out vec4 worldpos;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	normal = (mWorld*vec4(gl_Normal,0.0)).xyz;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
}
