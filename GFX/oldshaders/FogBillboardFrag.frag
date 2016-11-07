#version 110

uniform sampler2D Texture0; //base texture

varying vec4 vertexColor;
varying float depth;

void main (void)
{
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	//col*=vec4(vec3(2.0),1.0);
	col*=vertexColor;
	//col.a *= min(min(gl_FragCoord.z*10.0,1.0),(1.0-gl_FragCoord.z)*10.0);
	col.a *= max(0.0,min(min(depth*2.0,1.0),(1.0-depth)*2.0))*0.1;
	
    gl_FragColor = col;
}