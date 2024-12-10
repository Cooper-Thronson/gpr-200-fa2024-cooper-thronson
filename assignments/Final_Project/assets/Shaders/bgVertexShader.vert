#version 330 core
//I think this is done?

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
out vec3 outColor;
out vec2 TexCoord;
out vec3 Normal;
out float yPos;
uniform mat4 view;
uniform mat4 projection;
uniform float maxYPos;

void main()
{
	vec3 newPos = aPos;

	newPos *= 5;
	gl_Position = projection * view * model * vec4(newPos.x, newPos.y, newPos.z, 1.0);
	outColor = aColor;
	//outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	//TexCoord = aTexCoord;
	yPos = aPos.y / maxYPos;
}