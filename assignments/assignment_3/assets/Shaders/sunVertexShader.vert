#version 330 core
//I think this is done?

layout (location = 0) in vec3 aPos;
//layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
out vec3 outColor;
uniform mat4 view;
uniform mat4 projection;
out float yPos;

void main()
{
	vec3 newPos = aPos;

	newPos *= 5;
	gl_Position = projection * view * model * vec4(newPos.x, newPos.y, newPos.z, 1.0);
	outColor = vec3(0.7f, 0.45f, 0.0f);
	//outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	//TexCoord = aTexCoord;
	yPos = aPos.y;
}