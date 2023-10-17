#include "common.h"
#include "VertexArray.h"

namespace CMEngine {

    VertexArray::VertexArray() {
        GL_CALL(glGenVertexArrays(1, &rendererID));
    }

    VertexArray::~VertexArray() {
        GL_CALL(glDeleteVertexArrays(1, &rendererID));
    }

    void VertexArray::Bind() const {
        GL_CALL(glBindVertexArray(rendererID));
    }

    void VertexArray::Unbind() const {
        GL_CALL(glBindVertexArray(0));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
        Bind();
        vb.Bind();
        const auto& elements = layout.GetElements();
        uint32_t offset = 0;

        for (size_t i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            GL_CALL(glEnableVertexAttribArray(i));
            GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                layout.GetStride(), (const void*)offset));
            offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
        }
        vb.Unbind();
    }


}
