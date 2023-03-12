#include "common.h"
#include "DispatchEvent.h"

void sdl::DispatchEvent(const SDL_Event &event, IInputEventAcceptor &acceptor)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
        acceptor.OnKeyDown(event.key);
        break;
    case SDL_EVENT_KEY_UP:
        acceptor.OnKeyUp(event.key);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        acceptor.OnMouseDown(event.button);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        acceptor.OnMouseUp(event.button);
        break;
    case SDL_EVENT_MOUSE_MOTION:
        acceptor.OnMouseMotion(event.motion);
        break;
    case SDL_EVENT_MOUSE_WHEEL:
        acceptor.OnMouseWheel(event.wheel);
        break;
    }
}
