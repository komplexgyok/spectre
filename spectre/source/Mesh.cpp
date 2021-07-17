#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace Spectre
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices/*, std::vector<TextureM> textures*/)
		: m_Vertices(vertices), m_Indices(indices)//, m_Textures(textures)
		, m_VertexArray(0), m_VertexBuffer(0), m_IndexBuffer(0)
	{
		// Vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex buffer
		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_DYNAMIC_DRAW);

		// Vertex buffer layout
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		/*glDeleteBuffers(1, &m_IndexBuffer);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);*/
		std::cout << "Mesh destructor" << std::endl;
	}
}
