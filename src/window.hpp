#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdexcept>

namespace Engine {
class WindowError : public std::runtime_error {
  public:
    WindowError( const char *msg ) : std::runtime_error( msg ) {}
};

class Window {
  public:
    Window() noexcept( false );
    Window( const char *title, int width, int height,
            SDL_WindowFlags flags ) noexcept( false );
    ~Window() noexcept;

    const char *title() const noexcept;
    SDL_Window *window() const noexcept;
    SDL_Renderer *renderer() const noexcept;

  private:
    const char *m_title = nullptr;
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
};
} // namespace Engine