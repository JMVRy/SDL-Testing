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

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
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
    std::cout << "SDL-Testing  Copyright (c) 2026  JMVRy\n";
    std::cout << "SDL-Testing comes with ABSOLUTELY NO WARRANTY.\n";
    std::cout << "SDL-Testing is free software, and you are welcome to "
                 "redistribute it under certain conditions.\n";
    std::cout << "See <https://www.gnu.org/licenses/> for more information.\n";

    std::cout << argc << " arguments: ";
    for ( int i = 0; i < argc; i++ ) {
        std::cout << argv[ i ];
        if ( i < argc - 1 )
            std::cout << ", ";
    }
    std::cout << '\n';

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
        gHelloWorld.Render( gWindow.renderer() );

        // Present to screen
        SDL_RenderPresent( gWindow.renderer() );
    }

    close();
}

bool init() {
    // Initialize SDL
    if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
        // Error!
        std::cerr << "[ERROR] SDL could not initialize! " << SDL_GetError()
                  << '\n';
        return false;
    }

    // Create window
    try {
        gWindow = Engine::Window( "SDL Testing", SCREEN_WIDTH, SCREEN_HEIGHT,
                                  SDL_WINDOW_RESIZABLE );
    } catch ( Engine::WindowError &error ) {
        std::cerr << "Failed to initialize window!\n";
        std::cerr << error.what() << '\n';
        return false;
    }

    return true;
}

bool loadMedia() {
    const char *path = "img/hello_world.bmp";

    gHelloWorld = Engine::Texture( path, gWindow.renderer() );

    return true;
}

void close() {
    // Release SDL resources while SDL is still initialized.
    gHelloWorld.~Texture();
    gWindow.~Window();

    // Quit SDL
    SDL_Quit();
}