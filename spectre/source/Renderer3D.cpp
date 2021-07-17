#include "Renderer3D.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

namespace Spectre
{
	void Renderer3D::renderMesh(const TransformComponent& transform, const MeshComponent& mesh, const MeshRendererComponent& meshRenderer, const EditorCamera& camera, bool isOutline)
	{
		if (isOutline) {
			ResourceManager::getShader("solid")->use();
		}
		else {
			meshRenderer.shader->use();
		}

		meshRenderer.shader->setUniformVec3("u_LightPosition", camera.getPosition());
		meshRenderer.shader->setUniformVec3("u_MaterialDiffuse", meshRenderer.materialDiffuse);
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		if (isOutline) {
			model = glm::scale(model, transform.scale * glm::vec3(1.1f));
		}
		else {
			model = glm::scale(model, transform.scale);
		}
		
		if (isOutline) {
			ResourceManager::getShader("solid")->use();
			ResourceManager::getShader("solid")->setUniformMat4("u_Model", model);
		}
		else {
			meshRenderer.shader->use();
			meshRenderer.shader->setUniformMat4("u_Model", model);
		}
		
		glBindVertexArray(mesh.mesh.getVertexArray());
		glDrawElements(GL_TRIANGLES, mesh.mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}
