uniform mat4 uBone[70];

uniform mat4 mWorld;
uniform vec3 cameraPos;

varying mat3 TNTB;
varying vec3 pos;

varying vec4 distFromCenter;

attribute vec4 inBlendWeight;
attribute vec4 inBlendIndex;

attribute vec3 inTangent;
attribute vec3 inBinormal;

void main(void)
{
	
	vec4 Position = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 N = vec3(0.0,0.0,0.0);
	vec3 T = vec3(0.0,0.0,0.0);
	vec3 B = vec3(0.0,0.0,0.0);

	for (int i = 0; i < 4; ++i) {
		Position += vec4(uBone[int(inBlendIndex[i])] * gl_Vertex * inBlendWeight[i]);
		N += vec3(mat3(uBone[int(inBlendIndex[i])]) * gl_Normal * inBlendWeight[i]);
		T += vec3(mat3(uBone[int(inBlendIndex[i])]) * inTangent * inBlendWeight[i]);
		B += vec3(mat3(uBone[int(inBlendIndex[i])]) * inBinormal * inBlendWeight[i]);
	}
	
	pos = (mWorld*Position).xyz;
	TNTB[0] = mat3(mWorld)*N;
	TNTB[1] = mat3(mWorld)*T;
	TNTB[2] = mat3(mWorld)*B;
	
	gl_Position = gl_ModelViewProjectionMatrix * Position;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	distFromCenter = ((mWorld * Position)-vec4(cameraPos,1.0));
}
