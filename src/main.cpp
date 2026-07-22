#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <iostream>

#include "window.hpp"

const uint32_t SCREEN_WIDTH = 640;
const uint32_t SCREEN_HEIGHT = 480;

int main( int argc, char *argv[] ) {
    std::cout << argc << " arguments: ";
    for ( int i = 0; i < argc; i++ ) {
        std::cout << argv[ i ];
        if ( i < argc - 1 )
            std::cout << ", ";
    }
    std::cout << '\n';

    // Initialize SDL
    if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
        // Error!
        std::cerr << "[ERROR] SDL could not initialize! " << SDL_GetError()
                  << '\n';
        return 1;
    }

    // SDL initialized, create window
    auto window = Engine::Window( "SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT,
                                  SDL_WINDOW_RESIZABLE );

    SDL_SetRenderDrawColor( window.renderer(), 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( window.renderer() );
    SDL_RenderPresent( window.renderer() );

    // Poll for quit event to make window stick around
    SDL_Event e;
    bool quit = false;
    while ( !quit ) {
        while ( SDL_PollEvent( &e ) ) {
            if ( e.type == SDL_EVENT_QUIT )
                quit = true;
        }
    }

    // We escaped the loop, user wants to quit
    window.~Window();

    // Quit SDL
    SDL_Quit();
}