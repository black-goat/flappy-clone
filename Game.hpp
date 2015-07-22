#ifndef _GAME_HPP_INCLUDED
#define _GAME_HPP_INCLUDED

#include <cstdio>
#include <string>
#include <vector>

#include <SDL_mixer.h>

#include "LTexture.hpp"
#include "LTimer.hpp"
#include "LevelGenerator.hpp"
#include "Player.hpp"

class Player;

class Game{

public:

    // Initializes internal variables
    Game();

    // Frees allocated memory
    ~Game();

    // Creates window and renderer and initializes camera position
    bool init();

    // Starts game simulation
    void run();

    void pause();
    void unpause();

    // Gets ticks of game timer ( in milliseconds )
    Uint32 getTicks() const;

    // Gets game renderer
    SDL_Renderer* getRenderer() const;

    // Frees allocated memory and shuts down SDL subsystems
    void quit();

    // Creates level using level generator. Returns path to level file
    bool createLevel();

    LTexture getSpriteSheet() const;

    const std::vector<Pipe> getPipes();

private:
    // The position of the game camera
    SDL_Rect mCamera;

    // Loads required media for game
    bool loadMedia();

    // Game state flag
    bool mStarted;
    bool mPaused;

    // Renders game objects
    void render();

    // Moves camera position based on player position
    void moveCamera();

    // Reinitializes game variables and restarts game
    void restart();

    // Timer used in game simulation calculations
    LTimer mGameTimer;

    // Window and renderer for game
    SDL_Window* mGameWindow;
    SDL_Renderer* mGameRenderer;

    // The player entity
    Player* mPlayer;

    // Objects go here
    std::vector<Pipe> mPipes;

    // The level generator for the game
    LevelGenerator mLevelGen;

    // Textures needed for game
    LTexture mSpriteSheetTexture;
    LTexture mStartScreenTexture;
    LTexture mPauseTexture;
    LTexture mDeadTexture;

    // Clip rectangles for texture clipping
    SDL_Rect mBackgroundClipRect;
    SDL_Rect mTopPipeClipRect;
    SDL_Rect mBotPipeClipRect;

    // Is game initialized flag
    bool mInitialized;

    // The music for the game
    Mix_Music* mGameMusic;
};

void drawPoints( SDL_Renderer* r, std::vector<SDL_Point>& gPoints, SDL_Rect camera );

#endif // _GAME_HPP_INCLUDED
