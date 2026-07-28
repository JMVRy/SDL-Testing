/*
    SDL Testing - a test program to test out SDL's capabilities
    Copyright (C) 2026  JMVRy

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

#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <SDL3_image/SDL_image.h>

#include "texture.hpp"
#include "window.hpp"

static uint32_t SCREEN_WIDTH = 640;
static uint32_t SCREEN_HEIGHT = 480;

Engine::Window gWindow;
Engine::Texture gHelloWorld;

bool init();
bool loadMedia();
void close();

int main( int argc, char *argv[] ) {
    SDL_Log( "SDL-Testing  Copyright (c) 2026  JMVRy" );
    SDL_Log( "SDL-Testing comes with ABSOLUTELY NO WARRANTY." );
    SDL_Log( "SDL-Testing is free software, and you are welcome to "
             "redistribute it under certain conditions." );
    SDL_Log( "See <https://www.gnu.org/licenses/> for more information." );

    std::stringstream ss;
    ss << argc << " arguments: ";
    for ( int i = 0; i < argc; i++ ) {
        ss << argv[ i ];
        if ( i < argc - 1 )
            ss << ", ";
    }
    SDL_Log( "%s", ss.str().c_str() );

    // Initialize SDL
    if ( !init() )
        return 1;

    // Load media
    if ( !loadMedia() )
        return 2;

    // Poll for quit event to make window stick around
    SDL_Event e;
    bool quit = false;
    while ( !quit ) {
        while ( SDL_PollEvent( &e ) ) {
            switch ( e.type ) {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                SCREEN_WIDTH = e.window.data1;
                SCREEN_HEIGHT = e.window.data2;
                break;
            }
        }

        // Clear screen
        SDL_RenderClear( gWindow.renderer() );

        // Render texture to screen
        gHelloWorld.Render( gWindow.renderer(), SCREEN_WIDTH / 4.0f,
                            SCREEN_HEIGHT / 4.0f, SCREEN_WIDTH / 2.0f,
                            SCREEN_HEIGHT / 2.0f );

        // Present to screen
        SDL_RenderPresent( gWindow.renderer() );
    }

    close();
}

bool init() {
    // Initialize SDL
    if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
        // Error!
        SDL_LogError( SDL_LOG_CATEGORY_ERROR,
                      "SDL could not initialize! SDL_Error: %s",
                      SDL_GetError() );
        return false;
    }

    // Create window
    try {
        gWindow = Engine::Window( "SDL Testing", SCREEN_WIDTH, SCREEN_HEIGHT,
                                  SDL_WINDOW_RESIZABLE );
    } catch ( Engine::WindowError &error ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to initialize window!" );
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "%s", error.what() );
        return false;
    }

    return true;
}

bool loadMedia() {
    bool success = true;
    const char *path = "img/hello_world.bmp";

    try {
        gHelloWorld = Engine::Texture( path, gWindow.renderer() );
    } catch ( Engine::TextureError &err ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "gHelloWorld texture failed: %s",
                      err.what() );
        success = false;
    }

    return success;
}

void close() {
    gHelloWorld = Engine::Texture();
    gWindow = Engine::Window();

    SDL_Quit();
}