#version 110

uniform mat4 mWorldViewProj;
//uniform mat4 mInvWorld;
//uniform mat4 mTransWorld;

//out vec4 worldpos;

void main(void)
{
	gl_Position = mWorldViewProj * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
}
