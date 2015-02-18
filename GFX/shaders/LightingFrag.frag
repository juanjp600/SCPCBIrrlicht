#version 110

varying mat4 olightPos;
varying vec3 v;
varying vec3 TN;

uniform vec4 lightColor1;
uniform vec4 lightColor2;
uniform vec4 lightColor3;
uniform vec4 lightColor4;

uniform sampler2D baseMap;

uniform vec4 ambientLight;

float getLengthSQR (vec3 vec) {
	return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
} 

void main(void)
{
	vec4 oLightColor1 = lightColor1 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[0].xyz)/olightPos[0].w),0.0)),1.0);
	vec4 oLightColor2 = lightColor2 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[1].xyz)/olightPos[1].w),0.0)),1.0);
	vec4 oLightColor3 = lightColor3 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[2].xyz)/olightPos[2].w),0.0)),1.0);
	vec4 oLightColor4 = lightColor4 * vec4(vec3(max(1.0/(getLengthSQR(olightPos[3].xyz)/olightPos[3].w),0.0)),1.0);

	vec3 L1 = normalize(olightPos[0].xyz);
	vec3 L2 = normalize(olightPos[1].xyz);
	vec3 L3 = normalize(olightPos[2].xyz);
	vec3 L4 = normalize(olightPos[3].xyz);
	
	vec4 Idiff1 = vec4(0.0,0.0,0.0,1.0);
	vec4 Idiff2 = Idiff1;
	vec4 Idiff3 = Idiff1;
	vec4 Idiff4 = Idiff1;
	
	vec4 reflection1 = Idiff1;
	vec4 reflection2 = Idiff1;
	vec4 reflection3 = Idiff1;
	vec4 reflection4 = Idiff1;
	
	float lambertTerm;

	lambertTerm = dot(TN,L1);
	Idiff1 = oLightColor1 * vec4(max(lambertTerm, 0.0));

	reflection1 = min(pow(max(0.0,dot(normalize(L1+v),TN)),20.0),1.0)*vec4(0.5,0.5,0.5,1.0);
	
	reflection1.x=min(1.0,reflection1.x);
	reflection1.y=min(1.0,reflection1.y);
	reflection1.z=min(1.0,reflection1.z);
	reflection1.w=1.0;
	
	Idiff1+=reflection1*oLightColor1*vec4(2.0,2.0,2.0,1.0);
	
	lambertTerm = dot(TN,L2);
	Idiff2 = oLightColor2 * vec4(max(lambertTerm, 0.0));

	reflection2 = min(pow(max(0.0,dot(normalize(L2+v),TN)),20.0),1.0)*vec4(0.5,0.5,0.5,1.0);

	reflection2.x=min(1.0,reflection2.x);
	reflection2.y=min(1.0,reflection2.y);
	reflection2.z=min(1.0,reflection2.z);
	reflection2.w=1.0;
	
	Idiff2+=reflection2*oLightColor2*vec4(2.0,2.0,2.0,1.0);

	lambertTerm = dot(TN,L3);
	Idiff3 = oLightColor3 * vec4(max(lambertTerm, 0.0));

	reflection3 = min(pow(max(0.0,dot(normalize(L3+v),TN)),20.0),1.0)*vec4(0.5,0.5,0.5,1.0);
	
	reflection3.x=min(1.0,reflection3.x);
	reflection3.y=min(1.0,reflection3.y);
	reflection3.z=min(1.0,reflection3.z);
	reflection3.w=1.0;
	
	Idiff3+=reflection3*oLightColor3*vec4(2.0,2.0,2.0,1.0);

	lambertTerm = dot(TN,L4);
	Idiff4 = oLightColor4 * vec4(max(lambertTerm, 0.0));

	reflection4 = min(pow(max(0.0,dot(normalize(L4+v),TN)),20.0),1.0)*vec4(0.5,0.5,0.5,1.0);

	reflection4.x=min(1.0,reflection4.x);
	reflection4.y=min(1.0,reflection4.y);
	reflection4.z=min(1.0,reflection4.z);
	reflection4.w=1.0;
	
	Idiff4+=reflection4*oLightColor4*vec4(2.0,2.0,2.0,1.0);

	
	Idiff1.w = 1.0; Idiff2.w = 1.0; Idiff3.w = 1.0; Idiff4.w = 1.0;
	
	vec4 fdiff1 = Idiff1+Idiff2+Idiff3+Idiff4+ambientLight;

	gl_FragColor = vec4(vec4(fdiff1)*vec4(texture2D( baseMap, gl_TexCoord[0].xy ).xyz,1.0));
}