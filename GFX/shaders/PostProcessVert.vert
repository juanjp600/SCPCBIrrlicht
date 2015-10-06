varying vec4 tcoords;

void main(void)
{
	vec4 vertex = gl_Vertex;
	
	vertex*=vec4(2.0,2.0,1.0,1.0);
	vertex+=vec4(-1.0,-1.0,0.0,0.0);
	vertex.z = 0.0;
	
	tcoords = gl_Vertex;
	gl_Position = vertex;
}
