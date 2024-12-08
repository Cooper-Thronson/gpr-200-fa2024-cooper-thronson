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
	vec2 UVs;
	//mess with UV coords to loop
	UVs = TexCoord * scale;

	float delta = .75f * yPos + 0.25f;

	vec4 topColor = vec4(0.005f, 0.005f, 0.1f, 1.0f);
	vec4 bottomColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	if (delta > 0)
		FragColor = mix(bottomColor, topColor, clamp(pow(yPos, .2), 0, 1));
	else
		FragColor = vec4(0);
}
