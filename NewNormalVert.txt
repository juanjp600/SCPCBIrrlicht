#version 110

//heavily based on this shader: http://irrlicht.sourceforge.net/forum//viewtopic.php?f=9&t=21186

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;

uniform vec4 lightColor1;
uniform vec4 lightColor2;
uniform vec4 lightColor3;
uniform vec4 lightColor4;

//light radius ^ 2, because that way GPU does less math
uniform float lightRadius1;
uniform float lightRadius2;
uniform float lightRadius3;
uniform float lightRadius4;

varying vec3 N,TN; //fragment shader doesn't need tangent or bitangent
varying vec3 v;

//light positions for specular mapping
varying vec3 olightPos1;
varying vec3 olightPos2;
varying vec3 olightPos3;
varying vec3 olightPos4;

//transformed light positions for normal mapping
varying vec3 oTlightPos1;
varying vec3 oTlightPos2;
varying vec3 oTlightPos3;
varying vec3 oTlightPos4;

varying vec4 oLightColor1;
varying vec4 oLightColor2;
varying vec4 oLightColor3;
varying vec4 oLightColor4;

varying mat3 oNormalMatrix;

float getLengthSQR (vec3 vec) {
	return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
} 

void main(void)
{
	
	v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	N = gl_Normal;
	
	/* Irrlicht doesn't update tangent and bitangent
	for animated meshes, and it forces the values to
	gl_TexCoord[1] and [2], so we calculate these in
	the vertex shader instead. */
	vec3 T = -vec3(abs(N.y) + abs(N.z), abs(N.x), 0);
	vec3 B = cross(T,N);
	
	TN = gl_NormalMatrix*N;
	vec3 TT = gl_NormalMatrix*cross(B,N);
	vec3 TB = gl_NormalMatrix*B;
	
	oNormalMatrix = gl_NormalMatrix;
	
	olightPos1 = lightPos1 - v;
	olightPos2 = lightPos2 - v;
	olightPos3 = lightPos3 - v;
	olightPos4 = lightPos4 - v;
	
	oLightColor1 = lightColor1;
	oLightColor2 = lightColor2;
	oLightColor3 = lightColor3;
	oLightColor4 = lightColor4;
	
	oLightColor1 *= vec4(vec3(max(1.0/(getLengthSQR(olightPos1)/lightRadius1),0.0)),1.0);
	oLightColor2 *= vec4(vec3(max(1.0/(getLengthSQR(olightPos2)/lightRadius2),0.0)),1.0);
	oLightColor3 *= vec4(vec3(max(1.0/(getLengthSQR(olightPos3)/lightRadius3),0.0)),1.0);
	oLightColor4 *= vec4(vec3(max(1.0/(getLengthSQR(olightPos4)/lightRadius4),0.0)),1.0);
	
	//we don't care about a light that's too far away
	if (oLightColor1.x>0.001 || oLightColor1.y>0.001 || oLightColor1.z>0.001) {
		oTlightPos1.x = dot( TT, olightPos1.xyz );
		oTlightPos1.y = dot( TB, olightPos1.xyz );
		oTlightPos1.z = dot( TN, olightPos1.xyz );
	}
	if (oLightColor2.x>0.001 || oLightColor2.y>0.001 || oLightColor2.z>0.001) {
		oTlightPos2.x = dot( TT, olightPos2.xyz );
		oTlightPos2.y = dot( TB, olightPos2.xyz );
		oTlightPos2.z = dot( TN, olightPos2.xyz );
	}
	if (oLightColor3.x>0.001 || oLightColor3.y>0.001 || oLightColor3.z>0.001) {
		oTlightPos3.x = dot( TT, olightPos3.xyz );
		oTlightPos3.y = dot( TB, olightPos3.xyz );
		oTlightPos3.z = dot( TN, olightPos3.xyz );
	}
	if (oLightColor4.x>0.001 || oLightColor4.y>0.001 || oLightColor4.z>0.001) {
		oTlightPos4.x = dot( TT, olightPos4.xyz );
		oTlightPos4.y = dot( TB, olightPos4.xyz );
		oTlightPos4.z = dot( TN, olightPos4.xyz );
	}
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}
