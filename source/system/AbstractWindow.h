#pragma once

#include <memory>
#include <glm/fwd.hpp>
#include <SDL3/SDL_events.h>

class CAbstractWindow
{
public:
    CAbstractWindow();
    virtual ~CAbstractWindow();

    void Show(glm::ivec2 const& size);
    void DoGameLoop();

protected:
    void SetBackgroundColor(glm::vec4 const& color);

    virtual void OnWindowEvent(const SDL_Event &event) = 0;
    virtual void OnUpdateWindow(float deltaSeconds) = 0;
    virtual void OnDrawWindow(glm::ivec2 const& size) = 0;

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};
