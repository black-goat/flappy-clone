#ifndef _LEVELGENERATOR_HPP_INCLUDED
#define _LEVELGENERATOR_HPP_INCLUDED

#include <cstdio>
#include <random>
#include <string>
#include <SDL.h>

#include "constants.hpp"
#include "LTexture.hpp"

const int NUM_OBSTACLES = 1024;
// Width of 1 unit of ground
const int BLOCK_WIDTH = BIRD_LENGTH;
// Gap between two opposing pipes
const int PIPE_GAP = BIRD_LENGTH * 2.5;
// Minimum height of a single pipe
const int PIPE_MIN_HEIGHT = BIRD_LENGTH;
// Maximum height of a single pipe
const int PIPE_MAX_HEIGHT = SCREEN_HEIGHT / 2 - PIPE_GAP / 2;
// Maximum Y offset of a pipe
const int PIPE_MAX_Y = SCREEN_HEIGHT - BIRD_LENGTH;
// Space for "warmup" before pipes start appearing
const int STARTING_OFFSET = 15 * BIRD_LENGTH;

class Pipe{

public:

    static constexpr SDL_Color PIPE_COLOR = { 255, 0, 0, 255 };

    Pipe( int x, int y, int w, int h );

    // Renders pipes at their coordinates ( with their dimensions ) using provided texture with optional clipping
    void render( SDL_Renderer* renderer, const SDL_Rect camera, LTexture& stcTexture, SDL_Rect topClip, SDL_Rect botClip ) const;

    SDL_Rect getTopRect() const;
    SDL_Rect getBotRect() const;

private:

    SDL_Rect mTopRect;
    SDL_Rect mBottomRect;
};

class LevelGenerator{

public:

    LevelGenerator() = default;

    ~LevelGenerator() = default;

    // Generates level ( with seed )
    std::vector<Pipe> generate( const int seed );

    // Generate level ( without seed )
    std::vector<Pipe> generate();

private:


};

#endif // _LEVELGENERATOR_HPP_INCLUDED
