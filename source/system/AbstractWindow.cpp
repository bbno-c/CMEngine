#include "common.h"
#include "AbstractWindow.h"
#include "Utils.h"
#include <SDL3/SDL_video.h>

namespace
{
const char WINDOW_TITLE[] = "OpenGL Drawing Demo";
}

class CAbstractWindow::Impl
{
public:
    void Show(glm::ivec2 const& size)
    {
        m_size = size;

		CUtils::InitOnceSDL2();

        // Выбираем Compatiblity Profile
        // Установка атрибутов SDL_GL должна выполняться до создания окна,
        // иначе на некоторых видеокартах создание контекста завершится
        // OpenGL с ошибкой.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        // Специальное значение SDL_WINDOWPOS_CENTERED вместо x и y заставит SDL2
        // разместить окно в центре монитора по осям x и y.
        // Для использования OpenGL вы ДОЛЖНЫ указать флаг SDL_WINDOW_OPENGL.
        m_pWindow.reset(SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

        // Создаём контекст OpenGL, связанный с окном.
        m_pGLContext.reset(SDL_GL_CreateContext(m_pWindow.get()));
        if (!m_pGLContext)
        {
			CUtils::ValidateSDL2Errors();
        }
		CUtils::InitOnceGLEW();
    }

    glm::ivec2 GetWindowSize() const
    {
        return m_size;
    }

    bool IsTerminated() const
    {
        return m_isTerminated;
    }

    void SetBackgroundColor(const glm::vec4 &color)
    {
        m_clearColor = color;
    }

    void Clear()const
    {
        // Заливка кадра цветом фона средствами OpenGL
        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void SwapBuffers()const
    {
        // Вывод нарисованного кадра в окно на экране.
        // При этом система отдаёт старый буфер для рисования нового кадра.
        // Обмен двух буферов вместо создания новых позволяет не тратить ресурсы впустую.
        SDL_GL_SwapWindow(m_pWindow.get());
    }

    bool ConsumeEvent(const SDL_Event &event)
    {
        bool consumed = false;
        if (event.type == SDL_EVENT_QUIT)
        {
            m_isTerminated = true;
            consumed = true;
        }
        else if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST)
        {
            OnWindowEvent(event.window);
            consumed = true;
        }
        return consumed;
    }

private:
    void OnWindowEvent(const SDL_WindowEvent &event)
    {
        if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            m_size = {event.data1, event.data2};
        }
    }

    SDLWindowPtr m_pWindow;
    SDLGLContextPtr m_pGLContext;
    glm::ivec2 m_size;
    glm::vec4 m_clearColor;
    bool m_isTerminated = false;
};

CAbstractWindow::CAbstractWindow()
    : m_pImpl(new Impl)
{
}

CAbstractWindow::~CAbstractWindow()
{
}

void CAbstractWindow::Show(const glm::ivec2 &size)
{
    m_pImpl->Show(size);
}

void CAbstractWindow::DoGameLoop()
{
    SDL_Event event;
    CChronometer chronometer;
    while (true)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (!m_pImpl->ConsumeEvent(event))
            {
                OnWindowEvent(event);
            }
        }
        if (m_pImpl->IsTerminated())
        {
            break;
        }
        // Очистка буфера кадра, обновление и рисование сцены, вывод буфера кадра.
        m_pImpl->Clear();
        const float deltaSeconds = chronometer.GrabDeltaTime();
        OnUpdateWindow(deltaSeconds);
        OnDrawWindow(m_pImpl->GetWindowSize());
        CUtils::ValidateOpenGLErrors();
        m_pImpl->SwapBuffers();
    }
}

void CAbstractWindow::SetBackgroundColor(const glm::vec4 &color)
{
    m_pImpl->SetBackgroundColor(color);
}
