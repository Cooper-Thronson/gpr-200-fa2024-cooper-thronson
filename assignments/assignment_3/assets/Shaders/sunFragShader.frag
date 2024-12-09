#version 330 core
out vec4 FragColor;
in vec3 outColor;
in float yPos;


uniform float scale;
uniform vec3 lightDir;



void main ()
{
	float delta = .75f * yPos + 0.25f;
	
	FragColor = mix(vec4(outColor, 1), vec4(0.0f, 0.0f, 0.0f, 1), clamp(pow(yPos, .2), 0, 1));
	
}