#version 330 core


layout (location = 0) in vec3 aPos;


uniform mat4 model;
out vec3 outColor;
uniform mat4 view;
uniform mat4 projection;
out float yPos;

void main()
{
	vec3 newPos = aPos;

	newPos *= 5;
	//sets the position and color of the sun sphere
	gl_Position = projection * view * model * vec4(newPos.x, newPos.y, newPos.z, 1.0);
	outColor = vec3(1.0f, 0.8f, 0.0f);
	yPos = aPos.y;
}