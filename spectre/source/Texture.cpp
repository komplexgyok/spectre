#include "Texture.h"

#include <glad/glad.h>

namespace Spectre
{
	Texture::Texture(int width, int height, unsigned char* data)
	{
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}
}
