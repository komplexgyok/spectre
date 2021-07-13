#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace Spectre
{
	Mesh::Mesh(const std::string& filepath)
		: m_VertexArray(0), m_VertexBuffer(0), m_IndexBuffer(0)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		std::string line;
		std::string type;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> textureCoordinates;
		std::vector<glm::vec3> normals;
		uint32_t currentIndex = 0;

		while (std::getline(file, line)) {
			std::stringstream stream(line);

			stream >> type;

			// Vertex positions
			if (type == "v") {
				float x, y, z;
				
				stream >> x >> y >> z;
				positions.push_back(glm::vec3(x, y, z));
			}

			// Texture coordinates
			else if (type == "vt") {
				float s, t;

				stream >> s >> t;
				textureCoordinates.push_back(glm::vec2(s, t));
			}

			// Vertex normals
			else if (type == "vn") {
				float x, y, z;

				stream >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}

			// Indices
			else if (type == "f") {
				uint32_t index[3];
				char slashes[2];
				
				while (stream >> index[0] >> slashes[0] >> index[1] >> slashes[1] >> index[2]) {
					m_Vertices.push_back({ positions[index[0] - 1], textureCoordinates[index[1] - 1] , normals[index[2] - 1] });
					m_Indices.push_back(currentIndex++);
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

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
