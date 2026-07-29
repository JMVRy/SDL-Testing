/*
    This file is part of SDL Testing.

    SDL Testing is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SDL Testing is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with SDL Testing.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <memory>
#include <stdexcept>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "texture.hpp"

namespace Engine {
class WindowError : public std::runtime_error {
  public:
    WindowError( const char *msg ) : std::runtime_error( msg ) {}
};

class Window {
  public:
    Window() noexcept = default;
    Window( const char *title, int width, int height,
            SDL_WindowFlags flags ) noexcept( false );
    Window( const Window & ) = delete;
    Window &operator=( const Window & ) = delete;
    Window( Window &&other ) noexcept = default;
    Window &operator=( Window &&other ) noexcept;
    ~Window() noexcept = default;

    const char *title() const noexcept;
    const std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )> &
    window() const noexcept;
    const std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )> &
    renderer() const noexcept;
    const Engine::Texture &nullTexture() const noexcept;

  private:
    const char *m_title = nullptr;
    std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )> m_window{
        nullptr, SDL_DestroyWindow };
    std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )> m_renderer{
        nullptr, SDL_DestroyRenderer };
    Engine::Texture m_nullTexture;
};
} // namespace Engine