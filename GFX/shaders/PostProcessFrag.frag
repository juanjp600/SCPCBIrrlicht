#version 110

varying vec4 tcoords;
varying float iminBlur;
varying float imaxBlur;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform float gammaFactor;
//uniform float blurStrength;

float getDepth(in vec2 coords) {
	vec3 zbuff = texture2D(Texture1, clamp(coords,0.0,1.0)).xyz;
	return zbuff.x;
}

void main(void) {
	vec2 ntcoords = tcoords.xy;
	
	float dx = ntcoords.x-0.5;
	float dy = ntcoords.y-0.5;
	//float dist = max(abs(dx),abs(dy));
	float dist = sqrt(dx*dx+dy*dy);
	dist *= dist * dist * (imaxBlur-iminBlur);
	dist += iminBlur;
	
	float zdist = getDepth(ntcoords);
	float centerzdist1 = getDepth(vec2(0.5));
	float centerzdist2 = getDepth(vec2(0.5,0.51));
	float centerzdist3 = getDepth(vec2(0.5,0.49));
	float centerzdist4 = getDepth(vec2(0.51,0.5));
	float centerzdist5 = getDepth(vec2(0.49,0.5));
	
	float centerzdist = min(centerzdist1,min(centerzdist2,min(centerzdist3,min(centerzdist4,centerzdist5))));
	
	dist += abs(zdist-centerzdist)*2.0;
	
	float fzdist = zdist;
	zdist = 1.0-zdist;
	zdist *= zdist;
	vec4 color = texture2D(Texture0, clamp(ntcoords,0.0,1.0));// * zdist * vec4(0.1);
	
	dist+=noise1((color.r+color.g-color.b)*100000.0)*10.0;
	
	dist*=0.001;
	
	/*zdist = 1.0-getDepth(ntcoords+vec2(dist));
	zdist *= zdist;
	color += texture2D(Texture0, clamp(ntcoords+vec2(dist),0.0,1.0)) * vec4(0.225*zdist);
	zdist = 1.0-getDepth(ntcoords-vec2(dist));
	zdist *= zdist;
	color += texture2D(Texture0, clamp(ntcoords-vec2(dist),0.0,1.0)) * vec4(0.225*zdist);
	zdist = 1.0-getDepth(ntcoords+vec2(dist,-dist));
	zdist *= zdist;
	color += texture2D(Texture0, clamp(ntcoords+vec2(dist,-dist),0.0,1.0)) * vec4(0.225*zdist);
	zdist = 1.0-getDepth(ntcoords-vec2(dist,-dist));
	zdist *= zdist;
	color += texture2D(Texture0, clamp(ntcoords-vec2(dist,-dist),0.0,1.0)) * vec4(0.225*zdist);*/
	
	color = pow(color,vec4(gammaFactor));
	
	gl_FragColor = vec4(color.xyz,1.0);
	
}