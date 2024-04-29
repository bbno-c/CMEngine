#include "common.h"

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
#include <graphics/Model.h>

#include <utility/Base.h>
#include <functional>

constexpr const uint16_t ScreenWidth = 1920;
constexpr const uint16_t ScreenHeight = 1080;

void renderScene(CMEngine::Shader&);

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		};


		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	//assert(type == GL_DEBUG_TYPE_ERROR);
}

int main(int argc, char* argv[]) {
	using namespace CMEngine;

	VirtualFileSystem::GetInstance().Init("../../../../Assets/");

	Window window(ScreenWidth, ScreenHeight, "Master's Degree Showcase (CMEngine)");

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

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

	Vertex lightVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	Vertex planeVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-1, -1,  1), glm::vec3(0, 1,  0), glm::vec2(0, 0)},
		Vertex{glm::vec3(-1, -1, -1), glm::vec3(0, 1,  0), glm::vec2(0, 1)},
		Vertex{glm::vec3(1, -1, -1), glm::vec3(0, 1,  0), glm::vec2(1, 1)},
		Vertex{glm::vec3(1, -1,  1), glm::vec3(0, 1,  0), glm::vec2(1, 0)}
	};

	GLuint plaaneIndices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Shader simpleDepthShader("shaders/shadow_mapping_depth_vs.glsl", "shaders/shadow_mapping_depth_fs.glsl");
	Shader debugDepthQuad("shaders/debug_quad_vs.glsl", "shaders/debug_quad_depth_fs.glsl");

	Shader lightShader("shaders/light.vert", "shaders/light.frag");
	std::vector <Texture>	tex;
	std::vector <Vertex>	lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint>	lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);
	lightShader.UploadUniformFloat4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	std::vector <Texture>	planeTex{
		Texture(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/planks.png")),
		Texture(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/planksSpec.png"))
	};
	planeTex[0].SetType("texture_diffuse");
	planeTex[1].SetType("texture_specular");
	std::vector <Vertex>	planeVerts(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(Vertex));
	std::vector <GLuint>	planeInd(plaaneIndices, plaaneIndices + sizeof(plaaneIndices) / sizeof(GLuint));
	Mesh planeMesh(planeVerts, planeInd, planeTex);

	Shader ourShader("shaders/light_vs.glsl", "shaders/light_fs.glsl");
	Model ourModel(VirtualFileSystem::GetInstance().GetVFSFilePath("models/sponza/scene.gltf"));

// ### SHADOWS ### //

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ScreenWidth, ScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	debugDepthQuad.Bind();
	debugDepthQuad.UploadUniformInt("depthMap", 0);

// ### SHADOWS ### //

	float angleInDegrees = 0.0f; // Initial rotation angle.
	float rotationSpeed = 0.0f; // Rotation speed in degrees per second.
	Uint32 lastTime = SDL_GetTicks(); // Outside the loop, get the initial time.

	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightCubePosition = glm::vec3(-2.0f, 2.0f, -2.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRAMEBUFFER_SRGB); 

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

		float near_plane = 0.1f, far_plane = 100.0f;
		{
			// 1. render depth of scene to texture (from light's perspective)
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;
			// render scene from light's point of view
			simpleDepthShader.Bind();
			//simpleDepthShader.UploadUniformMat4("lightSpaceMatrix", camera->GetProjectionMatrix() * camera->GetViewMatrix());
			simpleDepthShader.UploadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			glViewport(0, 0, ScreenWidth, ScreenHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePosition); // translate it down so it's at the center of the scene
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
				model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
				angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
				glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);
				simpleDepthShader.UploadUniformMat4("model", model);
				ourModel.Draw(renderer, simpleDepthShader);
			}

			{
				glm::mat4 objectModel = glm::mat4(1.0f);
				objectModel = glm::translate(objectModel, glm::vec3(.0f, -10.0f, .0f));
				objectModel = glm::scale(objectModel, glm::vec3(50.0f, 1.0f, 50.0f));
				simpleDepthShader.UploadUniformMat4("model", objectModel);
				planeMesh.Draw(renderer, simpleDepthShader);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// reset viewport
			glViewport(0, 0, ScreenWidth, ScreenHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}

		{
			for (uint32_t i = 0; i < 4; i++) {
				glm::mat4 objectModel = glm::mat4(1.0f);
				objectModel = glm::translate(objectModel, pointLightPositions[i]);
				lightShader.UploadUniformMat4("model", objectModel);
				lightShader.UploadUniformMat4("view", camera->GetViewMatrix());
				lightShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
				light.Draw(renderer, lightShader);
			}
		}
		
		////////////////// RENDER NORMAL SCENE
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePosition); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
			model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
			angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
			glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);

			ourShader.UploadUniformMat4("model", model);
			ourShader.UploadUniformMat4("view", camera->GetViewMatrix());
			ourShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());

			ourShader.UploadUniformFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
			ourShader.UploadUniformFloat3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			ourShader.UploadUniformFloat3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
			ourShader.UploadUniformFloat3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.UploadUniformFloat("material.shininess", 32.0f);

			for (uint32_t i = 0; i < 4; i++) {
				std::stringstream uniformStream;
				uniformStream << "pointLights[" << i << "].position";
				ourShader.UploadUniformFloat3(uniformStream.str().c_str(), pointLightPositions[i]);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].constant";
				ourShader.UploadUniformFloat(uniformStream.str().c_str(), 1.0f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].linear";
				ourShader.UploadUniformFloat(uniformStream.str().c_str(), 0.09f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].quadratic";
				ourShader.UploadUniformFloat(uniformStream.str().c_str(), 0.032f);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].ambient";
				ourShader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].diffuse";
				ourShader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].specular";
				ourShader.UploadUniformFloat3(uniformStream.str().c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
			}

			ourShader.UploadUniformFloat3("spotLight.position", camera->GetPosition());
			ourShader.UploadUniformFloat3("spotLight.direction", camera->GetFront());
			ourShader.UploadUniformFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			ourShader.UploadUniformFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			ourShader.UploadUniformFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			ourShader.UploadUniformFloat("spotLight.constant", 1.0f);
			ourShader.UploadUniformFloat("spotLight.linear", 0.09f);
			ourShader.UploadUniformFloat("spotLight.quadratic", 0.032f);
			ourShader.UploadUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			ourShader.UploadUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			ourModel.Draw(renderer, ourShader);
		}

		{
			glm::mat4 objectModel = glm::mat4(1.0f);
			objectModel = glm::translate(objectModel, glm::vec3(.0f, -10.0f, .0f));
			objectModel = glm::scale(objectModel, glm::vec3(50.0f, 1.0f, 50.0f));	// it's a bit too big for our scene, so scale it down

			ourShader.UploadUniformMat4("model", objectModel);
			ourShader.UploadUniformMat4("view", camera->GetViewMatrix());
			ourShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());

			planeMesh.Draw(renderer, ourShader);
		}
		////////////////// RENDER NORMAL SCENE

		{
			// render Depth map to quad for visual debugging
			debugDepthQuad.Bind();
			debugDepthQuad.UploadUniformFloat("near_plane", near_plane);
			debugDepthQuad.UploadUniformFloat("far_plane", far_plane);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			renderQuad();

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

void renderScene(CMEngine::Shader&) {


}
