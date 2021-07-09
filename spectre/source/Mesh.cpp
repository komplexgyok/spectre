#include "Mesh.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

namespace Spectre
{
	Mesh::Mesh()
		: m_VertexArray(0), m_VertexBuffer(0), m_IndexBuffer(0)
	{
		// Cube
		m_Vertices.push_back({glm::vec3(-1.0f, -1.0f, 1.0f)});		// Left bottom front	(0)
		m_Vertices.push_back({glm::vec3(1.0f, -1.0f, 1.0f)});		// Right bottom front	(1)
		m_Vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f)});		// Right top front		(2)
		m_Vertices.push_back({glm::vec3(-1.0f, 1.0f, 1.0f)});		// Left top front		(3)
		m_Vertices.push_back({glm::vec3(-1.0f, -1.0f, -1.0f)});		// Left bottom back		(4)
		m_Vertices.push_back({glm::vec3(1.0f, -1.0f, -1.0f)});		// Right bottom back	(5)
		m_Vertices.push_back({glm::vec3(1.0f, 1.0f, -1.0f)});		// Right top back		(6)
		m_Vertices.push_back({glm::vec3(-1.0f, 1.0f, -1.0f)});		// Left top back		(7)

		// Front face
		m_Indices.push_back(0);
		m_Indices.push_back(1);
		m_Indices.push_back(2);

		m_Indices.push_back(2);
		m_Indices.push_back(3);
		m_Indices.push_back(0);

		// Back face
		m_Indices.push_back(5);
		m_Indices.push_back(4);
		m_Indices.push_back(7);

		m_Indices.push_back(7);
		m_Indices.push_back(6);
		m_Indices.push_back(5);

		// Left face
		m_Indices.push_back(4);
		m_Indices.push_back(0);
		m_Indices.push_back(3);

		m_Indices.push_back(3);
		m_Indices.push_back(7);
		m_Indices.push_back(4);

		// Right face
		m_Indices.push_back(1);
		m_Indices.push_back(5);
		m_Indices.push_back(6);

		m_Indices.push_back(6);
		m_Indices.push_back(2);
		m_Indices.push_back(1);

		// Top face
		m_Indices.push_back(3);
		m_Indices.push_back(2);
		m_Indices.push_back(6);

		m_Indices.push_back(6);
		m_Indices.push_back(7);
		m_Indices.push_back(3);

		// Bottom face
		m_Indices.push_back(4);
		m_Indices.push_back(5);
		m_Indices.push_back(1);

		m_Indices.push_back(1);
		m_Indices.push_back(0);
		m_Indices.push_back(4);

		// Vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex buffer
		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_DYNAMIC_DRAW);

		// Vertex buffer layout
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);
	}

	Mesh::Mesh(const std::string& filepath)
		: m_VertexArray(0), m_VertexBuffer(0), m_IndexBuffer(0)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		std::string line;
		std::string type;

		while (std::getline(file, line)) {
			std::stringstream stream(line);

			stream >> type;

			// Vertex positions
			if (type == "v") {
				float x, y, z;
				
				stream >> x >> y >> z;
				m_Vertices.push_back({glm::vec3(x, y, z)});
			}

			else if (type == "vn") {

			}

			// Indices
			else if (type == "f") {
				uint32_t index[3];
				char slashes[2];
				
				while (stream >> index[0] >> slashes[0] >> index[1] >> slashes[1] >> index[2]) {
					m_Indices.push_back(index[0] - 1);
				}
			}
		}

		// Vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex buffer
		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_DYNAMIC_DRAW);

		// Vertex buffer layout
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);
	}

	Mesh::~Mesh()
	{
		/*glDeleteBuffers(1, &m_IndexBuffer);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);*/
		std::cout << "Mesh destructor" << std::endl;
	}
}
