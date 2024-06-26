﻿#include "common.h"

#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
//#include <QGridLayout>
//#include <QVBoxLayout>

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
			-1.f,  -0.5f, 0.0f, 0.0f, 1.0f,
			-1.f, -1.f, 0.0f, 0.0f, 0.0f,
			 -0.5f,  -0.5f, 0.0f, 1.0f, 1.0f,
			 -0.5f, -1.f, 0.0f, 1.0f, 0.0f,
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
	//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	//	type, severity, message);
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		int a = 0;
	};
}

glm::vec3 cubePositionMoving(0.0f, 0.0f, 0.0f);
float cubeSpeed = 0.0f; // Speed of the cube's movement
bool movingRight = true;

void UpdateCubePosition(float deltaTime) {
	if (movingRight) {
		cubePositionMoving.x += cubeSpeed * deltaTime;
		if (cubePositionMoving.x > 10.0f) {
			cubePositionMoving.x = 10.0f;
			movingRight = false;
		}
	}
	else {
		cubePositionMoving.x -= cubeSpeed * deltaTime;
		if (cubePositionMoving.x < -10.0f) {
			cubePositionMoving.x = -10.0f;
			movingRight = true;
		}
	}
}

void handleInputData(const QString& data) {
	// Process the data and update the game engine accordingly
	//qDebug() << "Data received from Qt widget:" << data;
	// Example: Update some parameter in the game engine
	// engine.updateParameter(data.toStdString());
	fprintf(stderr, "Data received from Qt widget: %s \n", data.toStdString().c_str());

}

int main(int argc, char* argv[]) {
	using namespace CMEngine;

	VirtualFileSystem::GetInstance().Init("Assets/");

	QApplication app(argc, argv); // Basics of QT, start an application
	// Create a simple Qt widget
	MyWidget myWidget;
	myWidget.resize(320, 240);
	myWidget.setWindowTitle("Qt Widget Example");

	// Connect the signal to the slot/function
	QObject::connect(&myWidget, &MyWidget::sendDataToEngine, &handleInputData);

	// Show the Qt widget
	myWidget.show();


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
	camera->LoadStatesFromFile("camera_states.txt");

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
	glm::vec3(-1.0f,  5.0f,  6.5f),
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

	float cube_vertices[] = {
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
		Vertex{glm::vec3(-10, -1,  10), glm::vec3(0, 1,  0), glm::vec2(0, 0)},
		Vertex{glm::vec3(-10, -1, -10), glm::vec3(0, 1,  0), glm::vec2(0, 10)},
		Vertex{glm::vec3(10, -1, -10), glm::vec3(0, 1,  0), glm::vec2(10, 10)},
		Vertex{glm::vec3(10, -1,  10), glm::vec3(0, 1,  0), glm::vec2(10, 0)}
	};

	GLuint plaaneIndices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Shader simpleDepthShader("shaders/shadow_mapping_depth_vs.glsl", "shaders/shadow_mapping_depth_fs.glsl", "shaders/shadow_mapping_depth_gs.glsl");
	Shader debugDepthQuad("shaders/debug_quad_vs.glsl", "shaders/debug_quad_depth_fs.glsl");
	Shader lightShader("shaders/light.vert", "shaders/light.frag");
	Shader modelShader("shaders/shadow_mapping_vs.vert", "shaders/shadow_mapping_fs.frag");

	std::vector <Texture>	tex;
	std::vector <Vertex>	lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint>	lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);
	lightShader.UploadUniformFloat4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


	VertexArray cube_va;
	VertexBuffer vb(cube_vertices, sizeof(cube_vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	cube_va.AddBuffer(vb, layout);
	Texture cube_texture_diff(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/container2.png"));
	Texture cube_texture_spec(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/container2_specular.png"));

	std::vector <Texture>	planeTex{
		Texture(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/planks.png")),
		Texture(VirtualFileSystem::GetInstance().GetVFSFilePath("textures/planksSpec.png"))
	};
	planeTex[0].SetType("texture_diffuse");
	planeTex[1].SetType("texture_specular");
	std::vector <Vertex>	planeVerts(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(Vertex));
	std::vector <GLuint>	planeInd(plaaneIndices, plaaneIndices + sizeof(plaaneIndices) / sizeof(GLuint));
	Mesh planeMesh(planeVerts, planeInd, planeTex);
	Mesh planeMesh2(planeVerts, planeInd, planeTex);
	Mesh planeMesh3(planeVerts, planeInd, planeTex);
	Mesh planeMesh4(planeVerts, planeInd, planeTex);
	Mesh planeMesh5(planeVerts, planeInd, planeTex);

	//Shader modelShader("shaders/light_vs.glsl", "shaders/light_fs.glsl");
	modelShader.UploadUniformInt("depthMap", 2);
	Model modelObject(VirtualFileSystem::GetInstance().GetVFSFilePath("models/sponza/scene.gltf"));

	////////////////// SHADOWS

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	debugDepthQuad.Bind();
	debugDepthQuad.UploadUniformInt("depthMap", 0);

	////////////////// SHADOWS

	float angleInDegrees = 0.0f; // Initial rotation angle.
	float rotationSpeed = 0.0f; // Rotation speed in degrees per second.
	float shininess = 16.0f; // Rotation speed in degrees per second.
	Uint32 lastTime = SDL_GetTicks(); // Outside the loop, get the initial time.

	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, 6.5f);
	glm::vec3 lightPosition = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 lightCubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

	float Constant = 1.0f;
	float Linear = 0.09f;
	float Quadratic = 0.032f;
	float FarPlane = 25.0f;
	glm::vec3 Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	int shadowMethod = 0;
	glm::vec3 model_scale = glm::vec3(0.02f, 0.02f, 0.02f);
	//glm::vec3 model_scale = glm::vec3(1.0f, 1.0f, 1.0f);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRAMEBUFFER_SRGB); 

	Renderer renderer;

	while (!window.IsClosed()) {

		app.processEvents();

		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert milliseconds to seconds.
		lastTime = currentTime;

		UpdateCubePosition(deltaTime);
 
		window.Update(deltaTime, camera);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Control Panel");
			ImGui::SliderFloat3("Cube Position", glm::value_ptr(cubePosition), -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Cube Position", glm::value_ptr(lightCubePosition), -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPosition), -10.0f, 10.0f);
			ImGui::SliderFloat("rotation speed", &rotationSpeed, 0.0f, 100.0f);
			ImGui::SliderFloat("Shininess", &shininess, 0.0f, 32.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			// New floating panel for point lights
			ImGui::Begin("Point Light Control Panel");
			//for (int i = 0; i < numberOfPointLights; ++i) 
			{
				std::string labelPrefix = "Point Light ";// +std::to_string(i);

				ImGui::Separator();
				ImGui::Text("%s", labelPrefix.c_str());

				ImGui::SliderFloat((labelPrefix + " Move Speed").c_str(), &cubeSpeed, 0.0f, 10.0f);
				ImGui::SliderFloat3((labelPrefix + " Position").c_str(), glm::value_ptr(lightCubePosition), -10.0f, 10.0f);
				ImGui::SliderFloat((labelPrefix + " Constant").c_str(), &Constant, 0.0f, 1.0f);
				ImGui::SliderFloat((labelPrefix + " Linear").c_str(), &Linear, 0.0f, 1.0f);
				ImGui::SliderFloat((labelPrefix + " Quadratic").c_str(), &Quadratic, 0.0f, 1.0f);
				ImGui::ColorEdit3((labelPrefix + " Ambient").c_str(), glm::value_ptr(Ambient));
				ImGui::ColorEdit3((labelPrefix + " Diffuse").c_str(), glm::value_ptr(Diffuse));
				ImGui::ColorEdit3((labelPrefix + " Specular").c_str(), glm::value_ptr(Specular));
				ImGui::SliderFloat((labelPrefix + " Far Plane").c_str(), &FarPlane, 0.0f, 1000.0f);
			}

			const char* shadowMethods[] = { "ShadowCalculation with PCF", "ShadowCalculationNoPCF", "No Shadows" };
			ImGui::Combo("Shadow Method", &shadowMethod, shadowMethods, IM_ARRAYSIZE(shadowMethods));
			modelShader.UploadUniformInt("shadowCalculationMethod", shadowMethod);

			ImGui::End();


			ImGui::Begin("Camera Control");
			if (ImGui::Button("Save Current State")) {
				camera->SaveCurrentState();
			}

			static int selectedIndex = -1;
			const auto& states = camera->GetStates();
			std::vector<const char*> stateLabels;
			stateLabels.reserve(states.size());
			for (size_t i = 0; i < states.size(); ++i) {
				stateLabels.push_back(std::to_string(i).c_str());
			}

			ImGui::ListBox("Saved States", &selectedIndex, stateLabels.data(), static_cast<int>(states.size()));

			if (ImGui::Button("Load Selected State") && selectedIndex >= 0 && selectedIndex < states.size()) {
				camera->LoadState(selectedIndex);
			}

			if (ImGui::Button("Delete Selected State") && selectedIndex >= 0 && selectedIndex < states.size()) {
				camera->DeleteState(selectedIndex);
				selectedIndex = -1;  // Reset the selected index
			}
			ImGui::End();
		}

		renderer.Clear();

		////////////////// PRE-RENDER SCENE

		// 1. render depth of scene to texture (from light's perspective)
		//glm::mat4 lightProjection, lightView;
		//glm::mat4 lightSpaceMatrix;
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, CAM_NEAR, CAM_FAR);
		////lightView = glm::lookAt(pointLightPositions[0] + lightCubePosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

		//// Use perspective projection instead of orthographic
		//float fov = 45.0f;
		//float aspectRatio = ScreenWidth/ ScreenHeight;
		//float nearPlane = CAM_NEAR;
		//float farPlane = CAM_FAR;
		////lightProjection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		//lightView = glm::lookAt(pointLightPositions[0] + lightCubePosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//lightSpaceMatrix = lightProjection * lightView;

		//angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.

		glm::vec3 light_pos = pointLightPositions[0] + cubePositionMoving + lightCubePosition;
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		float near_plane = 1.0f;
		float far_plane = FarPlane;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


		// render scene from light's point of view
		//simpleDepthShader.Bind();
		//simpleDepthShader.UploadUniformMat4("lightSpaceMatrix", camera->GetProjectionMatrix() * camera->GetViewMatrix());
		//simpleDepthShader.UploadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

		// 1. render scene to depth cubemap
		// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (unsigned int i = 0; i < 6; ++i)
			simpleDepthShader.UploadUniformMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		simpleDepthShader.UploadUniformFloat("far_plane", far_plane);
		simpleDepthShader.UploadUniformFloat3("lightPos", light_pos);

		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePosition); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
			model = glm::scale(model, model_scale);	// it's a bit too big for our scene, so scale it down
			angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
			glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);
			simpleDepthShader.UploadUniformMat4("model", model);
			modelObject.Draw(renderer, simpleDepthShader);
		}

		//for (uint32_t i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i] + cubePosition);  // No translation
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
		//	glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);

		//	simpleDepthShader.UploadUniformMat4("model", model);

		//	renderer.Draw(cube_va, 36, simpleDepthShader);
		//}

		//{
		//	glm::mat4 objectModel = glm::mat4(1.0f);
		//	objectModel = glm::translate(objectModel, glm::vec3(.0f, -2.0f, .0f));
		//	simpleDepthShader.UploadUniformMat4("model", objectModel);
		//	planeMesh.Draw(renderer, simpleDepthShader);
		//}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, ScreenWidth, ScreenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		////////////////// RENDER NORMAL SCENE

		{
			for (uint32_t i = 0; i < 1; i++) {
				glm::mat4 objectModel = glm::mat4(1.0f);
				objectModel = glm::translate(objectModel, light_pos);
				lightShader.UploadUniformMat4("model", objectModel);
				lightShader.UploadUniformMat4("view", camera->GetViewMatrix());
				lightShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
				light.Draw(renderer, lightShader);
			}
		}

		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePosition); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // No rotation
			model = glm::scale(model, model_scale);	// it's a bit too big for our scene, so scale it down
			angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
			glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);

			modelShader.UploadUniformMat4("model", model);
			modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
			modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());

			//modelShader.UploadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			modelShader.UploadUniformFloat3("dirLight.direction", lightPosition);
			modelShader.UploadUniformFloat3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			modelShader.UploadUniformFloat3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
			modelShader.UploadUniformFloat3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			modelShader.UploadUniformFloat("material.shininess", shininess);

			for (uint32_t i = 0; i < 1; i++) {
				std::stringstream uniformStream;
				uniformStream << "pointLights[" << i << "].position";
				modelShader.UploadUniformFloat3(uniformStream.str().c_str(), light_pos);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].constant";
				modelShader.UploadUniformFloat(uniformStream.str().c_str(), Constant);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].linear";
				modelShader.UploadUniformFloat(uniformStream.str().c_str(), Linear);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].quadratic";
				modelShader.UploadUniformFloat(uniformStream.str().c_str(), Quadratic);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].ambient";
				modelShader.UploadUniformFloat3(uniformStream.str().c_str(), Ambient);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].diffuse";
				modelShader.UploadUniformFloat3(uniformStream.str().c_str(), Diffuse);
				uniformStream.str("");
				uniformStream << "pointLights[" << i << "].specular";
				modelShader.UploadUniformFloat3(uniformStream.str().c_str(), Specular);
			}

			modelShader.UploadUniformFloat3("spotLight.position", camera->GetPosition());
			modelShader.UploadUniformFloat3("spotLight.direction", camera->GetFront());
			modelShader.UploadUniformFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			modelShader.UploadUniformFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			modelShader.UploadUniformFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			modelShader.UploadUniformFloat("spotLight.constant", 1.0f);
			modelShader.UploadUniformFloat("spotLight.linear", 0.09f);
			modelShader.UploadUniformFloat("spotLight.quadratic", 0.032f);
			modelShader.UploadUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			modelShader.UploadUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
			modelShader.UploadUniformFloat("far_plane", far_plane);

			modelObject.Draw(renderer, modelShader);
		}

		//for (uint32_t i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]+cubePosition);  // No translation
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	//model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
		//	angleInDegrees += rotationSpeed * deltaTime; // Update rotation angle.
		//	glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, glm::radians(angleInDegrees), rotationAxis);

		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());

		//	modelShader.UploadUniformInt("material.diffuse", 0);
		//	modelShader.UploadUniformInt("material.specular", 1);

		//	cube_texture_diff.Bind(0);
		//	cube_texture_spec.Bind(1);
		//	renderer.Draw(cube_va, 36, modelShader);
		//}

		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(.0f, -5.0f, .0f));
		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
		//	planeMesh.Draw(renderer, modelShader);

		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -9.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
		//	planeMesh2.Draw(renderer, modelShader);

		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 9.0f));
		//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
		//	planeMesh3.Draw(renderer, modelShader);

		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
		//	planeMesh4.Draw(renderer, modelShader);

		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(-9.0f, 0.0f, 0.0f));
		//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//	modelShader.UploadUniformMat4("model", model);
		//	modelShader.UploadUniformMat4("view", camera->GetViewMatrix());
		//	modelShader.UploadUniformMat4("projection", camera->GetProjectionMatrix());
		//	planeMesh5.Draw(renderer, modelShader);

		//}
		////////////////// POST-RENDER

		//{
		//	// render Depth map to quad for visual debugging
		//	debugDepthQuad.Bind();
		//	debugDepthQuad.UploadUniformFloat("near_plane", CAM_NEAR);
		//	debugDepthQuad.UploadUniformFloat("far_plane", CAM_FAR);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		//	renderQuad();

		//}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	camera->SaveStatesToFile("camera_states.txt");

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	int RetVal = app.exec();	// Most examples have this on the return, we


	return RetVal;
	//return 0;
}

void renderScene(CMEngine::Shader& shader) {


}
