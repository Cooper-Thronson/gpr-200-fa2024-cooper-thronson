#version 330 core
//I think this is done?

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;


out vec3 outColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	outColor = aColor;
	//outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	TexCoord = aTexCoord;
}