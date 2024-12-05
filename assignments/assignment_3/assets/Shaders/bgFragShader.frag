#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 outColor;
in vec3 Normal;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float scale;
uniform vec3 lightDir;

//FragColor = texture(_Tex,UV * 5.0);

void main ()
{
	vec2 UVs;
	//mess with UV coords to loop
	UVs = TexCoord * scale;
	FragColor = vec4(1);

}
