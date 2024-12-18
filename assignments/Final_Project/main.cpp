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

#include <ew/mesh.h>
#include <ew/procGen.h>
//The baseline code for this was all Cooper's. additions include skydome and water mesh, skydome parts done by cooper, water parts done by isa.

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

//all of these shaders done by cooper, water shaders done by isa.
const char* FRAG_SHADER_PATH = "assets/Shaders/fragmentShader.frag";
const char* VERT_SHADER_PATH = "assets/Shaders/vertexShader.vert";
const char* BG_VERT_SHADER_PATH = "assets/Shaders/bgVertexShader.vert";
const char* BG_FRAG_SHADER_PATH = "assets/Shaders/bgFragShader.frag";
const char* LB_FRAG_SHADER_PATH = "assets/Shaders/lightbulbShader.frag";
const char* LB_VERT_SHADER_PATH = "assets/Shaders/lightbulbShader.vert";
const char* SUN_VERT_SHADER_PATH = "assets/Shaders/sunVertexShader.vert";
const char* SUN_FRAG_SHADER_PATH = "assets/Shaders/sunFragShader.frag";

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 model = glm::mat4(1.0f);

glm::vec3 lightPosition = glm::vec3(20.7f, 0.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 0.45f, 0.0f);
float ambientK;
float diffuseK;
float shininess = 64.0f;
float specularK;



glm::mat4 projection;
//camera vecs
glm::vec3 cameraPos = glm::vec3(20.0f, 3.0f, 3.0f);
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
bool mouseLock;
float fov = 45.0f;

glm::vec3 direction;


const float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;

bool waterWireFrame = false;
bool pointRender = false;

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

//setting up sphere vertexes for generation
struct vertex {
	glm::vec3 pos;
	//glm::vec3 normal;
	//glm::vec2 UV
};

//making the two vectors of vertices and indices, and then setting up variables in relation to pi for the pitch and yaw of all of the points.
std::vector<vertex> sphereVertices;
std::vector<unsigned int> sphereIndices;
unsigned int subdivs = 64;
float angleStep = glm::two_pi<float>() / (float)subdivs;
float heightStep = glm::pi<float>() / (float)subdivs;
float phi = -glm::half_pi<float>();
float y = glm::sin(phi);
int row = 0;

//camera movement code
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// normal coords
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader myShader { VERT_SHADER_PATH, FRAG_SHADER_PATH }; // cooper

	Shader bulbShader{ LB_VERT_SHADER_PATH, LB_FRAG_SHADER_PATH }; // cooper

	Shader waterShader("assets/Shaders/waterVertexShader.vert", "assets/Shaders/waterFragmentShader.frag"); //water shader (isa)

	Shader sunShader{ SUN_VERT_SHADER_PATH, SUN_FRAG_SHADER_PATH }; // cooper

	Shader bgShader { BG_VERT_SHADER_PATH, BG_FRAG_SHADER_PATH }; // cooper
	

	stbi_set_flip_vertically_on_load(true);

	Texture2D texture = Texture2D("assets/Textures/AwesomeGhost.png", GL_NEAREST, GL_CLAMP_TO_BORDER, GL_RGBA);
	
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);;

	// water plane initialization (isa)
	ew::MeshData waterMeshData;
	ew::createPlaneXY(50.0f, 50.0f, 55, &waterMeshData);
	ew::Mesh waterMesh = ew::Mesh(waterMeshData);

	//glm view.
	
	glm::mat4 view = glm::mat4(1.0f);
	

	//glm view model and projection, from previous assignment.
	
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

	//skysphere radius.
	float radius = 1000.0f;

	//nested for loop for the vertices. both of these use the row, heightstep, and anglestep variables to rotate around the circle at each distance, creating rings of indices and vertices.
	for (int row = 0; row <= subdivs; row++)
	{
		float ph = heightStep * row;
		for (int col = 0; col <= subdivs; col++)
		{
			float theta = angleStep * col;
			vertex v;
			v.pos.y = cos(ph);
			v.pos.x = cos(theta) * sin(ph);
			v.pos.z = sin(theta) * sin(ph);
			v.pos *= radius;
			sphereVertices.push_back(v);
		}
	}
	//indices loop. this checks which indices need to connect to which points, keeping relativity to the quad that is bein made. points are pushed back twice as they overlap as indices.
	for (int row = 0; row < subdivs; row++)
	{
		for (int col = 0; col < subdivs; col++)
		{
			int tl = row * (subdivs + 1) + col;
			int tr = tl + 1;
			int bl = tl + subdivs + 1;
			int br = bl + 1;

			sphereIndices.push_back(bl);
			sphereIndices.push_back(br);
			sphereIndices.push_back(tr);


			sphereIndices.push_back(tr);
			sphereIndices.push_back(tl);
			sphereIndices.push_back(bl);


		}
	}


	//SPHERES
	//vertex array object
	unsigned int sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);

	//vertex buffer object
	unsigned int sphereVBO;
	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sphereVertices.size(), sphereVertices.data(), GL_STATIC_DRAW);

	unsigned int sphereEBO;
	glGenBuffers(1, &sphereEBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sphereIndices.size(), sphereIndices.data(), GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//time variable for waves
		float time = (float)glfwGetTime();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 10000.0f);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		myShader.setMat4("projection", projection);
		myShader.setMat4("view", view);

		ew::DrawMode drawMode = pointRender ? ew::DrawMode::POINTS : ew::DrawMode::TRIANGLES; //set draw mode for water draw call (isa)

		//water shenanigans (courtesy of Isa)
		waterShader.use();

		//set uniforms
		int viewLocWater = glGetUniformLocation(waterShader.getID(), "view");
		glUniformMatrix4fv(viewLocWater, 1, GL_FALSE, glm::value_ptr(view));

		int projLocWater = glGetUniformLocation(waterShader.getID(), "projection");
		glUniformMatrix4fv(projLocWater, 1, GL_FALSE, glm::value_ptr(projection));

		waterShader.setVec3("lightPos", lightPosition);
		waterShader.setVec3("lightColor", lightColor);
		waterShader.setVec3("viewPos", cameraPos);
		waterShader.setFloat("time", time);
		waterShader.setFloat("shiny", shininess);

		//more plane tomfoolery
		glm::mat4 planeTransform = glm::mat4(1);
		planeTransform = glm::rotate(planeTransform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		planeTransform = glm::translate(planeTransform, glm::vec3(-5.0, -5.0, 0.0));
		waterShader.setMat4("model", planeTransform);
		//draw call
		waterMesh.draw(drawMode);
		//end water shenanigans

		//draw sphere
		bgShader.use();
		bgShader.setMat4("projection", projection);
		bgShader.setMat4("view", view);
		bgShader.setFloat("maxYPos", radius);
		glm::mat4 model = glm::mat4(1.0f);
		glPointSize(4.0);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_CULL_FACE, GL_FILL);
		glBindVertexArray(sphereVAO);
		bgShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
		//create sun
		sunShader.use();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		glDisable(GL_CULL_FACE);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(0.0015f, 0.0015f, 0.0015f));
		sunShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			mouseLock = true;
		}
		else {
			mouseLock = false;
		}

		if (mouseLock) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		bulbShader.use();

		bulbShader.setMat4("projection", projection);
		bulbShader.setMat4("view", view);
		//IMGUI

		bulbShader.setVec3("aColor", lightColor);

		//POS FROM IMGUI
		model = glm::translate(model, lightPosition);

		bulbShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		
		
		
		//imGUI settings
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");
		
		ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);
		
		if (ImGui::Checkbox("Wireframe", &waterWireFrame)) {
			glPolygonMode(GL_FRONT_AND_BACK, waterWireFrame ? GL_LINE : GL_FILL);
		}
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
	glfwTerminate();

}

//handles mouse postions and movement
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
//handles zoom.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}