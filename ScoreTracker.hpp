#ifndef _SCORE_TRACKER_H_INCLUDED
#define _SCORE_TRACKER_H_INCLUDED

#include <iostream>
#include <fstream>

#include <SDL.h>

#include "Game.hpp"
#include "Player.hpp"

class Player;
class Game;

class ScoreTracker{

static const int PLAYER_SCORE_OFFSET = BIRD_LENGTH / 2;

public:
    // Initializes internal variables and loads textures for bitmaping
    ScoreTracker( Game* game, const Player* player );

    // Frees allocated memory
    ~ScoreTracker() = default;

    // Renders current score to screen
    void render();

    // Updates current score
    void updateScore();

private:
    // The players score
    int mScore;
    // The maximum score the player has achieved
    int mMaxScore;

    // Enum indicating which clip texture is used for clipping of given number
    enum ST_NUMBER_TEXTURE{ ST_0, ST_1, ST_2, ST_3, ST_4, ST_5, ST_6, ST_7, ST_8, ST_9, ST_TOTAL };

    // Sets variables for mTextureClips for clip rendering
    void setClips();

    // Rects for texture clipping of the number for score rendering
    SDL_Rect mTextureClips[ ST_TOTAL ];

    // IO stream for reading/writing highscore
    std::fstream mFileStream;

    // Pointer to player to whom the highscore belongs to
    const Player* mPlayerPointer;

    const Game* mGamePointer;

    // Sprite sheet used for rendering score
    LTexture mSpriteSheet;

};

#endif // _SCORE_TRACKER_H_INCLUDED

