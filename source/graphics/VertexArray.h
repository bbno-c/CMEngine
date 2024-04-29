#pragma once
#include "common.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

namespace CMEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& other) noexcept : m_RendererID{ std::exchange(other.m_RendererID, 0) } {}
        VertexArray& operator=(VertexArray&& other) noexcept
        {
            m_RendererID = std::exchange(other.m_RendererID, 0);
            return *this;
        }

        void Bind() const;

        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    private:
        uint32_t m_RendererID;
    };


}
