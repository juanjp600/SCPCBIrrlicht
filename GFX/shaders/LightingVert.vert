uniform mat4 uBone[85];

uniform mat4 mWorld;
uniform vec3 cameraPos;

varying vec3 TN;
varying vec3 pos;

varying vec4 distFromCenter;

attribute vec4 inBlendWeight;
attribute vec4 inBlendIndex;

mat3 toMat3(in mat4 inp) {
	return mat3(inp[0].xyz,inp[1].xyz,inp[2].xyz);
}

void main(void)
{
	vec4 Position = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 N = vec3(0.0,0.0,0.0);

	for (int i = 0; i < 4; ++i) {
		Position += vec4(uBone[int(inBlendIndex[i])] * gl_Vertex * inBlendWeight[i]);
		N += vec3(toMat3(uBone[int(inBlendIndex[i])]) * gl_Normal * inBlendWeight[i]);
	}
	    
	//vec3 N = gl_Normal;
	
	pos = (mWorld*Position).xyz;
	TN = toMat3(mWorld)*N;
	
	gl_Position = gl_ModelViewProjectionMatrix * Position;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	distFromCenter = ((mWorld * Position)-vec4(cameraPos,1.0));
}