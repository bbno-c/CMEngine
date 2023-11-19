#include "common.h"
#include <graphics/Renderer.h>

namespace CMEngine {
	void Renderer::Clear() const
	{
		(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	}

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
		shader.Bind();
		va.Bind();
		ib.Bind();

		(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
	}

}