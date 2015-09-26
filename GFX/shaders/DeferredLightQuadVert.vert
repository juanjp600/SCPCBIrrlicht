void main(void)
{
	vec4 Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	Position.z = 0.3;
	gl_Position = Position;
}