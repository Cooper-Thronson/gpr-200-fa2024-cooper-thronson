#version 330 core
out vec4 FragColor;
in vec3 outColor;
in float yPos;


uniform float scale;
uniform vec3 lightDir;



void main ()
{
	
	FragColor = vec4(outColor, 0.1f);
	
}