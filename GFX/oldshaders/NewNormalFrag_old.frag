#version 110

//heavily based on this shader: http://irrlicht.sourceforge.net/forum//viewtopic.php?f=9&t=21186

varying mat3 TNTB;
varying vec3 v;
varying mat4 olightPos;

uniform vec4 lightColor1;
uniform vec4 lightColor2;
uniform vec4 lightColor3;
uniform vec4 lightColor4;

uniform sampler2D baseMap;
uniform sampler2D bumpMap;
uniform sampler2D specMap;

uniform vec4 ambientLight;

float getLengthSQR (vec3 vec) {
	return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
} 

void main(void)
{
	vec3 oTlightPos1;
	oTlightPos1.x = dot( TNTB[1], olightPos[0].xyz );
	oTlightPos1.y = dot( TNTB[2], olightPos[0].xyz );
	oTlightPos1.z = dot( TNTB[0], olightPos[0].xyz );
	vec3 oTlightPos2;
	oTlightPos2.x = dot( TNTB[1], olightPos[1].xyz );
	oTlightPos2.y = dot( TNTB[2], olightPos[1].xyz );
	oTlightPos2.z = dot( TNTB[0], olightPos[1].xyz );
	vec3 oTlightPos3;
	oTlightPos3.x = dot( TNTB[1], olightPos[2].xyz );
	oTlightPos3.y = dot( TNTB[2], olightPos[2].xyz );
	oTlightPos3.z = dot( TNTB[0], olightPos[2].xyz );
	vec3 oTlightPos4;
	oTlightPos4.x = dot( TNTB[1], olightPos[3].xyz );
	oTlightPos4.y = dot( TNTB[2], olightPos[3].xyz );
	oTlightPos4.z = dot( TNTB[0], olightPos[3].xyz );
	
	vec4 oLightColor1 = lightColor1 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[0].xyz)/olightPos[0].w),0.0)),1.0);
	vec4 oLightColor2 = lightColor2 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[1].xyz)/olightPos[1].w),0.0)),1.0);
	vec4 oLightColor3 = lightColor3 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[2].xyz)/olightPos[2].w),0.0)),1.0);
	vec4 oLightColor4 = lightColor4 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[3].xyz)/olightPos[3].w),0.0)),1.0);
	
	vec3 distNormal = texture2D( bumpMap, gl_TexCoord[0].xy ).xyz;
	
	vec3 viewVec = normalize(v);
	
	distNormal.xy*=vec2(2.0);
	distNormal.xy-=vec2(1.0);
	distNormal=(vec3(0.0,0.0,1.0)-distNormal)*4.0+distNormal;
	
	vec3 fN = normalize(distNormal);
	vec3 L1 = oTlightPos1;
	vec3 L2 = oTlightPos2;
	vec3 L3 = oTlightPos3;
	vec3 L4 = oTlightPos4;
	
	vec4 Idiff1 = vec4(0.0,0.0,0.0,1.0);
	vec4 Idiff2 = Idiff1;
	vec4 Idiff3 = Idiff1;
	vec4 Idiff4 = Idiff1;
	
	vec4 reflection1 = Idiff1;
	vec4 reflection2 = Idiff1;
	vec4 reflection3 = Idiff1;
	vec4 reflection4 = Idiff1;
	
	vec3 refNormDir = normalize(TNTB[0]);
	float lambertTerm;
	
	vec4 specularTerm = texture2D( specMap, gl_TexCoord[0].xy );
	
	L1 = normalize(L1);
	Idiff1 = oLightColor1 * vec4(max(dot(fN,L1), 0.0));
	L1 = normalize(olightPos[0].xyz);

	lambertTerm = dot(refNormDir,L1);
	reflection1 = min(pow(max(0.0,dot(normalize(L1+viewVec),refNormDir)),20.0),1.0)*specularTerm;
	//}
	reflection1.x=min(1.0,reflection1.x);
	reflection1.y=min(1.0,reflection1.y);
	reflection1.z=min(1.0,reflection1.z);
	reflection1.w=1.0;
	
	Idiff1+=reflection1*oLightColor1*vec4(15.0,15.0,15.0,1.0);

	L2 = normalize(L2);
	Idiff2 = oLightColor2 * vec4(max(dot(fN,L2), 0.0));
	L2 = normalize(olightPos[1].xyz);

	lambertTerm = dot(refNormDir,L2);
	reflection2 = min(pow(max(0.0,dot(normalize(L2+viewVec),refNormDir)),20.0),1.0)*specularTerm;
	//}
	reflection2.x=min(1.0,reflection2.x);
	reflection2.y=min(1.0,reflection2.y);
	reflection2.z=min(1.0,reflection2.z);
	reflection2.w=1.0;
	
	Idiff2+=reflection2*oLightColor2*vec4(15.0,15.0,15.0,1.0);

	L3 = normalize(L3);
	Idiff3 = oLightColor3 * vec4(max(dot(fN,L3), 0.0));
	L3 = normalize(olightPos[2].xyz);

	lambertTerm = dot(refNormDir,L3);
	reflection3 = min(pow(max(0.0,dot(normalize(L3+viewVec),refNormDir)),20.0),1.0)*specularTerm;
	reflection3.x=min(1.0,reflection3.x);
	reflection3.y=min(1.0,reflection3.y);
	reflection3.z=min(1.0,reflection3.z);
	reflection3.w=1.0;
	
	Idiff3+=reflection3*oLightColor3*vec4(15.0,15.0,15.0,1.0);

	L4 = normalize(L4);
	Idiff4 = oLightColor4 * vec4(max(dot(fN,L4), 0.0));
	L4 = normalize(olightPos[3].xyz);

	lambertTerm = dot(refNormDir,L4);
	reflection4 = min(pow(max(0.0,dot(normalize(L4+viewVec),refNormDir)),20.0),1.0)*specularTerm;
	reflection4.x=min(1.0,reflection4.x);
	reflection4.y=min(1.0,reflection4.y);
	reflection4.z=min(1.0,reflection4.z);
	reflection4.w=1.0;
	
	Idiff4+=reflection4*oLightColor4*vec4(15.0,15.0,15.0,1.0);
	
	Idiff1.w = 1.0; Idiff2.w = 1.0; Idiff3.w = 1.0; Idiff4.w = 1.0;
	
	gl_FragColor = vec4(Idiff1+Idiff2+Idiff3+Idiff4+ambientLight)*vec4(texture2D( baseMap, gl_TexCoord[0].xy ).xyz,1.0);
}