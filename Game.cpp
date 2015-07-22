#include <cstdio>
#include <string>
#include <vector>

#include <SDL_mixer.h>

#include "constants.hpp"
#include "Engine.hpp"
#include "Game.hpp"
#include "LevelGenerator.hpp"

Game::Game()
{
    mGameWindow = nullptr;
    mGameRenderer = nullptr;
    mGameMusic = nullptr;
    mPlayer = nullptr;

    mCamera = { 0, 0, 0, 0 };

    mStarted = false;
    mPaused = false;

    mBackgroundClipRect = { 0, 0, 0, 0 };
    mTopPipeClipRect = { 0, 0, 0, 0 };
    mBotPipeClipRect = { 0, 0, 0, 0 };
}

Game::~Game()
{
    quit();
}

void Game::quit()
{
    // If game is not initialized
    if( !mInitialized )
    {
        mSpriteSheetTexture.free();
        mStartScreenTexture.free();
        mPauseTexture.free();
        mDeadTexture.free();

        Mix_FreeMusic( mGameMusic );

        SDL_DestroyRenderer( mGameRenderer );
        SDL_DestroyWindow( mGameWindow );
        mGameRenderer = nullptr;
        mGameWindow = nullptr;

        mInitialized = false;
    }
}

bool Game::init()
{
    // Success flag
    bool success = true;

    // Attempt to create window for game
    mGameWindow = SDL_CreateWindow( "Flappy Clone!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( mGameWindow == nullptr )
    {
        printf( "Could not create window for game! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Attempt to create renderer for game window
        mGameRenderer = SDL_CreateRenderer( mGameWindow, -1, SDL_RENDERER_ACCELERATED  );
        if( mGameRenderer == nullptr )
        {
            printf( "Could not create renderer for game window! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor( mGameRenderer, 0xff, 0xff, 0xff, 0xff );

            // Initialize camera
            mCamera.x = 0;
            mCamera.y = 0;
            mCamera.w = SCREEN_WIDTH;
            mCamera.h = SCREEN_HEIGHT;

            // Attempt to create level
            if( !createLevel() )
            {
                printf( "Could not create level!\n " );
                success = false;
            }

            // Start timer and pause it until game starts
            mGameTimer.start();
            mGameTimer.pause();
            mStarted = false;
            mPaused = false;
            //mPlayer = new Player( this, mGameTimer.getTicks() );
            //mLastCamMove = mGameTimer.getTicks();

            // Attempt to load media
            if( !loadMedia() )
            {
                printf( "Could not load media!\n " );
                success = false;
            }
        }
    }

    mInitialized = true;

    return success;
}

bool Game::loadMedia()
{
    // Success flag
    bool success = true;

    if( !mSpriteSheetTexture.loadFromFile( mGameRenderer, "assets/sprite_sheet.png" ) )
    {
        printf( "Could not load sprite sheet texture!\n" );
        success = false;
    }

    if( !mPauseTexture.loadFromFile( mGameRenderer, "assets/pause_screen.png" ) )
    {
        printf( "Could not load pause screen texture!\n" );
        success = false;
    }
    else
    {
        mPauseTexture.setBlendMode( SDL_BLENDMODE_BLEND );
        mPauseTexture.setAlpha( 126 );
    }

    if( !mStartScreenTexture.loadFromFile( mGameRenderer, "assets/start_screen.png" ) )
    {
        printf( "Could not load start screen texture!\n" );
        success = false;
    }

    if( !mDeadTexture.loadFromFile( mGameRenderer, "assets/dead.png" ) )
    {
        printf( "Could not load dead texture!\n" );
        success = false;
    }
    else
    {
        mDeadTexture.setBlendMode( SDL_BLENDMODE_BLEND );
        mDeadTexture.setAlpha( 126 );
    }

    mGameMusic = Mix_LoadMUS( "assets/rollin.mp3" );
    if( mGameMusic == nullptr )
    {
        printf( "Could not load game music! Mix_Error: %s\n", Mix_GetError() );
        success = false;
    }
    // Set clip rectangles
    mBackgroundClipRect.x = 0;
    mBackgroundClipRect.y = 0;
    mBackgroundClipRect.w = 288;
    mBackgroundClipRect.h = 512;

    mTopPipeClipRect.x = 112;
    mTopPipeClipRect.y = 645;
    mTopPipeClipRect.w = 52;
    mTopPipeClipRect.h = 321;

    mBotPipeClipRect.x = 168;
    mBotPipeClipRect.y = 646;
    mBotPipeClipRect.w = 52;
    mBotPipeClipRect.h = 320;

    return success;
}

bool Game::createLevel()
{
    mPipes = mLevelGen.generate();

    // Returns whether level was created
    return !mPipes.empty();
}

void Game::pause()
{
    mPaused = true;
    mGameTimer.pause();
}

void Game::unpause()
{
    mPaused = false;
    mGameTimer.unpause();
}

void Game::run()
{
    // Game loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Wait for player to start game
    while( !mStarted )
    {
        SDL_SetRenderDrawColor( mGameRenderer, 0xff, 0xff, 0xff, 0xff );
        SDL_RenderClear( mGameRenderer );

        SDL_WaitEvent( &e );

        if( e.type == SDL_QUIT )
        {
            quit = true;
            break;
        }

        if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE )
        {
            mStarted = true;
            unpause();
        }

        mStartScreenTexture.renderStretched( mGameRenderer, 0, 0, &FULL_SCREEN_STRETCH_RECT );

        SDL_RenderPresent( mGameRenderer );
    }

    mPlayer = new Player( this, mGameTimer.getTicks() );

    //Mix_PlayMusic( mGameMusic, -1 );

    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE )
            {
                if( mPaused )
                {
                    unpause();
                }
                else
                {
                    pause();
                }
            }

            if( !mPaused && mPlayer->isAlive() )
                mPlayer->handleEvent( e );
        }

        render();

        while( !mPlayer->isAlive() && !quit )
        {
            SDL_WaitEvent( &e );

            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE )
            {
                restart();
            }

            if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE )
            {
                quit = true;
            }
        }

        int currentTime = mGameTimer.getTicks();

        if( !mPaused && mPlayer->isAlive() )
        {
            mPlayer->move( mPipes, currentTime );
            moveCamera();
        }
    }

    this->quit();
}

void Game::render()
{
    // Clear the screen
    SDL_SetRenderDrawColor( mGameRenderer, 0xff, 0xff, 0xff, 0xff );
    SDL_RenderClear( mGameRenderer );

    mSpriteSheetTexture.renderStretched( mGameRenderer, 0, 0, &FULL_SCREEN_STRETCH_RECT, &mBackgroundClipRect );

    // Set color for pipe rendering
    SDL_Color pipeColor = Pipe::PIPE_COLOR;
    SDL_SetRenderDrawColor( mGameRenderer, pipeColor.r, pipeColor.g, pipeColor.b, pipeColor.a );

    for( std::vector<Pipe>::iterator iter = mPipes.begin(); iter != mPipes.end(); ++iter )
    {
        SDL_Rect topRect = iter->getTopRect();
        if( topRect.x >= mCamera.x - topRect.w && topRect.x <= mCamera.x + mCamera.w )
        {
            iter->render( mGameRenderer, mCamera, mSpriteSheetTexture, mTopPipeClipRect, mBotPipeClipRect );
        }
    }

    mPlayer->renderScore();

    if( mPlayer->isAlive() )
    {
        mPlayer->render( mCamera.x, mCamera.y );

        if( mPaused )
        {
            mPauseTexture.renderStretched( mGameRenderer, 0, 0, &FULL_SCREEN_STRETCH_RECT );
        }
    }
    else
    {
        mDeadTexture.renderStretched( mGameRenderer, 0, 0, &FULL_SCREEN_STRETCH_RECT );
    }

    /* TRAJECTORY DRAWING
    drawPoints( mGameRenderer, mPlayer->gPoints, mCamera );
    */
    SDL_RenderPresent( mGameRenderer );
}

void Game::restart()
{
    mPipes.clear();
    if( !createLevel() )
    {
        printf( "Failed to create new level!\n" );
    }

    delete mPlayer;

    mGameTimer.reset();

    mPlayer = new Player( this, mGameTimer.getTicks() );

    mCamera.x = 0; mCamera.y = 0;
}

void Game::moveCamera()
{
    mCamera.x = mPlayer->getCollider().x - Player::PLAYER_CAMERA_OFFSET;
}

Uint32 Game::getTicks() const
{
    return mGameTimer.getTicks();
}

SDL_Renderer* Game::getRenderer() const
{
    return mGameRenderer;
}

LTexture Game::getSpriteSheet() const
{
    return mSpriteSheetTexture;
}

const std::vector<Pipe> Game::getPipes()
{
    return mPipes;
}


/*
void drawPoints( SDL_Renderer* r, std::vector<SDL_Point>& gPoints, SDL_Rect camera )
{
    for( auto i : gPoints )
    {
        SDL_RenderDrawPoint( r, i.x - camera.x, i.y + BIRD_LENGTH / 2 - camera.y );
    }
}*/
