#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float scale;

//FragColor = texture(_Tex,UV * 5.0);

void main ()
{
	vec2 UVs;
	//mess with UV coords to loop
	UVs = TexCoord * scale;
	FragColor = vec4(1, 0);

}
