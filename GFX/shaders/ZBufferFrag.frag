varying vec4 coords;

void main(void) {
	float dist = coords.z;
	
	gl_FragColor = vec4(dist,0.0,0.0,1.0);
}