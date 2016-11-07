#version 110

uniform vec4 lightPos1;
uniform mat4 lightMatrix;

varying vec4 olightPos;
varying vec3 v;
varying vec3 TN;
varying vec3 TT;
varying vec3 TB;
varying vec4 pixelFromLight;

void main(void)
{
	v = -vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 N = gl_Normal;
	pixelFromLight = (lightMatrix * gl_Vertex);
	
	//vec3 T = -vec3(abs(N.y) + abs(N.z), abs(N.x), 0);
	//vec3 B = cross(T,N);
	
	float oneDivSqrtSix = 1.0/sqrt(6.0);
	float oneDivSqrtTwo = 1.0/sqrt(2.0);
	float oneDivSqrtThree = 1.0/sqrt(3.0);
	float sqrtTwoDivThree = sqrt(2.0/3.0);
	TN = vec3(-N.z*oneDivSqrtSix,-N.z*oneDivSqrtTwo,N.z*oneDivSqrtThree);
	TN += vec3(-N.y*oneDivSqrtTwo,N.y*oneDivSqrtThree,-N.y*oneDivSqrtSix);
	TN += vec3(N.x*oneDivSqrtThree,-N.x*oneDivSqrtSix,-N.x*oneDivSqrtTwo);
	
	TT = vec3(-N.z*oneDivSqrtSix,N.z*oneDivSqrtTwo,N.z*oneDivSqrtThree);
	TT += vec3(N.y*oneDivSqrtTwo,N.y*oneDivSqrtThree,-N.y*oneDivSqrtSix);
	TT += vec3(N.x*oneDivSqrtThree,-N.x*oneDivSqrtSix,N.x*oneDivSqrtTwo);
	
	TB = vec3(N.z*sqrtTwoDivThree,0.0,N.z*oneDivSqrtThree);
	TB += vec3(0.0,N.y*oneDivSqrtThree,N.y*sqrtTwoDivThree);
	TB += vec3(N.x*oneDivSqrtThree,N.x*sqrtTwoDivThree,0.0);
	
	TN = gl_NormalMatrix*normalize(TN);
	TT = gl_NormalMatrix*normalize(TT);
	TB = gl_NormalMatrix*normalize(TB);
	
	olightPos = vec4(lightPos1.xyz + v,lightPos1.w);
	
	gl_Position = vec4(ceil((gl_MultiTexCoord1.x*2.0-1.0)*1024.0)/1024.0,-ceil((gl_MultiTexCoord1.y*2.0-1.0)*1024.0)/1024.0,0.0,1.0);//gl_ModelViewProjectionMatrix * gl_Vertex;
	
	v = normalize(v);
	
	gl_TexCoord[0] = gl_MultiTexCoord1;
	
}