#include "common.h"
#include "VertexArray.h"

namespace CMEngine {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray() {
        if (m_RendererID != 0) {
            glDeleteVertexArrays(1, &m_RendererID);
        }
    }
    void VertexArray::Bind() const {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
        Bind();
        vb.Bind();
        const auto& elements = layout.GetElements();
        uint32_t offset = 0;

        for (size_t i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized,
                layout.GetStride(), (const void*)offset);
            offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
        }
        vb.Unbind();
    }


}
