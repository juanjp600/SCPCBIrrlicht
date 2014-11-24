#version 110

//heavily based on this shader: http://irrlicht.sourceforge.net/forum//viewtopic.php?f=9&t=21186

//pos = xyz; radius ^ 2 = w
uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightPos3;
uniform vec4 lightPos4;

//light radius ^ 2, because that way GPU does less math
/*uniform float lightRadius1;
uniform float lightRadius2;
uniform float lightRadius3;
uniform float lightRadius4;*/

varying mat3 TNTB;
varying vec3 v;

//light positions for specular mapping
varying mat4 olightPos;

/*//transformed light positions for normal mapping
varying vec3 oTlightPos1;
varying vec3 oTlightPos2;
varying vec3 oTlightPos3;
varying vec3 oTlightPos4;

varying vec4 oLightColor1;
varying vec4 oLightColor2;
varying vec4 oLightColor3;
varying vec4 oLightColor4;*/

//varying mat3 oNormalMatrix;

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
