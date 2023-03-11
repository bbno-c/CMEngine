#include "AbstractWindow.h"
#include <SDL3/SDL.h>

#ifdef _WIN32
#undef main
#endif

class CWindow : public CAbstractWindow
{
protected:
    void OnWindowEvent(const SDL_Event &event) override
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_r:
                SetBackgroundColor(glm::vec4(1, 0, 0, 1));
                break;
            case SDLK_g:
                SetBackgroundColor(glm::vec4(0, 1, 0, 1));
                break;
            case SDLK_b:
                SetBackgroundColor(glm::vec4(0, 0, 1, 1));
                break;
            }
        }
    }

    void OnUpdateWindow(float deltaSeconds) override
    {
        (void)deltaSeconds;
    }

    void OnDrawWindow() override
    {
    }
};

int main()
{
    CWindow window;
    window.ShowFixedSize(glm::ivec2(800, 600));
    window.DoGameLoop();

    return 0;
}