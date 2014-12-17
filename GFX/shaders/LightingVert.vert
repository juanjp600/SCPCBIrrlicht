#version 110

uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightPos3;
uniform vec4 lightPos4;

varying mat4 olightPos;
varying vec3 v;
varying vec3 TN;

void main(void)
{
	v = -vec3(gl_ModelViewMatrix * gl_Vertex);       
	vec3 N = gl_Normal;
	
	TN = normalize(gl_NormalMatrix*N);
	
	olightPos[0] = vec4(lightPos1.xyz + v,lightPos1.w);
	olightPos[1] = vec4(lightPos2.xyz + v,lightPos1.w);
	olightPos[2] = vec4(lightPos3.xyz + v,lightPos1.w);
	olightPos[3] = vec4(lightPos4.xyz + v,lightPos1.w);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	v = normalize(v);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}