varying vec3 nrml;
varying vec3 eye;
varying vec2 text;

uniform sampler2D tex;

void main()
{
    vec3 l = normalize(gl_LightSource[0].position.xyz - eye);
    vec3 v = normalize(-eye);
    vec3 r = normalize(-reflect(l, nrml));
    
    vec4 Ia = gl_FrontLightProduct[0].ambient;
    vec4 Id = gl_FrontLightProduct[0].diffuse * max(dot(nrml, l), 0.0);
    vec4 Is = gl_FrontLightProduct[0].specular * pow(max(dot(v, r), 0.0), gl_FrontMaterial.shininess);
    
    gl_FragColor = (Ia + Id + Is) * texture2D(tex, text);
}