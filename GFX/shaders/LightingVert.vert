uniform mat4 uBone[70];

uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightPos3;
uniform vec4 lightPos4;

uniform mat4 mWorld;
uniform vec3 cameraPos;

varying mat4 olightPos;
varying vec3 v;
varying vec3 TN;

varying vec4 distFromCenter;

attribute vec4 inBlendWeight;
attribute vec4 inBlendIndex;

void main(void)
{
	vec4 Position = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 N = vec3(0.0,0.0,0.0);

	for (int i = 0; i < 4; ++i) {
		Position += vec4(uBone[int(inBlendIndex[i])] * gl_Vertex * inBlendWeight[i]);
		N += vec3(mat3(uBone[int(inBlendIndex[i])]) * gl_Normal * inBlendWeight[i]);
	}
	
	v = -vec3(gl_ModelViewMatrix * Position);       
	//vec3 N = gl_Normal;
	
	TN = normalize(gl_NormalMatrix*N);
	
	olightPos[0] = vec4(lightPos1.xyz + v,lightPos1.w);
	olightPos[1] = vec4(lightPos2.xyz + v,lightPos1.w);
	olightPos[2] = vec4(lightPos3.xyz + v,lightPos1.w);
	olightPos[3] = vec4(lightPos4.xyz + v,lightPos1.w);
	
	gl_Position = gl_ModelViewProjectionMatrix * Position;
	
	v = normalize(v);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	distFromCenter = ((mWorld * Position)-vec4(cameraPos,1.0));
}