#pragma once

#include "Scene.h"

namespace Spectre
{
	class HierarchyPanel
	{
	public:
		HierarchyPanel();

		void onImGuiRender();

		inline uint32_t getSelected() const { return m_Selected; }
		
		void setScene(const std::shared_ptr<Scene>& scene);

	private:
		std::shared_ptr<Scene> m_Scene;
		uint32_t m_Selected;
	};
}
