#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 ViewSpacePos;
in mat4 InvView;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    
    vec2 uv = TexCoord;

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * lightColor;
    
    vec3 objectColor = vec3(0.51, 0.75, 0.85);

    //recalculate normals for lighting in view space
    vec3 xTangent = dFdx(ViewSpacePos);
    vec3 yTangent = dFdy(ViewSpacePos);
    vec3 norm = normalize(cross(xTangent, yTangent));
    //translate the new normals back into world space
    norm = (InvView * vec4(norm, 0.0)).xyz;

    //cool vectors
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 reflectDir = reflect(-lightDir, norm); 

    //specular lighting
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = lightColor * spec; 

    //diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}