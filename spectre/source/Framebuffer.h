#pragma once

#include <cstdint>

namespace Spectre
{
	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

		void create(int32_t width, int32_t height);
		void recreate(int32_t width, int32_t height);

		void bind() const;
		void unbind() const;

		inline uint32_t getColorAttachment() const { return m_ColorAttachment; }
		inline int32_t getWidth() const { return m_Width; }
		inline int32_t getHeight() const { return m_Height; }

	private:
		uint32_t m_RendererId;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;

		int32_t m_Width;
		int32_t m_Height;
	};
}
