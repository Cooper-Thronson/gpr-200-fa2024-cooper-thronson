#version 330 core
//I think this is done?

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;


out vec3 outColor;
out vec2 TexCoord;
out vec3 Normal;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 newPos = aPos;
	newPos *= 5;
	gl_Position = projection * view * vec4(newPos.x, newPos.y, newPos.z, 1.0);
	outColor = aColor;
	//outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	//TexCoord = aTexCoord;
}