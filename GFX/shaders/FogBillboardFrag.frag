uniform sampler2D Texture0; //base texture

varying vec4 vertexColor;
varying float depth;

void main (void)
{
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	col*=vertexColor;
	float dist = min(abs(6.0-depth)/7.0,1.0);
	col.a *= max(0.0,1.0-dist)*0.05;
	
    gl_FragColor = col;
}