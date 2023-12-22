#pragma once
#include <graphics/Shader.h>
#include <graphics/VertexArray.h>
#include <graphics/IndexBuffer.h>

namespace CMEngine {

	class Renderer {
	public:
		void Clear() const;
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		void Draw(const VertexArray& va, const int count, const Shader& shader) const;
	private:

	};
}