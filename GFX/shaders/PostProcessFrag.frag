varying vec4 tcoords;

uniform float minBlur;
uniform float maxBlur;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

float getDepth(in vec2 coords) {
	vec3 zbuff = texture2D(Texture1, clamp(coords,0.0,1.0)).xyz;
	return zbuff.x;
}

void main(void) {
	vec2 ntcoords = tcoords.xy;
	
	float dx = ntcoords.x-0.5;
	float dy = ntcoords.y-0.5;
	float dist = sqrt(dx*dx+dy*dy);
	dist *= dist * dist * (maxBlur-minBlur);
	dist += minBlur;
	
	float zdist = getDepth(ntcoords);
	float centerzdist1 = getDepth(vec2(0.5));
	float centerzdist2 = getDepth(vec2(0.5,0.51));
	float centerzdist3 = getDepth(vec2(0.5,0.49));
	float centerzdist4 = getDepth(vec2(0.51,0.5));
	float centerzdist5 = getDepth(vec2(0.49,0.5));
	
	float centerzdist = min(centerzdist1,min(centerzdist2,min(centerzdist3,min(centerzdist4,centerzdist5))));
	
	dist += abs(zdist-centerzdist)*0.05;
	
	float fzdist = zdist;
	vec4 color = texture2D(Texture0, clamp(ntcoords,0.0,1.0)) * vec4(0.1);
	
	dist*=0.001;
	
	color += texture2D(Texture0, clamp(ntcoords+vec2(dist),0.0,1.0)) * vec4(0.225);
	color += texture2D(Texture0, clamp(ntcoords-vec2(dist),0.0,1.0)) * vec4(0.225);
	color += texture2D(Texture0, clamp(ntcoords+vec2(dist,-dist),0.0,1.0)) * vec4(0.225);
	color += texture2D(Texture0, clamp(ntcoords-vec2(dist,-dist),0.0,1.0)) * vec4(0.225);
	
	gl_FragColor = vec4(color.xyz,1.0);
	
}