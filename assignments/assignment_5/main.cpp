#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <ew/external/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#include <CThronson/Texture.h>
#include <CThronson/Shader.h>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const char* FRAG_SHADER_PATH = "assets/Shaders/fragmentShader.frag";
const char* VERT_SHADER_PATH = "assets/Shaders/vertexShader.vert";
const char* BG_VERT_SHADER_PATH = "assets/Shaders/bgVertexShader.vert";
const char* BG_FRAG_SHADER_PATH = "assets/Shaders/bgFragShader.frag";
const char* LB_FRAG_SHADER_PATH = "assets/Shaders/lightbulbShader.frag";
const char* LB_VERT_SHADER_PATH = "assets/Shaders/lightbulbShader.vert";

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool mouseLock;


glm::vec3 lightPosition;
glm::vec3 lightColor;
float ambientK;
float diffuseK;
float shininess;
float specularK;


glm::mat4 model = glm::mat4(1.0f);


glm::mat4 projection;
//camera vecs
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirecton = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirecton));
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400;
float lastY = 300;
bool firstMouse;
float fov = 45.0f;

glm::vec3 direction;


const float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
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
	const float cameraSpeed = 2.5f * deltaTime;
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

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal coords
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	Shader myShader { VERT_SHADER_PATH, FRAG_SHADER_PATH };
	Shader bulbShader{ LB_VERT_SHADER_PATH, LB_FRAG_SHADER_PATH };



	//Shader bgShader { BG_VERT_SHADER_PATH, BG_FRAG_SHADER_PATH };
	

	stbi_set_flip_vertically_on_load(true);

	Texture2D texture = Texture2D("assets/Textures/AwesomeGhost.png", GL_NEAREST, GL_CLAMP_TO_BORDER, GL_RGBA);
	
	glEnable(GL_DEPTH_TEST);

	//make the other 2 textures
	//Texture2D bgTexture1("assets/Textures/CoolGuy.png", GL_NEAREST, GL_REPEAT, GL_RGBA);
	//Texture2D bgTexture2("assets/Textures/wall.jpg", GL_NEAREST, GL_REPEAT, GL_RGB);

	//glm translation stuff
	
	glm::mat4 view = glm::mat4(1.0f);
	

	//glm location things
	
	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));


	

	glEnable(GL_DEPTH_TEST);
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));


	

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			mouseLock = true;
		}
		else {
			mouseLock = false;
		}

		if (mouseLock) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}



		//Drawing Character
		myShader.use();
		

		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);
		myShader.setFloat("shininess", shininess);
		//CHANGE THESE VEC 3S
		myShader.setFloat("diffStrength", diffuseK);
		myShader.setFloat("ambientStrength", ambientK);
		myShader.setVec3("lightPos", lightPosition);
		myShader.setVec3("lightColor", lightColor);
		myShader.setVec3("viewPos", cameraPos);


		glBindVertexArray(VAO);

		texture.Bind(GL_TEXTURE2);
		myShader.setInt("ourTexture", 2);
		
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(i/5, i/5, i/5));
			myShader.setMat4("model", model);

			//draw arrays?
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		bulbShader.use();

		bulbShader.setMat4("projection", projection);
		bulbShader.setMat4("view", view);
		//IMGUI
		
		bulbShader.setVec3("aColor", lightColor);
		
		glm::mat4 model = glm::mat4(1.0f);
		//POS FROM IMGUI
		model = glm::translate(model, lightPosition);

		bulbShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");
		{
			ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
			ImGui::ColorEdit3("Light Color", &lightColor.r);
			ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);
		}
		ImGui::End();
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
	glfwTerminate();

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseLock == false)
	{
		return;
	}
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}