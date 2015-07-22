#include <cstdio>
#include <string>
#include <random>
#include <vector>
#include <functional>
#include <ctime>

#include <SDL.h>

#include "LevelGenerator.hpp"
#include "constants.hpp"

Pipe::Pipe( int x, int y, int w, int h )
{
    mTopRect.x = x;
    mTopRect.y = y;
    mTopRect.w = w;
    mTopRect.h = h;

    mBottomRect.x = x;
    mBottomRect.y = mTopRect.y + mTopRect.h + PIPE_GAP;
    mBottomRect.w = w;
    mBottomRect.h = SCREEN_HEIGHT - mBottomRect.y;
}

SDL_Rect Pipe::getTopRect() const
{
    return mTopRect;
}

SDL_Rect Pipe::getBotRect() const
{
    return mBottomRect;
}

void Pipe::render( SDL_Renderer* renderer, const SDL_Rect camera, LTexture& srcTexture, SDL_Rect topClip, SDL_Rect botClip ) const
{
    SDL_Rect renderRectTop = { mTopRect.x - camera.x, mTopRect.y - camera.y, mTopRect.w, mTopRect.h };
    SDL_Rect renderRectBot = { mBottomRect.x - camera.x, mBottomRect.y - camera.y, mBottomRect.w, mBottomRect.h };

    if( mTopRect.h < topClip.h )
    {
        topClip.y = topClip.y + topClip.h - mTopRect.h;
        topClip.h = mTopRect.h;
    }
    if( mBottomRect.h < botClip.h )
    {
        botClip.h = mBottomRect.h;
    }

    srcTexture.renderStretched( renderer, renderRectTop.x, renderRectTop.y, &renderRectTop, &topClip );
    srcTexture.renderStretched( renderer, renderRectBot.x, renderRectBot.y, &renderRectBot, &botClip );
}

std::vector<Pipe> LevelGenerator::generate( const int seed )
{
    std::vector<Pipe> level;

    std::mt19937 generatorRandomEngine( seed );

    std::uniform_int_distribution<int> generatorDist( PIPE_MIN_HEIGHT, PIPE_MAX_HEIGHT );
    auto pipeGen = std::bind( generatorDist, std::ref(generatorRandomEngine) );

    int currentPipePosX = STARTING_OFFSET;
    for( int i = 0; i < NUM_OBSTACLES; ++i )
    {
        int currentPipeHeight = pipeGen();
        level.push_back( Pipe( currentPipePosX, 0, BLOCK_WIDTH, currentPipeHeight ) );
        currentPipePosX += 3 * BLOCK_WIDTH;
    }

    return level;
}

std::vector<Pipe> LevelGenerator::generate()
{
    std::vector<Pipe> level;

    std::mt19937 generatorRandomEngine( time( nullptr ) );

    std::uniform_int_distribution<int> generatorDist( PIPE_MIN_HEIGHT, PIPE_MAX_HEIGHT );
    auto pipeGen = std::bind( generatorDist, std::ref(generatorRandomEngine) );

    int currentPipePosX = STARTING_OFFSET;
    for( int i = 0; i < NUM_OBSTACLES; ++i )
    {
        int currentPipeHeight = pipeGen();
        level.push_back( Pipe( currentPipePosX, 0, BLOCK_WIDTH, currentPipeHeight ) );
        currentPipePosX += 3 * BLOCK_WIDTH;
    }

    return level;
}

