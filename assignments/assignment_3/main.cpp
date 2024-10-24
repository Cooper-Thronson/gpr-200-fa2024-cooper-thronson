#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <ew/external/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CThronson/Texture.h>
#include <CThronson/Shader.h>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const char* FRAG_SHADER_PATH = "assets/Shaders/fragmentShader.frag";
const char* VERT_SHADER_PATH = "assets/Shaders/vertexShader.vert";
const char* BG_VERT_SHADER_PATH = "assets/Shaders/bgVertexShader.vert";
const char* BG_FRAG_SHADER_PATH = "assets/Shaders/bgFragShader.frag";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 model = glm::mat4(1.0f);

glm::mat4 view = glm::mat4(1.0f);

glm::mat4 projection;
//camera vecs
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirecton = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirecton));
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);

const float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;






float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

float textCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f),

	glm::vec3(0.0f,  3.0f,  -2.0f),
	glm::vec3(4.0f,  1.0f, -5.0f),
	glm::vec3(-1.5f, -3.7f, 5.5f),
	glm::vec3(4.8f, 2.0f, -9.3f),
	glm::vec3(3.4f, 2.4f, -5.5f),
	glm::vec3(-7.7f,  7.0f, -2.5f),
	glm::vec3(4.3f, -5.0f, -2.5f),
	glm::vec3(5.5f,  5.0f, -2.5f),
	glm::vec3(6.5f,  6.2f, -3.5f),
	glm::vec3(5.3f,  -4.0f, -2.5f)
};



void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	//vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader myShader { VERT_SHADER_PATH, FRAG_SHADER_PATH };

	//Shader bgShader { BG_VERT_SHADER_PATH, BG_FRAG_SHADER_PATH };
	

	stbi_set_flip_vertically_on_load(true);

	Texture2D texture = Texture2D("assets/Textures/AwesomeGhost.png", GL_NEAREST, GL_CLAMP_TO_BORDER, GL_RGBA);
	
	glEnable(GL_DEPTH_TEST);
	

	//make the other 2 textures
	//Texture2D bgTexture1("assets/Textures/CoolGuy.png", GL_NEAREST, GL_REPEAT, GL_RGBA);
	//Texture2D bgTexture2("assets/Textures/wall.jpg", GL_NEAREST, GL_REPEAT, GL_RGB);

	//glm translation stuff
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//glm location things
	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		//Drawing BG
		bgShader.use();
		//i think this code should work after i make the textures?
		
		bgShader.setInt("texture1", 0);
		bgShader.setInt("texture2", 1);
		bgShader.setFloat("scale", 50);
		bgTexture1.Bind(GL_TEXTURE0);
		bgTexture2.Bind(GL_TEXTURE1);
		*/

		glBindVertexArray(VAO);



		//Drawing Character
		myShader.use();

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		texture.Bind(GL_TEXTURE2);
		myShader.setInt("ourTexture", 2);
		
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(i, i, i));
			myShader.setMat4("model", model);

			//draw arrays?
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		

		
		//texture.Bind();
		
		

		//float timeValue = glfwGetTime();
		//float blackValue = sin(timeValue) / 2.0f + 0.5f;

		//shader.cpp setfloat
		//int vertexColorLocation = glGetUniformLocation(myShader.ID, "colorScale");
		//glUniform1f(vertexColorLocation, blackValue);
		

		

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");


}
