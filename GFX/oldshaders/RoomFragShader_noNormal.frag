uniform sampler2D Texture0; //base texture
uniform sampler2D Texture1; //lightmap

uniform sampler2D fogTexture;

uniform float fogNear;
uniform float fogFar;

varying vec3 normal;

varying vec4 distFromCenter;

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
	
	float fog = (sqrt(distFromCenter.x*distFromCenter.x+distFromCenter.y*distFromCenter.y+distFromCenter.z*distFromCenter.z)-fogNear);
	fog /= fogFar-fogNear;
	fog = clamp(1.0-fog,0.0,1.0);
	fog*=fog;
	
    gl_FragColor = (col*vec4(fog))+(texture2D(fogTexture,gl_FragCoord.xy/vec2(1280.0,720.0))*vec4(1.0-fog));
}