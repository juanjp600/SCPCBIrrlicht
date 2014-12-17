#version 110

//heavily based on this shader: http://irrlicht.sourceforge.net/forum//viewtopic.php?f=9&t=21186

//pos = xyz; radius ^ 2 = w
uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightPos3;
uniform vec4 lightPos4;

varying mat3 TNTB;
varying vec3 v;
varying mat4 olightPos;

void main(void)
{
	
	v = -vec3(gl_ModelViewMatrix * gl_Vertex);       
	vec3 N = gl_Normal;
	
	/* Irrlicht doesn't update tangent and bitangent
	for animated meshes, and it forces the values to
	gl_TexCoord[1] and [2], so we calculate these in
	the vertex shader instead. */
	vec3 T = -vec3(abs(N.y) + abs(N.z), abs(N.x), 0);
	vec3 B = cross(T,N);
	
	TNTB[0] = gl_NormalMatrix*N;
	TNTB[1] = gl_NormalMatrix*T;
	TNTB[2] = gl_NormalMatrix*B;
	
	olightPos[0] = vec4(lightPos1.xyz + v,lightPos1.w);
	olightPos[1] = vec4(lightPos2.xyz + v,lightPos1.w);
	olightPos[2] = vec4(lightPos3.xyz + v,lightPos1.w);
	olightPos[3] = vec4(lightPos4.xyz + v,lightPos1.w);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}
