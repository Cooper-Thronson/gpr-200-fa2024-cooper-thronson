#include "Texture.h"
#include <iostream>
//#include <GLFW/glfw3.h>
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"


Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode, int format)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	

}

Texture2D::~Texture2D()
{

}

void Texture2D::Bind(unsigned int slot)
{

	glActiveTexture(slot);

	glBindTexture(GL_TEXTURE_2D, texture);

	
	return;
}