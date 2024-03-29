﻿#include "common.h"

#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include <graphics/Window.h>
#include <graphics/Shader.h>
#include <graphics/Renderer.h>
#include <graphics/VertexArray.h>
#include <graphics/VertexBuffer.h>
#include <graphics/VertexBufferLayout.h>
#include <graphics/IndexBuffer.h>
#include <graphics/Texture.h>
#include <graphics/Camera.h>

#include <utility/Base.h>

constexpr const uint16_t ScreenWidth = 1920;
constexpr const uint16_t ScreenHeight = 1080;

int main(int argc, char* argv[]) {
	using namespace CMEngine;

	VirtualFileSystem::GetInstance().Init("../../../../Assets/");

	Window window(ScreenWidth, ScreenHeight, "Master's Degree Showcase (CMEngine)");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.Fonts->AddFontDefault();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window.GetWindow(), window.GetContext());
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	const char* glVersion = (const char*)glGetString(GL_VERSION);
	if (glVersion) {
		printf("OpenGL Version: %s\n", glVersion);
	}
	else {
		// Handle error
	}

	// Creates camera object
	std::shared_ptr<Camera> camera = std::make_unique<Camera>(ScreenWidth, ScreenHeight);

	float learnogl_vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	float vertices[] = {
		// aPos               // texCoord 
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 
	   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 
	   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 
		0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 
		0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 
	   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 
	   -0.5f,  0.5f,  0.5f,   1.0f, 1.0f 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7,
		0, 1, 4,
		1, 4, 5,
		2, 3, 6,
		3, 6, 7,
		0, 3, 4,
		3, 4, 7,
		1, 2, 5,
		2, 5, 6
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Shader shader("shaders/light_vs.glsl", "shaders/light_fs.glsl");
	VertexArray va;
	VertexBuffer vb(learnogl_vertices, sizeof(learnogl_vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	//IndexBuffer ib(indices_triangles, sizeof(indices_triangles) / sizeof(uint32_t));

	Texture texture(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/container2.png"));
	//texture.Bind(0);
	shader.UploadUniformInt("material.diffuse", 0);

	Texture texture2(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/container2_specular.png"));
	//texture2.Bind(1);
	shader.UploadUniformInt("material.specular", 1);

	Texture texture_bat_signal(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/bat_logo.png"));
	shader.UploadUniformInt("spotLight.diffuse_txt", 2);
	shader.UploadUniformInt("spotLight.specular_txt", 3);


	shader.UploadUniformFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	shader.UploadUniformFloat3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.UploadUniformFloat3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
	shader.UploadUniformFloat3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	va.Unbind();
	vb.Unbind();
	//ib.Unbind();
	shader.Unbind();

	Shader shader_light_source("shaders/basic_color_vs.glsl", "shaders/basic_color_fs.glsl");
	shader_light_source.UploadUniformFloat3("objectColor", glm::vec3(1.0f,1.0f,1.0f));
	VertexArray va_light;
	VertexBuffer vb_light(vertices, sizeof(vertices));
	VertexBufferLayout layout_light;
	layout_light.Push<float>(3);
	layout_light.Push<float>(2);
	va_light.AddBuffer(vb_light, layout_light);
	IndexBuffer ib_light(indices, sizeof(indices) / sizeof(uint32_t));
	va_light.Unbind();
	vb_light.Unbind();
	ib_light.Unbind();
	shader_light_source.Unbind();

	float angleInDegrees = 0.0f; // Initial rotation angle.
	float rotationSpeed = 0.0f; // Rotation speed in degrees per second.
	Uint32 lastTime = SDL_GetTicks(); // Outside the loop, get the initial time.

	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightCubePosition = glm::vec3(-2.0f, 2.0f, -2.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	Renderer renderer;

	while (!window.IsClosed()) {
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert milliseconds to seconds.
		lastTime = currentTime;

		window.Update(deltaTime, camera);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Control Panel");
			ImGui::SliderFloat3("Cube Position", glm::value_ptr(cubePosition), -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Cube Position", glm::value_ptr(lightCubePosition), -10.0f, 10.0f);
			ImGui::SliderFloat("rotation speed", &rotationSpeed, 0.0f, 100.0f);



			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		renderer.Clear();

		for (uint32_t i = 0; i < 4; i++)
		{
			// Model Matrix
			glm::mat4 model = glm::mat4(1.0f);  // Initialize with identity matrix
			model = glm::translate(model, pointLightPositions[i]);  // No translation
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));  // No scaling
			// Handles camera inputs
			shader_light_source.UploadUniformMat4("model", model);
			shader_light_source.UploadUniformMat4("view", camera->GetViewMatrix());
			shader_light_source.UploadUniformMat4("projection", camera->GetProjectionMatrix());

			renderer.Draw(va_light, ib_light, shader_light_source);
		}

		for (uint32_t i = 0; i < 10; i++)
		{
			// Model Matrix
			glm::mat4 model = glm::mat4(1.0f);  // Initialize with identity matrix
			model = glm::translate(model, cubePositions[i]);  // No translation
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // No scaling
			angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
			// Normalize the diagonal to use as rotation axis (very important)
			glm::vec3 rotationAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
			// Incorporate the rotation into the Model matrix
			model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);
			// Handles camera inputs
			shader.UploadUniformMat4("model", model);
			shader.UploadUniformMat4("view", camera->GetViewMatrix());
			shader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
			shader.UploadUniformFloat3("viewPos", camera->GetPosition());
			shader.UploadUniformFloat("material.shininess", 32.0f);

			for (uint32_t i = 0; i < 4; i++) {
				std::stringstream uniformStream;
				uniformStream << "pointLights[" << i << "].position";
				shader.UploadUniformFloat3(uniformStream.str().c_str(), pointLightPositions[i]);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].constant";
				shader.UploadUniformFloat(uniformStream.str().c_str(), 1.0f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].linear";
				shader.UploadUniformFloat(uniformStream.str().c_str(), 0.09f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].quadratic";
				shader.UploadUniformFloat(uniformStream.str().c_str(), 0.032f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].ambient";
				shader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].diffuse";
				shader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].specular";
				shader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
			}

			shader.UploadUniformFloat3("spotLight.position", glm::vec3(0.0f, 0.0f, 3.0f));//camera->GetPosition());
			shader.UploadUniformFloat3("spotLight.direction", glm::vec3(0.2f, 0.0f, -1.0f));// camera->GetFront());
			shader.UploadUniformFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			shader.UploadUniformFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.UploadUniformFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.UploadUniformFloat("spotLight.constant", 1.0f);
			shader.UploadUniformFloat("spotLight.linear", 0.09f);
			shader.UploadUniformFloat("spotLight.quadratic", 0.032f);
			shader.UploadUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			shader.UploadUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			texture.Bind(0);
			texture2.Bind(1);
			texture_bat_signal.Bind(2);
			texture_bat_signal.Bind(3);

			renderer.Draw(va, 36, shader);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}