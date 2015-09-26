uniform vec2 screenDimensions;

uniform vec3 cameraPos;
uniform vec4 lightPos;
uniform vec4 lightColor;

uniform sampler2D specularFactor;
uniform sampler2D positionInWorld;
uniform sampler2D normalInWorld;

float getLengthSQR (vec3 vec) {
	return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}

void main(void)
{
	vec4 posInWorld = texture2D(positionInWorld,gl_FragCoord.xy/vec2(1280.0,720.0));
	vec4 normInWorld = texture2D(normalInWorld,gl_FragCoord.xy/vec2(1280.0,720.0));
	vec3 lightDir = normalize(lightPos.xyz-posInWorld.xyz);
	vec4 light = vec4(max(dot(normInWorld.xyz,lightDir),0.0));
	light*=light;
	vec4 specular = vec4(pow(max(dot(reflect(lightDir,normInWorld.xyz),normalize(posInWorld.xyz-cameraPos)),0.0),20.0));
	specular*=texture2D(specularFactor,gl_FragCoord.xy/vec2(1280.0,720.0));
	vec4 distFactor=vec4(max(1.0-(getLengthSQR(lightPos.xyz-posInWorld.xyz)/lightPos.w),0.0));
	distFactor*=distFactor;
	light*=distFactor;
	specular*=distFactor;
	gl_FragData[0] = light*lightColor;
	gl_FragData[1] = specular*lightColor;
}