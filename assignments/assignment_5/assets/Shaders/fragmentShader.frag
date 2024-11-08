#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float shininess;

in vec3 outColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D ourTexture;
uniform float ambientStrength;
uniform float diffStrength;


void main()
{
	vec3 viewDir = normalize(viewPos - WorldPos);
	
    vec3 ambient = ambientStrength * lightColor;
	
	vec4 texColor = texture(ourTexture, TexCoord);
	//if (texColor.a < 0.1)
		//discard;
	vec3 normal = normalize(Normal);

	vec3 lightDir = normalize(lightPos - WorldPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininess);


	vec3 specular = lightColor * spec;
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * diffStrength;

	vec3 result = (ambient + diffuse + specular) * vec3(texColor);
	
	FragColor = vec4(result, 1.0);

}