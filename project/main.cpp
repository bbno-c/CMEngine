#include "common.h"

#include <graphics/Window.h>
#include <graphics/Shader.h>
#include <graphics/Renderer.h>
#include <graphics/VertexArray.h>
#include <graphics/VertexBuffer.h>
#include <graphics/VertexBufferLayout.h>
#include <graphics/IndexBuffer.h>

#include <utility/Base.h>

int main(int argc, char* argv[]) {

	using namespace CMEngine;

	Window window(800, 600, "My Game Engine");

	float vertices[] = {
	 0.5f,  0.5f, -0.5f,  // front top right
	 0.5f, -0.5f, -0.5f,  // front bottom right
	-0.5f, -0.5f, -0.5f,  // front bottom left
	-0.5f,  0.5f, -0.5f,  // front top left
	 0.5f,  0.5f,  0.5f,  // back top right
	 0.5f, -0.5f,  0.5f,  // back bottom right
	-0.5f, -0.5f,  0.5f,  // back bottom left
	-0.5f,  0.5f,  0.5f   // back top left
	};

	unsigned int indices[] = {
		0, 1, 3,  // front face
		1, 2, 3,  // front face
		4, 5, 7,  // back face
		5, 6, 7,  // back face
		0, 1, 4,  // right face
		1, 4, 5,  // right face
		2, 3, 6,  // left face
		3, 6, 7,  // left face
		0, 3, 4,  // top face
		3, 4, 7,  // top face
		1, 2, 5,  // bottom face
		2, 5, 6   // bottom face
	};

	Shader shader ("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	VertexArray va;
	VertexBuffer vb (vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	uint32_t count = sizeof(indices) / sizeof(uint32_t);
	IndexBuffer ib(indices, count);

	Renderer renderer;

	float angleInDegrees = 0.0f; // Initial rotation angle.
	float rotationSpeed = 20.0f; // Rotation speed in degrees per second.
	Uint32 lastTime = SDL_GetTicks(); // Outside the loop, get the initial time.

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!window.IsClosed()) {
		renderer.Clear();

		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert milliseconds to seconds.
		lastTime = currentTime;

		ib.Bind();
		shader.Bind();

		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);  // Initialize with identity matrix
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  // No translation
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // No scaling

		angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.

		// Normalize the diagonal to use as rotation axis (very important)
		glm::vec3 rotationAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));

		// Incorporate the rotation into the Model matrix
		model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);

		// View Matrix
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

		// Projection Matrix
		float fov = 45.0f;   // Field of View in degrees
		float aspect = 800.0f / 600.0f;  // Assuming an 800x600 window size
		float nearClip = 0.1f;
		float farClip = 100.0f;
		glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);

		shader.UploadUniformMat4("model", model);
		shader.UploadUniformMat4("view", view);
		shader.UploadUniformMat4("projection", projection);

		renderer.Draw(va, ib, shader);

		window.Update();
	}

	return 0;
}