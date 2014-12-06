#version 110

varying vec4 coords;

//uniform float far;

void main(void) {
	float dist = sqrt(coords.z*coords.z+coords.x*coords.x+coords.y*coords.y);
	
	dist /= 150.0;
	dist = min(dist,1.0);
	
	gl_FragColor = vec4(dist,0.0,0.0,1.0);
}