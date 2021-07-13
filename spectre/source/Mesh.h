#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Spectre
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 textureCoordinate;
		glm::vec3 normal;
	};
	
	class Mesh
	{
	public:
		Mesh(const std::string& filepath);
		~Mesh();

		inline uint32_t getVertexArray() const { return m_VertexArray; }
		inline uint32_t getIndexBuffer() const { return m_IndexBuffer; }
		inline uint32_t getIndexCount() const { return m_Indices.size(); }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		uint32_t m_VertexArray;
		uint32_t m_VertexBuffer;
		uint32_t m_IndexBuffer;
	};
}
