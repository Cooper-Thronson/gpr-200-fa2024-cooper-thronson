#version 330 core
out vec4 FragColor;
in float yPos;
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

	//mixing colors based on location
	vec4 topColor = vec4(0.005f, 0.005f, 0.1f, 1.0f);
	vec4 bottomColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//gradient
	FragColor = mix(bottomColor, topColor, yPos + 0.4f);
	
}
