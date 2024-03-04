#pragma once
#include "common.h"
#include <graphics/Texture.h>
#include <graphics/VertexArray.h>
#include <graphics/VertexBuffer.h>
#include <graphics/VertexBufferLayout.h>

namespace CMEngine {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        //// tangent
        //glm::vec3 Tangent;
        //// bitangent
        //glm::vec3 Bitangent;
    };

    class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(const Renderer& renderer, Shader& shader);
    private:
        //  render data
        VertexArray VAO;
        VertexBuffer VBO;
        IndexBuffer EBO;

        void setupMesh();
    };
}