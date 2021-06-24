#pragma once

namespace Spectre
{
	class Texture
	{
	public:
		Texture(int width, int height, unsigned char* data);

		void bind() const;

		inline unsigned int getId() const { return m_Id; }

	private:
		unsigned int m_Id;
	};
}
