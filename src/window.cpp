#include "window.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <sstream>

Engine::Window::Window( const char *title, int width, int height,
                        SDL_WindowFlags flags ) noexcept( false ) {
    using namespace std::string_literals;

    this->m_title = title;
    bool created =
        SDL_CreateWindowAndRenderer( this->m_title, width, height, flags,
                                     &this->m_window, &this->m_renderer );
    if ( !created ) {
        std::stringstream str(
            "SDL_CreateWindowAndRenderer failed! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }
}

Engine::Window::Window() {}

Engine::Window::~Window() noexcept {
    SDL_DestroyRenderer( this->m_renderer );
    SDL_DestroyWindow( this->m_window );
}

const char *Engine::Window::title() const noexcept { return this->m_title; }
SDL_Window *Engine::Window::window() const noexcept { return this->m_window; }
SDL_Renderer *Engine::Window::renderer() const noexcept {
    return this->m_renderer;
}
