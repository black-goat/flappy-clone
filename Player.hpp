#ifndef _PLAYER_HPP_INCLUDED
#define _PLAYER_HPP_INCLUDED

#include <string>
#include <vector>

#include "Game.hpp"
#include "LTexture.hpp"
#include "LTimer.hpp"
#include "LevelGenerator.hpp"
#include "constants.hpp"
#include "ScoreTracker.hpp"

class Game;
class ScoreTracker;

struct Circle{
    int x, y;
    int r;
};

class Player{

// static const int NUM_ANIMATION_TEXTURES = ??
// Dimensions of the player character
static const int PLAYER_WIDTH = BIRD_LENGTH;
static const int PLAYER_HEIGHT = ( BIRD_LENGTH * 24 ) / 34;

// Height ( in pixels ) of a wing flap ( increase in Y )
static const int FLAP_HEIGHT =  BIRD_LENGTH * 4.35;

// Speed at which birds rotation speed changes
static constexpr double ROTATION_SPEED = 500.f;

// Angle at which the bird is rotated to after a flap
static constexpr double ROTATION_AFTER_FLAP = -22.f;

// Duration of each frame of the flap animation in milliseconds
static const int ANIMATION_FRAME_DURATION = 60;

// Air time that a flap gives in milliseconds ( ANIMATION_FRAME_DURATION * number of animation frames )
static const int FLAP_AIR_TIME = ANIMATION_FRAME_DURATION * 8;

public:

    // How far the player is from the leftmost side of the camera
    static const int PLAYER_CAMERA_OFFSET = 2 * BIRD_LENGTH;

    // Initializes internal variables
    Player( Game* game, int currentTicks );

    // Deallocates memory
    ~Player() = default;

    // Renders player character
    void render( int camPosX, int camPosY );

    // Renders players score
    void renderScore();

    // Handles event
    void handleEvent( SDL_Event& e );

    // Moves player character. Return true if move was successful and false if there was collision ( player dies )
    bool move( const std::vector<Pipe>& pipes, const int currentTime );

    // Checks collision with set of pipes
    bool checkCollision( const Pipe& collisionPipe );

    bool isAlive() const;

    int getScore() const{ return mCurrentScore; }

    // Accessor function for the player collider
    SDL_Rect getCollider();

private:
    // The position of the player
    double mPosX, mPosY;

    // The X and Y axis velocities of the player character ( in px/s )
    double mVelX, mVelY;

    // Whether or not the player is alive
    bool mAlive;

    // The last time the player character was moved ( in milliseconds )
    int mLastMove;

    // The last time the player flapped ( pressed space ) in milliseconds
    int mLastFlap;

    // The circle used for collision detection
    SDL_Rect mCollider;

    // The angle at which to rotate the bird
    double mRotationAngle;

    // The current rotation speed of the bird
    double mRotationSpeed;

    // Shifts collider position after player moves
    void shiftCollider();

    // The sprite sheet for the player character
    LTexture mSpriteSheet;

    // The texture of the player character
    SDL_Rect mPlayerTextureClip;

    // For stretching the bird at higher resolutions
    SDL_Rect mPlayerTextureStretchRect;

    // The game in which the player was created
    Game* mGamePointer;

    // The players current score
    int mCurrentScore;

    // The score tracker for the player
    ScoreTracker* mScoreTracker;

    // Updates player score
    void updateScore();

    enum PlayerTexture{ FLAP_NEUTRAL = 0, FLAP_UP, FLAP_DOWN, FLAP_TOTAL  };

    // The set of textures required for animating the player character
    std::vector<SDL_Rect> mAnimationClips;

    enum SoundEffects{ SFX_FLAP = 0, SFX_GET_POINT, SFX_HIT, SFX_DIE, SFX_TOTAL };

    // The set of sound effects
    std::vector<Mix_Chunk*> mSoundEffects;

public:
    /* TRAJECTORY
    std::vector<SDL_Point> gPoints;
    */
};

#endif // _PLAYER_HPP_INCLUDED
