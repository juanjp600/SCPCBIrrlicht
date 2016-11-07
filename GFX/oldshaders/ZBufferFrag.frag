#version 110

varying vec4 coords;

//uniform float far;

void main(void) {
	float dist = coords.z;
	
	dist /= 150.0;
	dist = min(dist,1.0);
	
	gl_FragColor = vec4(dist,0.0,0.0,1.0);
}