#include "Window.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

namespace CMEngine {

    MyWidget::MyWidget(QWidget* parent) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        inputField = new QLineEdit(this);
        sendButton = new QPushButton("Send to Engine", this);

        layout->addWidget(inputField);
        layout->addWidget(sendButton);

        setLayout(layout);

        connect(sendButton, &QPushButton::clicked, this, &MyWidget::onButtonClicked);
    }

    void MyWidget::onButtonClicked() {
        emit sendDataToEngine(inputField->text());
    }

    Window::Window(int width, int height, const char* title)
        : m_isClosed(false)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            // SDL Initialization failed
            throw std::runtime_error("Failed to initialize SDL");
        }

        // Set the desired OpenGL version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        const char* glsl_version = "#version 330";

        ScreenWidth = width;
        ScreenHeight = height;

        m_window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_EVENT_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            // Window creation failed
            SDL_Quit();
            throw std::runtime_error("Failed to create window");
        }

        m_glContext = SDL_GL_CreateContext(m_window);

        if (glewInit() != GLEW_OK) {
            // GLEW initialization failed
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            throw std::runtime_error("Failed to initialize GLEW");
        }

    }

    Window::~Window() {
        SDL_GL_DeleteContext(m_glContext);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Window::Update(const float deltaTime, std::shared_ptr<Camera> camera) {
        SDL_GL_SwapWindow(m_window);

        // Handles key inputs
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);

        if (keystates[SDL_SCANCODE_W]) {
            camera->MoveForward(deltaTime);
        }
        if (keystates[SDL_SCANCODE_A]) {
            camera->MoveLeft(deltaTime);
        }
        if (keystates[SDL_SCANCODE_S]) {
            camera->MoveBackward(deltaTime);
        }
        if (keystates[SDL_SCANCODE_D]) {
            camera->MoveRight(deltaTime);
        }
        if (keystates[SDL_SCANCODE_SPACE]) {
            camera->MoveUp(deltaTime);
        }
        if (keystates[SDL_SCANCODE_LCTRL]) {
            camera->MoveDown(deltaTime);
        }
        //if (keystates[SDL_SCANCODE_LSHIFT]) {
        //    speed = 0.4f;
        //}
        //else {
        //    speed = 0.1f;
        //}

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            ImGui_ImplSDL3_ProcessEvent(&event);

            if (!ImGui::GetIO().WantCaptureMouse) {
                // The mouse is hovering over the current widget

            if (event.type == SDL_EVENT_QUIT) {
                m_isClosed = true;
            }

            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                // Handle window resize here
                int newWidth = event.window.data1;
                int newHeight = event.window.data2;

                ScreenWidth = event.window.data1;
                ScreenHeight = event.window.data2;

                // Update your rendering viewport or projection matrix with the new width and height
                glViewport(0, 0, newWidth, newHeight);
                camera->SetPerspective(45.0f, 0.1f, 100.0f);
            }

            // Mouse Input
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                SDL_SetRelativeMouseMode(SDL_TRUE); // Hide the mouse cursor
                if (firstClick)
                {
                    SDL_WarpMouseInWindow(m_window, ScreenWidth / 2, ScreenHeight / 2); // Center the mouse
                    firstClick = false;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT)
            {
                SDL_SetRelativeMouseMode(SDL_FALSE); // Show the mouse cursor
                firstClick = true;
            }
            else if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                if (SDL_GetRelativeMouseMode())
                {
                    camera->Rotate(event.motion.xrel, -event.motion.yrel);
                    SDL_WarpMouseInWindow(m_window, ScreenWidth / 2, ScreenHeight / 2); // Center the mouse
                }
            }
            }
        }
    }

    bool Window::IsClosed() const {
        return m_isClosed;
    }
}