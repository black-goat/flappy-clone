#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <functional>
#include <random>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "LTexture.hpp"
#include "LTimer.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "LevelGenerator.hpp"

bool init_SDL();
void close_SDL();

int main( int argc, char** argv )
{
    if( !init_SDL() )
    {
        printf( "Could not initialize SDL!\n" );
    }
    else
    {
        Game myGame;
        std::cout << "Game created!\n";
        if( !myGame.init() )
        {
            std::cout << "Could not create game!\n";
        }
        else
        {
            myGame.run();
        }
    }

    close_SDL();

    return 0;
}

bool init_SDL()
{
    // Success flag
    bool success = true;

    // Initialize video rendering, timer and audio
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if( ( IMG_Init( imgFlags ) & imgFlags ) != imgFlags )
        {
            printf( "Could not initialize PNG and JPEG loading! IMG_Error: %s\n", IMG_GetError() );
            success = false;
        }
        else
        {
            if( TTF_Init() == -1 )
            {
                printf( "SDL_TTF could not initialize! TTF_Error: %s\n", TTF_GetError() );
                success = false;
            }
        }

        // Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL Mixer could not initialize! Mix_Error: %s\n", Mix_GetError() );
            success = false;
        }
    }

    return success;
}

void close_SDL()
{
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
