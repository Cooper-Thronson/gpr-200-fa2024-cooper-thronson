//courtesy of Isa
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord; //Varying
out vec3 Normal;
out vec3 FragPos;
out vec3 ViewSpacePos;
out mat4 InvView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

const float waveLen = 5;
const float waveAmp = 0.75;

void main()
{
   vec4 pos = vec4(aPos, 1.0);

   //generate wave offsets
   float radiansX = (pos.x / waveLen + (time/2.0)) * 2.0 * 3.141592;
   float radiansZ = (pos.y / waveLen + (time/2.0)) * 2.0 * 3.141592;
   //apply wave offsets
   pos.x += waveAmp * 0.5 * (sin(radiansZ) + cos(radiansX));
   pos.y += waveAmp * 0.5 * (sin(radiansZ) + cos(radiansX));
   pos.z += waveAmp * 0.5 * (sin(radiansZ) + cos(radiansX));

   ViewSpacePos = (view * model * pos).xyz;
   InvView = inverse(view);
   gl_Position = projection * view * model * pos;
   TexCoord = aTexCoord; //Pass-through
   FragPos = vec3(model * pos);
   Normal =mat3(transpose(inverse(model))) * aNormal;
}