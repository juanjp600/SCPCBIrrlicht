varying vec4 tcoords;

uniform sampler2D Texture0;

uniform float invGamma;

void main(void) {
	vec4 color = texture2D(Texture0,tcoords);
	color = pow(color,vec4(vec3(invGamma),1.0));
	gl_FragColor = color;
}