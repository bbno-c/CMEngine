#pragma once
#include "common.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

namespace CMEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;

        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    private:
        uint32_t rendererID;
    };


}
