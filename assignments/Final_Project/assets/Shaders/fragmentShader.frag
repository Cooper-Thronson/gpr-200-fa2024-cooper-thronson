#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;

in vec3 outColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D ourTexture;


void main()
{
	
	vec4 texColor = texture(ourTexture, TexCoord);
	//if (texColor.a < 0.1)
		//discard;
	vec3 normal = normalize(Normal);

	FragColor = texColor;

	FragColor = vec4(abs(normal), 1.0);

}