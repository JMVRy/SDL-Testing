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

#include "window.hpp"

const uint32_t SCREEN_WIDTH = 640;
const uint32_t SCREEN_HEIGHT = 480;

Engine::Window *gWindow = nullptr;
SDL_Texture *gHelloWorld = nullptr;

bool init();
bool loadMedia();
void close();

int main( int argc, char *argv[] ) {
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
            if ( e.type == SDL_EVENT_QUIT )
                quit = true;
        }

        // Clear screen
        SDL_RenderClear( gWindow->renderer() );

        // Render texture to screen
        SDL_RenderTexture( gWindow->renderer(), gHelloWorld, nullptr, nullptr );

        // Present to screen
        SDL_RenderPresent( gWindow->renderer() );
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
        gWindow = new Engine::Window( "Render an image Tutorial", SCREEN_WIDTH,
                                      SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE );
    } catch ( Engine::WindowError &error ) {
        std::cerr << "Failed to initialize window!\n";
        std::cerr << error.what() << '\n';
        return false;
    }

    return true;
}

bool loadMedia() {
    const char *path = "img/hello_world.bmp";
    gHelloWorld = IMG_LoadTexture( gWindow->renderer(), path );
    if ( gHelloWorld == nullptr ) {
        std::cerr << "Failed to load texture into renderer! SDL_Error: "
                  << SDL_GetError() << '\n';
        return false;
    }

    return true;
}

void close() {
    // Destroy image texture
    SDL_DestroyTexture( gHelloWorld );
    gHelloWorld = nullptr;

    // Remove window
    delete gWindow;
    gWindow = nullptr;

    // Quit SDL
    SDL_Quit();
}