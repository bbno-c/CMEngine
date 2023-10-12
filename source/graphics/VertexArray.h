#pragma once
#include "common.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

namespace CMEngine {

    class VertexArray {
    public:
        VertexArray() {
            GL_CALL(glGenVertexArrays(1, &rendererID));
        }

        ~VertexArray() {
            GL_CALL(glDeleteVertexArrays(1, &rendererID));
        }

        void Bind() const {
            GL_CALL(glBindVertexArray(rendererID));
        }

        void Unbind() const {
            GL_CALL(glBindVertexArray(0));
        }

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
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
        }

    private:
        uint32_t rendererID;
    };


}
