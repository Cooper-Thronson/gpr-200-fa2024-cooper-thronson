#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

//FragColor = texture(_Tex,UV * 5.0);
//somethin here idk

void main ()
{
	//mess with UV coords to loop

	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

}
