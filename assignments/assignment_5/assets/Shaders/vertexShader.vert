#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 WorldPos;

//out vec3 outColor;
out vec2 TexCoord;

uniform float colorScale;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//outColor = aColor;
	//outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	TexCoord = aTexCoord;
	// outcolor = aColor * colorScale also works apparently
	WorldPos = vec3(model * vec4(aPos, 1.0));
	Normal = transpose(inverse(mat3(model))) * aNormal;
}
