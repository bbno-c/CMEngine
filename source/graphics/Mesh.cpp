#include "Mesh.h"

namespace CMEngine {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void Mesh::setupMesh()
    {
        VAO.Bind();
        VBO.AddBufferData(vertices.data(), vertices.size() * sizeof(Vertex));
        VertexBufferLayout layout;
        layout.Push<float>(3); // vertex positions
        layout.Push<float>(3); // vertex normals
        layout.Push<float>(2); // vertex texture coords
        VAO.AddBuffer(VBO, layout);
        EBO.AddBufferData(indices.data(), indices.size());
        VAO.Unbind();
        VBO.Unbind();
        EBO.Unbind();
    }

    void Mesh::Draw(const Renderer& renderer, Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].GetType();
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.UploadUniformInt((/*"material." +*/ name + number).c_str(), i);
            textures[i].Bind(i);
        }

        // draw mesh
        renderer.Draw(VAO, EBO, shader);   
    }

}