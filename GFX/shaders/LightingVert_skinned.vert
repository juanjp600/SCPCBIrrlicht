#define MAX_JOINT_NUM 80

uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightPos3;
uniform vec4 lightPos4;

varying mat4 olightPos;
varying vec3 v;
varying vec3 TN;
varying vec3 dacolor;

uniform mat4 JointTransform[MAX_JOINT_NUM];

mat3 toMat3(in mat4 inp) {
	return mat3(inp[0].xyz,inp[1].xyz,inp[2].xyz);
}

void main(void)
{
	dacolor = vec3(gl_Color.r,gl_Color.g,gl_Color.b);
	
	int verCol = int(gl_Color.r * 255.9);
	mat4 vertTran = JointTransform[verCol - 1];
	
	verCol = int(gl_Color.g * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];

	verCol = int(gl_Color.b * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];
		
	verCol = int(gl_Color.a * 255.9);
	if(verCol > 0)
		vertTran += JointTransform[verCol - 1];
	
	v = -vec3(gl_ModelViewMatrix * gl_Vertex);       
	vec3 N = gl_Normal;
	
	TN = normalize(gl_NormalMatrix*toMat3(vertTran)*N);
	
	olightPos[0] = vec4(lightPos1.xyz + v,lightPos1.w);
	olightPos[1] = vec4(lightPos2.xyz + v,lightPos1.w);
	olightPos[2] = vec4(lightPos3.xyz + v,lightPos1.w);
	olightPos[3] = vec4(lightPos4.xyz + v,lightPos1.w);
	
	mat4 ModelTransform = gl_ModelViewProjectionMatrix;
	ModelTransform *= vertTran;
	gl_Position = ModelTransform * gl_Vertex;
	
	v = normalize(v);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}