varying vec4 olightPos;
varying vec3 v;
varying vec3 TN;
varying vec3 TT;
varying vec3 TB;
varying vec4 pixelFromLight;

uniform vec4 lightColor1;

uniform sampler2D lightDepthMap;
uniform sampler2D renderedLights;

uniform vec4 ambientLight;

uniform vec3 useTBN;

float getLengthSQR (vec3 vec) {
	return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}

void main(void)
{
	vec2 depthCoords = (((pixelFromLight.xy)/pixelFromLight.w)+vec2(1.0))*vec2(0.5) * vec2(1024.0/1025.0);
	bool isOkay = (depthCoords.x>=-0.0&&depthCoords.x<=1.0&&depthCoords.y>=-0.0&&depthCoords.y<=1.0&&pixelFromLight.z>=0.0&&pixelFromLight.z<=150.0);
	depthCoords = clamp(depthCoords,vec2(0.0),vec2(1.0));
	
	vec4 Idiff1 = vec4(0.0,0.0,0.0,1.0);
	float depthRead = 1.0;
	
	/*if (gl_TexCoord[0].x>1.0 || gl_TexCoord[0].x<0.0 || gl_TexCoord[0].y>1.0 || gl_TexCoord[0].y<0.0) {
		discard;
	}*/
	
	if (isOkay) {
		depthRead = texture2D(lightDepthMap,depthCoords).r;
		/* depthRead += texture2D(lightDepthMap,depthCoords+vec2(1.0/512.0)).r;
		depthRead += texture2D(lightDepthMap,depthCoords-vec2(1.0/512.0)).r;
		depthRead += texture2D(lightDepthMap,depthCoords+vec2(1.0/512.0,-1.0/512.0)).r;
		depthRead += texture2D(lightDepthMap,depthCoords+vec2(-1.0/512.0,1.0/512.0)).r;
		depthRead *= 0.2; */
		if (depthRead+0.003>(pixelFromLight.z/150.0)) {
			vec3 distNormal = normalize(vec3(useTBN.z)*TN + vec3(useTBN.x)*TT + vec3(useTBN.y)*TB);
			
			vec4 oLightColor1 = lightColor1 * vec4(vec3(max(1.0/(getLengthSQR(olightPos.xyz)/olightPos.w),0.0)),1.0);

			vec3 L1 = normalize(olightPos.xyz);
			
			//vec4 reflection1 = Idiff1;
			
			float lambertTerm;

			lambertTerm = dot(distNormal,L1);
			Idiff1 = oLightColor1 * vec4(max(lambertTerm, 0.0));

			/*reflection1 = min(pow(max(0.0,dot(normalize(L1+v),TN)),20.0),1.0)*vec4(0.5,0.5,0.5,1.0);
			
			reflection1.x=min(1.0,reflection1.x);
			reflection1.y=min(1.0,reflection1.y);
			reflection1.z=min(1.0,reflection1.z);
			reflection1.w=1.0;
			
			Idiff1+=reflection1*oLightColor1*vec4(2.0,2.0,2.0,1.0);*/
			
			//Idiff1*=vec4((pixelFromLight.z-0.01<texture2D(lightDepthMap,pixelFromLight.xy*vec2(0.5)+vec2(0.5)).r))+0.1;//&&(abs(pixelFromLight.x)<1.0)&&(abs(pixelFromLight.y)<1.0));
			
			Idiff1.w = 1.0;
		}
	}
	gl_FragColor = (Idiff1*vec4(0.5))+vec4(texture2D( renderedLights, gl_FragCoord.xy/vec2(1024.0) ).xyz,1.0);//vec4(vec3(abs(pixelFromLight.xyz*vec3((abs(pixelFromLight.x)<1.0)&&(abs(pixelFromLight.y)<1.0)&&(pixelFromLight.z>0.0)&&(pixelFromLight.z<1.0))))+vec4(texture2D( renderedLights, gl_FragCoord.xy/vec2(1280.0,720.0) )).xyz,1.0);//texture2D(lightDepthMap,pixelFromLight.xy*vec2(0.5)+vec2(0.5));//vec4(fdiff1+vec4(texture2D( renderedLights, gl_FragCoord.xy/vec2(1280.0,720.0) ).xyz,1.0));
	//gl_FragDepth = 0.0;
}