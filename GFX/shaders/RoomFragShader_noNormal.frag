#version 110

uniform sampler2D Texture0; //base texture
uniform sampler2D Texture1; //lightmap

varying vec3 normal;

void main (void)
{
	vec4 col2 = texture2D(Texture1, vec2(gl_TexCoord[1]));
	col2 += texture2D(Texture1, vec2(gl_TexCoord[1])+vec2(1.0/1024.0,1.0/1024.0));
	col2 += texture2D(Texture1, vec2(gl_TexCoord[1])-vec2(1.0/1024.0,1.0/1024.0));
	col2 += texture2D(Texture1, vec2(gl_TexCoord[1])+vec2(1.0/1024.0,-1.0/1024.0));
	col2 += texture2D(Texture1, vec2(gl_TexCoord[1])+vec2(-1.0/1024.0,1.0/1024.0));
	col2*=vec4(vec3(0.2),1.0);
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	col*=vec4(vec3(2.0),1.0);
	col *= col2;
	
    gl_FragColor = col;
}