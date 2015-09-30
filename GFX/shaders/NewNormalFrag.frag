varying mat3 TNTB;
varying vec3 pos;

varying vec4 distFromCenter;

uniform sampler2D baseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D diffuseLight;
uniform sampler2D specularLight;
uniform sampler2D fogTexture;

uniform float fogNear;
uniform float fogFar;

uniform float renderSpecularFactor;

void main(void)
{
	vec3 distNormal = texture2D( normalMap, gl_TexCoord[0].xy ).xyz;
	
	distNormal.xy*=vec2(2.0);
	distNormal.xy-=vec2(1.0);
	
	distNormal*=vec3(4.0);
	distNormal = normalize(vec3(distNormal.z)*TNTB[0] + vec3(distNormal.x)*TNTB[1] + vec3(distNormal.y)*TNTB[2]);
	
	vec4 finalImage = vec4(texture2D( baseMap, gl_TexCoord[0].xy ).xyz,1.0);
	finalImage*=vec4(texture2D(diffuseLight,gl_FragCoord.xy/vec2(1280.0,720.0)).xyz,1.0);
	finalImage+=vec4(texture2D(specularLight,gl_FragCoord.xy/vec2(1280.0,720.0)).xyz,1.0);
	gl_FragData[0] = (finalImage*vec4(1.0-renderSpecularFactor))+vec4(texture2D( specularMap, gl_TexCoord[0].xy ).xyz*vec3(renderSpecularFactor*3.0),1.0);
	gl_FragData[1] = vec4(distNormal,1.0);
	gl_FragData[2] = vec4(pos,1.0);
}
