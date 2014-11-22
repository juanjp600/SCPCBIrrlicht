#version 110

varying vec4 coords;

//uniform float far;

void main(void) {
	float dist = coords.z;
	
	dist /= 150.0;
	dist *= 256.0*256.0*256.0;
	dist = min(dist,256.0*256.0*256.0-1.0);
	vec3 coutput;
	//store the depth value in rgb
	coutput.x = mod(floor(dist),256.0)/255.0;
	coutput.y = mod(floor(dist/256.0),256.0)/255.0;
	coutput.z = min(floor(dist/(256.0*256.0)),255.0)/255.0;
	
	//vec3 whitecolor = vec3((coutput.x*255.0)+(coutput.y*255.0*255.0)+(coutput.z*255.0*255.0*255.0))/(255.0*255.0*255.0);
	gl_FragColor = vec4(coutput,1.0);
}