#version 110

uniform sampler2D Texture0; //base texture

varying vec4 vertexColor;

void main (void)
{
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	col*=vec4(vec3(2.0),1.0);
	col*=vertexColor;
	
    gl_FragColor = col;
}