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

#include <cstdint>
#include <iostream>
#include <utility>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include "texture.hpp"

Engine::Texture::Texture( SDL_Texture *texture ) noexcept
    : m_texture( texture ) {
    float width = 0.0f;
    float height = 0.0f;
    SDL_GetTextureSize( texture, &width, &height );
    this->m_width = static_cast<uint32_t>( width );
    this->m_height = static_cast<uint32_t>( height );
}

Engine::Texture::Texture( std::string path, SDL_Renderer *renderer ) {
    bool loaded = this->LoadNewTexture( path, renderer );
    if ( !loaded ) {
        throw Engine::TextureError( "Failed to create texture!" );
    }
}

Engine::Texture::Texture() noexcept
    : m_texture( nullptr ), m_width( 0 ), m_height( 0 ) {}

Engine::Texture::Texture( Texture &&other ) noexcept
    : m_texture( std::exchange( other.m_texture, nullptr ) ),
      m_width( other.m_width ), m_height( other.m_height ) {}

Engine::Texture &Engine::Texture::operator=( Texture &&other ) noexcept {
    if ( this == &other )
        return *this;

    SDL_DestroyTexture( this->m_texture );

    this->m_texture = std::exchange( other.m_texture, nullptr );
    this->m_width = other.m_width;
    this->m_height = other.m_height;

    return *this;
}

Engine::Texture::~Texture() noexcept { SDL_DestroyTexture( this->m_texture ); }

bool Engine::Texture::LoadNewTexture(
    std::string path, SDL_Renderer *renderer ) noexcept( false ) {
    if ( this->m_texture != nullptr )
        SDL_DestroyTexture( this->m_texture );

    SDL_Texture *texture = IMG_LoadTexture( renderer, path.c_str() );
    if ( texture == nullptr ) {
        std::cerr << "Failed to load texture from '" << path
                  << "'! SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }

    this->m_texture = texture;
    SDL_GetTextureSize( texture, &this->m_width, &this->m_height );
    return true;
}

void Engine::Texture::Render( SDL_Renderer *renderer ) const noexcept {
    SDL_RenderTexture( renderer, this->m_texture, nullptr, nullptr );
}

void Engine::Texture::Render( SDL_Renderer *renderer, float x,
                              float y ) const noexcept {
    SDL_FRect rect = { x, y, static_cast<float>( this->m_width ),
                       static_cast<float>( this->m_height ) };
    SDL_RenderTexture( renderer, this->m_texture, nullptr, &rect );
}

void Engine::Texture::Render( SDL_Renderer *renderer, float x, float y,
                              float width, float height ) const noexcept {
    SDL_FRect rect = { x, y, width, height };
    SDL_RenderTexture( renderer, this->m_texture, nullptr, &rect );
}

SDL_Texture *Engine::Texture::texture() const noexcept {
    return this->m_texture;
}
uint32_t Engine::Texture::width() const noexcept { return this->m_width; }
uint32_t Engine::Texture::height() const noexcept { return this->m_height; }
