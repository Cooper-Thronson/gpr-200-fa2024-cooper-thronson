#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 outColor;

uniform float colorScale;

void main()
{
	gl_Position = vec4(aPos.x * colorScale, aPos.y + colorScale, aPos.z, 1.0);
	outColor = mix(aColor, vec3(0.0, 0.0, 0.0), colorScale);
	// outcolor = aColor * colorScale also works apparently
}
