#include "Renderer3D.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Spectre
{
	void Renderer3D::renderMesh(const TransformComponent& transform, const Mesh& mesh, std::shared_ptr<Shader> shader)
	{
		shader->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, transform.scale);
		shader->setUniformMat4("u_Model", model);
		
		glBindVertexArray(mesh.getVertexArray());
		glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}
