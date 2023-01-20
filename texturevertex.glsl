varying vec3 nrml;
varying vec3 eye;
varying vec2 text;

void main()
{
	eye = vec3(gl_ModelViewMatrix * gl_Vertex);
	nrml = normalize(gl_NormalMatrix * gl_Normal);

	text = 0.05 * (gl_Vertex.xz + vec2(0, gl_Vertex.y)); 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}