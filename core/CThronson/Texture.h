#pragma once

class Texture2D {

public:
	Texture2D(const char* filePath, int filterMode, int wrapMode, int format);
	~Texture2D();
	void Bind(unsigned int slot = 0);

private: 
	unsigned int m_id;
	int m_width, m_height;
	unsigned int texture;

};