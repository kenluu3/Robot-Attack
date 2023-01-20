varying vec3 nrml;
varying vec3 eye;

void main()
{
	eye = vec3(gl_ModelViewMatrix * gl_Vertex);
	nrml = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}