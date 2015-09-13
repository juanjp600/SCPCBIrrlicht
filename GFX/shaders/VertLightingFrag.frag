uniform sampler2D Texture0; //base texture

uniform sampler2D fogTexture;

varying vec4 vertexColor;

varying vec4 distFromCenter;

uniform float fogNear;
uniform float fogFar;

void main (void)
{
	vec4 col = texture2D(Texture0, vec2(gl_TexCoord[0]));
	col*=vec4(vec3(2.0),1.0);
	col*=vertexColor;
	
	float fog = (sqrt(distFromCenter.x*distFromCenter.x+distFromCenter.y*distFromCenter.y+distFromCenter.z*distFromCenter.z)-fogNear);
	fog /= fogFar-fogNear;
	fog = clamp(1.0-fog,0.0,1.0);
	fog*=fog;
	
    gl_FragColor = (col*vec4(vec3(fog),1.0))+(texture2D(fogTexture,gl_FragCoord.xy/vec2(1280.0,720.0))*vec4(vec3(1.0-fog),1.0));
}