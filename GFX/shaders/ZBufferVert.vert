uniform mat4 uBone[70];

varying vec4 coords;

attribute vec4 inBlendWeight;
attribute vec4 inBlendIndex;

void main(void)
{
	coords = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 4; ++i) {
		coords += vec4(uBone[int(inBlendIndex[i])] * gl_Vertex * inBlendWeight[i]);
	}
	
	coords = gl_ModelViewProjectionMatrix * coords;
	
	gl_Position = coords;
}
