varying vec4 tcoords;

uniform sampler2D Texture0;

uniform float invGamma;

void main(void) {
	vec4 color = texture2D(Texture0,tcoords.xy);
	color.r = pow(color.r,invGamma);
	color.g = pow(color.g,invGamma);
	color.b = pow(color.b,invGamma);
	gl_FragColor = color;
}