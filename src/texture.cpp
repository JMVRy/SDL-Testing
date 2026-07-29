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

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include "texture.hpp"

Engine::Texture::Texture( SDL_Texture *texture ) noexcept
    : m_texture( texture, SDL_DestroyTexture ) {
    float width = 0.0f;
    float height = 0.0f;
    SDL_GetTextureSize( texture, &width, &height );
    this->m_width = static_cast<uint32_t>( width );
    this->m_height = static_cast<uint32_t>( height );
}

Engine::Texture::Texture( std::string path, SDL_Renderer *renderer ) {
    bool loaded = this->LoadNewTexture( path, renderer );
    if ( !loaded ) {
        throw Engine::TextureError( "Failed to load texture!" );
    }
}

bool Engine::Texture::LoadNewTexture( std::string path,
                                      SDL_Renderer *renderer ) noexcept {
    SDL_Texture *texture = IMG_LoadTexture( renderer, path.c_str() );
    if ( texture == nullptr ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR,
                      "Failed to load texture from '%s'! SDL_Error: %s",
                      path.c_str(), SDL_GetError() );
        return false;
    }

    this->m_texture.reset( texture );
    SDL_GetTextureSize( texture, &this->m_width, &this->m_height );
    return true;
}

void Engine::Texture::Render( SDL_Renderer *renderer ) const noexcept {
    SDL_RenderTexture( renderer, this->m_texture.get(), nullptr, nullptr );
}

void Engine::Texture::Render( SDL_Renderer *renderer, float x,
                              float y ) const noexcept {
    SDL_FRect rect = { x, y, this->m_width, this->m_height };
    SDL_RenderTexture( renderer, this->m_texture.get(), nullptr, &rect );
}

void Engine::Texture::Render( SDL_Renderer *renderer, float x, float y,
                              float width, float height ) const noexcept {
    SDL_FRect rect = { x, y, width, height };
    SDL_RenderTexture( renderer, this->m_texture.get(), nullptr, &rect );
}

const std::unique_ptr<SDL_Texture, decltype( &SDL_DestroyTexture )> &
Engine::Texture::texture() const noexcept {
    return this->m_texture;
}
uint32_t Engine::Texture::width() const noexcept { return this->m_width; }
uint32_t Engine::Texture::height() const noexcept { return this->m_height; }

bool Engine::Texture::isLoaded() const noexcept {
    return this->m_texture != nullptr;
}
