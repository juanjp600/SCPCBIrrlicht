#version 110

uniform sampler2D Texture0; //base texture
uniform sampler2D Texture1; //lightmap
uniform sampler2D Texture1_t; //lightmap
uniform sampler2D Texture1_b; //lightmap
uniform sampler2D Texture2; //distortion map

uniform sampler2D reflection;

uniform float reflectFactor;

varying vec3 normal;

void main (void)
{
	vec4 col3 = texture2D(Texture2, vec2(gl_TexCoord[0]));
	
	vec4 col4 = texture2D(reflection, (gl_FragCoord.xy/vec2(-1280.0,720.0))+((col3.rg-vec2(0.5))*vec2(0.3)));
	
	float nReflectFactor = min(col4.r+col4.g+col4.b,reflectFactor);
	
	col4*=vec4(min(max(abs(col3.r-0.5),abs(col3.g-0.5)),1.0)*2.0*nReflectFactor);
	
	//interpolate with nearby pixels to reduce artifacts
	col3+=texture2D(Texture2, vec2(gl_TexCoord[0])+vec2(0.0009765625));
	col3+=texture2D(Texture2, vec2(gl_TexCoord[0])+vec2(-0.0009765625));
	col3+=texture2D(Texture2, vec2(gl_TexCoord[0])+vec2(0.0009765625,-0.0009765625));
	col3+=texture2D(Texture2, vec2(gl_TexCoord[0])+vec2(-0.0009765625,0.0009765625));
	col3/=vec4(vec3(5.0),1.0);
	col3.rg-=vec2(0.5);
	col3.rg*=vec2(5.0);
	
	col3 = vec4(normalize(col3.xyz),1.0);
	
	float oneDivSqrtSix = 1.0/sqrt(6.0);
	float oneDivSqrtTwo = 1.0/sqrt(2.0);
	float oneDivSqrtThree = 1.0/sqrt(3.0);
	float sqrtTwoDivThree = sqrt(2.0/3.0);
	
	vec2 lightmapCoords = vec2(gl_TexCoord[1]);//-(vec2(col3.r-0.5,col3.g-0.5)*vec2(0.03));
	vec4 col2 = texture2D(Texture1, lightmapCoords)*vec4(vec3(dot(vec3(-oneDivSqrtSix,-oneDivSqrtTwo,oneDivSqrtThree),col3.xyz)),1.0);
	col2 += texture2D(Texture1_b, lightmapCoords)*vec4(vec3(dot(vec3(sqrtTwoDivThree,oneDivSqrtTwo,oneDivSqrtThree),col3.xyz)),1.0);
	col2 += texture2D(Texture1_t, lightmapCoords)*vec4(vec3(dot(vec3(-oneDivSqrtSix,0.0,oneDivSqrtThree),col3.xyz)),1.0);
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	col*=vec4(vec3(2.0),1.0);
	col*=vec4(1.0-(min(max(abs(col3.r-0.5),abs(col3.g-0.5)),1.0)*2.0*nReflectFactor));
	col *= col2;
	col+=col4;
	
    gl_FragColor = col2;//*vec4(abs(normal)*reflectFactor,1.0);
}