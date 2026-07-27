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

#include <cstdint>
#include <stdexcept>
#include <string>

#include <SDL3/SDL_render.h>

namespace Engine {
class TextureError : public std::runtime_error {
  public:
    TextureError( const char *msg ) : std::runtime_error( msg ) {}
};

class Texture {
  public:
    Texture() noexcept;
    Texture( std::string path, SDL_Renderer *renderer ) noexcept( false );
    Texture( SDL_Texture *texture ) noexcept;
    Texture( const Texture & ) = delete;
    Texture &operator=( const Texture & ) = delete;
    Texture( Texture &&other ) noexcept;
    Texture &operator=( Texture &&other ) noexcept;
    ~Texture() noexcept;

    bool LoadNewTexture( std::string path,
                         SDL_Renderer *renderer ) noexcept( false );
    void Render( SDL_Renderer *renderer ) const noexcept;
    void Render( SDL_Renderer *renderer, float x, float y ) const noexcept;
    void Render( SDL_Renderer *renderer, float x, float y, float width,
                 float height ) const noexcept;

    SDL_Texture *texture() const noexcept;
    uint32_t width() const noexcept;
    uint32_t height() const noexcept;

  private:
    SDL_Texture *m_texture = nullptr;

    float m_width = 0;
    float m_height = 0;
};
} // namespace Engine