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

#include "window.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
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

    SDL_Surface *nullTextureSurf =
        SDL_CreateSurface( 2, 2, SDL_PIXELFORMAT_RGBA8888 );
    if ( nullTextureSurf == nullptr ) {
        std::stringstream str( "Could not create null texture! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }

    bool locked = SDL_LockSurface( nullTextureSurf );
    if ( !locked ) {
        std::stringstream str( "Could not lock null texture! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }

    if ( nullTextureSurf->pixels == nullptr ) {
        throw Engine::WindowError( "Pixels are null on null texture!" );
    }
    nullTextureSurf->pixels = new uint32_t[ sizeof( uint32_t ) * 4 ]{
        0xFF00FFFFU, 0x000000FFU, 0x000000FFU, 0xFF00FFFFU };
    if ( nullTextureSurf->pixels == nullptr ) {
        std::stringstream str(
            "Pixels not written to null texture! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }
    SDL_UnlockSurface( nullTextureSurf );

    this->m_nullTexture =
        SDL_CreateTextureFromSurface( this->m_renderer, nullTextureSurf );
    if ( this->m_nullTexture == nullptr ) {
        std::stringstream str(
            "Could not create null texture from surface! SDL_Error: " );
        str << SDL_GetError();
        throw Engine::WindowError( str.str().c_str() );
    }
}

Engine::Window::Window() noexcept( false ) {}

Engine::Window::~Window() noexcept {
    SDL_DestroyRenderer( this->m_renderer );
    SDL_DestroyWindow( this->m_window );
}

const char *Engine::Window::title() const noexcept { return this->m_title; }
SDL_Window *Engine::Window::window() const noexcept { return this->m_window; }
SDL_Renderer *Engine::Window::renderer() const noexcept {
    return this->m_renderer;
}
SDL_Texture *Engine::Window::nullTexture() const noexcept {
    return this->m_nullTexture;
}
