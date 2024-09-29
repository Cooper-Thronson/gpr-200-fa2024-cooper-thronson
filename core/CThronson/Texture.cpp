#include "Texture.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"


Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode)
{
	glGenTextures(1, &texture);
	

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

}

Texture2D::~Texture2D()
{

}

void Texture2D::Bind(unsigned int slot = 0)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	return;
}