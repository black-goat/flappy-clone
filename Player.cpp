#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <iomanip>

#include <SDL.h>

#include "LTexture.hpp"
#include "LTimer.hpp"
#include "constants.hpp"
#include "Player.hpp"
#include "CollisionDetection.hpp"
#include "LevelGenerator.hpp"
#include "ScoreTracker.hpp"

Player::Player( Game* game, int currentTicks )
{
    mPosX = PLAYER_CAMERA_OFFSET;
    mPosY = SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2;

    mVelY = 0.0; mVelX = CAMERA_VELOCITY;

    mLastMove = currentTicks;

    mLastFlap = currentTicks;

    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    mGamePointer = game;

    if( !mSpriteSheet.loadFromFile( mGamePointer->getRenderer(), "assets/sprite_sheet.png" ) )
    {
        printf( "Could not load player sprite sheet!\n" );
    }

    mPlayerTextureClip.x = 6;
    mPlayerTextureClip.y = 982;
    mPlayerTextureClip.w = 34;
    mPlayerTextureClip.h = 24;

    mPlayerTextureStretchRect.x = mPosX;
    mPlayerTextureStretchRect.y = mPosY;
    mPlayerTextureStretchRect.w = BIRD_LENGTH;
    mPlayerTextureStretchRect.h = ( BIRD_LENGTH * 24 ) / 34;

    mRotationAngle = 0.0;

    mRotationSpeed = ROTATION_SPEED;

    mCurrentScore = 0;

    mAlive = true;

    mScoreTracker = new ScoreTracker( mGamePointer, this );

    mAnimationClips.reserve( FLAP_TOTAL );

    // Set animation neutral clip rect
    mAnimationClips[ FLAP_NEUTRAL ].x = 62;
    mAnimationClips[ FLAP_NEUTRAL ].y = 982;
    mAnimationClips[ FLAP_NEUTRAL ].w = 34;
    mAnimationClips[ FLAP_NEUTRAL ].h = 24;

    // Set animation flapping up clip rect
    mAnimationClips[ FLAP_UP ].x = 6;
    mAnimationClips[ FLAP_UP ].y = 982;
    mAnimationClips[ FLAP_UP ].w = 34;
    mAnimationClips[ FLAP_UP ].h = 24;

    // Set animation flapping down clip rect
    mAnimationClips[ FLAP_DOWN ].x = 118;
    mAnimationClips[ FLAP_DOWN ].y = 982;
    mAnimationClips[ FLAP_DOWN ].w = 34;
    mAnimationClips[ FLAP_DOWN ].h = 24;


    // Load sound effects
    mSoundEffects.reserve( SFX_TOTAL );

    mSoundEffects[ SFX_FLAP ] = Mix_LoadWAV( "assets/sounds/sfx_wing.ogg" );
    if( mSoundEffects[ SFX_FLAP ] == nullptr )
    {
        printf( "Could not load SFX_FLAP sound effect! Mix_Error: %s\n", Mix_GetError() );
    }

    mSoundEffects[ SFX_GET_POINT ] = Mix_LoadWAV( "assets/sounds/sfx_point.ogg" );
    if( mSoundEffects[ SFX_GET_POINT ] == nullptr )
    {
        printf( "Could not load SFX_GET_POINT sound effect! Mix_Error: %s\n", Mix_GetError() );
    }

    mSoundEffects[ SFX_HIT ] = Mix_LoadWAV( "assets/sounds/sfx_hit.ogg" );
    if( mSoundEffects[ SFX_HIT ] == nullptr )
    {
        printf( "Could not load SFX_HIT sound effect! Mix_Error: %s\n", Mix_GetError() );
    }

    mSoundEffects[ SFX_DIE ] = Mix_LoadWAV( "assets/sounds/sfx_die.ogg" );
    if( mSoundEffects[ SFX_DIE ] == nullptr )
    {
        printf( "Could not load SFX_DIE sound effect! Mix_Error: %s\n", Mix_GetError() );
    }
}

void Player::render( int camPosX, int camPosY )
{
    mSpriteSheet.renderStretched( mGamePointer->getRenderer(), mPosX - camPosX, mPosY - camPosY, &mPlayerTextureStretchRect, &mPlayerTextureClip, mRotationAngle );
}

void Player::handleEvent( SDL_Event& e )
{
    if( e.type == SDL_KEYDOWN )
    {
        if( e.key.keysym.sym == SDLK_SPACE )
        {
            // Adjust player velocity
            mVelY = -FLAP_HEIGHT;
            // Reset player rotation speed to return him to neutral position fast
            mRotationSpeed = -500;
            // Set player rotation to neutral
            mRotationAngle = ROTATION_AFTER_FLAP;
            // Record last flap time
            mLastFlap = mGamePointer->getTicks();
            // Play flap sound effect
            Mix_PlayChannel( -1, mSoundEffects[ SFX_FLAP ], 0 );
        }
    }
}

bool Player::checkCollision( const Pipe& collisionPipe )
{
    // If collides with top pipe
    if( CD_checkCollision( mCollider, collisionPipe.getTopRect() ) )
    {
        return true;
    }
    // If collides with bottom pipe
    if( CD_checkCollision( mCollider, collisionPipe.getBotRect() ) )
    {
        return true;
    }

    // If no collision was detected
    return false;
}

bool Player::move( const std::vector<Pipe>& pipes, const int currentTime )
{
    // Get time passed in milliseconds
    double timePassed = double ( currentTime - mLastMove );
    // Adjust time passed to seconds
    timePassed /= 1000.f;

    /* FOR TRAJECTORY DRAWING
    gPoints.push_back( SDL_Point{ mPosX, mPosY } );
    */

    mPosX += mVelX * timePassed;

    mPosY = mPosY + ( GRAVITY * timePassed * timePassed ) + mVelY * timePassed;

    mVelY = mVelY + GRAVITY * timePassed;

    // If flap is finished start rotating
    if( mGamePointer->getTicks() - mLastFlap > FLAP_AIR_TIME )
    {
        mRotationAngle += mRotationSpeed * timePassed;
    }

    if( mRotationAngle < ROTATION_AFTER_FLAP )
    {
        mRotationAngle = ROTATION_AFTER_FLAP;
        mRotationSpeed = 0;
    }
    if( mRotationAngle > 90 )
    {
        mRotationAngle = 90;
    }

    mRotationSpeed += ROTATION_SPEED * timePassed;

    // Check collision with new player position
    for( size_t i = 0; i < pipes.size(); ++i )
    {
        if( checkCollision( pipes[ i ] ) && mAlive == true )
        {
            Mix_PlayChannel( -1, mSoundEffects[ SFX_HIT ], 0 );
            Mix_PlayChannel( -1, mSoundEffects[ SFX_DIE ], 0 );
            mAlive = false;
            break;
        }
    }

    if( mPosY < 0 /*- mPlayerTextureStretchRect.h / 2*/ )
    {
        mPosY = 0/* - mPlayerTextureStretchRect.h*/;
    }
    if( mPosY + PLAYER_HEIGHT > SCREEN_HEIGHT )
    {
        mAlive = false;
    }

    mLastMove = currentTime;
    shiftCollider();

    // Update the players score after move
    updateScore();
    mScoreTracker->updateScore();

    // Choose texture to display
    switch( ( mGamePointer->getTicks() - mLastFlap ) / ANIMATION_FRAME_DURATION )
    {
        case 0:
            mPlayerTextureClip = mAnimationClips[ FLAP_UP ];
            break;
        case 1:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        case 2:
            mPlayerTextureClip = mAnimationClips[ FLAP_DOWN ];
            break;
        case 3:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        case 4:
            mPlayerTextureClip = mAnimationClips[ FLAP_UP ];
            break;
        case 5:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        case 6:
            mPlayerTextureClip = mAnimationClips[ FLAP_DOWN ];
            break;
        case 7:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        /*
        case 8:
            mPlayerTextureClip = mAnimationClips[ FLAP_UP ];
            break;
        case 9:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        case 10:
            mPlayerTextureClip = mAnimationClips[ FLAP_DOWN ];
            break;
        case 11:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
        */
        default:
            mPlayerTextureClip = mAnimationClips[ FLAP_NEUTRAL ];
            break;
    }


    // Return whether player is alive
    return mAlive;
}

void Player::shiftCollider()
{
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

SDL_Rect Player::getCollider()
{
    return mCollider;
}

bool Player::isAlive() const
{
    return mAlive;
}

void Player::updateScore()
{
    std::vector<Pipe> pipes = mGamePointer->getPipes();
    int score = 0;
    for( auto p : pipes )
    {
        if( p.getTopRect().x < mPosX )
            ++score;
        else
            break;
    }

    if( score > mCurrentScore )
    {
        Mix_PlayChannel( -1, mSoundEffects[ SFX_GET_POINT ], 0 );
    }

    mCurrentScore = score;
}

void Player::renderScore()
{
    mScoreTracker->render();
}
