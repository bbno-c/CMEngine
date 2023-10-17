#include "Window.h"

namespace CMEngine {

    Window::Window(int width, int height, const char* title)
        : m_isClosed(false)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            // SDL Initialization failed
            throw std::runtime_error("Failed to initialize SDL");
        }

        // Set the desired OpenGL version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

        m_window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_EVENT_WINDOW_SHOWN
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

    void Window::Update() {
        SDL_GL_SwapWindow(m_window);

        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                m_isClosed = true;
            }
        }
    }

    bool Window::IsClosed() const {
        return m_isClosed;
    }
}