#pragma once

#include <SDL.h>
#include <GL/glew.h>

namespace CMEngine {

    class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        void Clear() const;
        void Update();
        bool IsClosed() const;

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
    };
}