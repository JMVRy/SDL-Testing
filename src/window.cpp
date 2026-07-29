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

#include <memory>
#include <sstream>
#include <utility>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include "texture.hpp"
#include "window.hpp"

Engine::Window::Window( const char *title, int width, int height,
                        SDL_WindowFlags flags ) noexcept( false ) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool created = SDL_CreateWindowAndRenderer(
        ( this->m_title = title ), width, height, flags, &window, &renderer );
    if ( !created ) {
        std::stringstream str(
            "SDL_CreateWindowAndRenderer failed! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }

    this->m_window.reset( window );
    this->m_renderer.reset( renderer );

    std::unique_ptr<SDL_Surface, decltype( &SDL_DestroySurface )>
        nullTextureSurf{ SDL_CreateSurface( 2, 2, SDL_PIXELFORMAT_RGBA8888 ),
                         SDL_DestroySurface };
    if ( nullTextureSurf == nullptr ) {
        std::stringstream str( "Could not create null texture! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }

    bool locked = SDL_LockSurface( nullTextureSurf.get() );
    if ( !locked ) {
        std::stringstream str( "Could not lock null texture! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }

    if ( nullTextureSurf->pixels == nullptr ) {
        throw Engine::WindowError( "Pixels are null on null texture!" );
    }
    auto *pixels = static_cast<uint32_t *>( nullTextureSurf->pixels );
    pixels[ 0 ] = 0xFF00FFFFU;
    pixels[ 1 ] = 0x000000FFU;
    pixels[ 2 ] = 0x000000FFU;
    pixels[ 3 ] = 0xFF00FFFFU;
    SDL_UnlockSurface( nullTextureSurf.get() );

    SDL_Texture *nullTexture = SDL_CreateTextureFromSurface(
        this->m_renderer.get(), nullTextureSurf.get() );
    if ( nullTexture == nullptr ) {
        std::stringstream str(
            "Could not create null texture from surface! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }
    this->m_nullTexture = Engine::Texture( nullTexture );
}

Engine::Window &Engine::Window::operator=( Window &&other ) noexcept {
    if ( this == &other )
        return *this;

    this->m_nullTexture = std::move( other.m_nullTexture );

    this->m_renderer.reset();
    this->m_window.reset();

    this->m_title = std::exchange( other.m_title, nullptr );
    this->m_window = std::move( other.m_window );
    this->m_renderer = std::move( other.m_renderer );

    return *this;
}

const char *Engine::Window::title() const noexcept { return this->m_title; }
const std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )> &
Engine::Window::window() const noexcept {
    return this->m_window;
}
const std::unique_ptr<SDL_Renderer, decltype( &SDL_DestroyRenderer )> &
Engine::Window::renderer() const noexcept {
    return this->m_renderer;
}
const Engine::Texture &Engine::Window::nullTexture() const noexcept {
    return this->m_nullTexture;
}
