#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <ew/external/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <CThronson/Texture.h>
#include <CThronson/Shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const char* FRAG_SHADER_PATH = "assets/Shaders/fragmentShader.frag";
const char* VERT_SHADER_PATH = "assets/Shaders/vertexShader.vert";
const char* BG_VERT_SHADER_PATH = "assets/Shaders/bgVertexShader.vert";
const char* BG_FRAG_SHADER_PATH = "assets/Shaders/bgFragShader.frag";


glm::mat4 model = glm::mat4(1.0f);

glm::mat4 view = glm::mat4(1.0f);

glm::mat4 projection;



float vertices[] = {
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader myShader { VERT_SHADER_PATH, FRAG_SHADER_PATH };

	//Shader bgShader { BG_VERT_SHADER_PATH, BG_FRAG_SHADER_PATH };
	

	stbi_set_flip_vertically_on_load(true);

	Texture2D texture = Texture2D("assets/Textures/AwesomeGhost.png", GL_NEAREST, GL_CLAMP_TO_BORDER, GL_RGBA);
	

	

	//make the other 2 textures
	//Texture2D bgTexture1("assets/Textures/CoolGuy.png", GL_NEAREST, GL_REPEAT, GL_RGBA);
	//Texture2D bgTexture2("assets/Textures/wall.jpg", GL_NEAREST, GL_REPEAT, GL_RGB);

	model = glm::rotate(model, (float)glfwGetTime * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
		
		
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//Drawing Character
		myShader.use();

		texture.Bind(GL_TEXTURE2);
		myShader.setInt("ourTexture", 2);
		//texture.Bind();
		
		

		float timeValue = glfwGetTime();
		float blackValue = sin(timeValue) / 2.0f + 0.5f;

		//shader.cpp setfloat
		int vertexColorLocation = glGetUniformLocation(myShader.ID, "colorScale");
		glUniform1f(vertexColorLocation, blackValue);
		

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
