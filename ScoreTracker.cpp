#include <fstream>
#include <iostream>
#include <vector>

#include <SDL.h>

#include "Game.hpp"
#include "ScoreTracker.hpp"
#include "Player.hpp"
#include "constants.hpp"

ScoreTracker::ScoreTracker( Game* game, const Player* player )
{
    mPlayerPointer = player;

    mGamePointer = game;

    mScore = 0;

    mFileStream.open( "highscore.hs" );

    mMaxScore = 0;

    mFileStream >> mMaxScore;

    if( !mSpriteSheet.loadFromFile( game->getRenderer(), "assets/sprite_sheet.png" ) )
    {
        printf( "Could not load player sprite sheet!\n" );
    }

    mFileStream.close();

    setClips();
}

void ScoreTracker::updateScore()
{
    mScore = mPlayerPointer->getScore();
    if( !mPlayerPointer->isAlive() )
    {
        if( mScore > mMaxScore )
        {
            mFileStream.open( "highscore.hs", std::ofstream::out );
            mFileStream << mScore;
            mMaxScore = mScore;
            mFileStream.close();
        }
    }
}

void ScoreTracker::render()
{
    // The length of the score
    int scoreDigits = 1;
    int tmpScore = mScore;
    // The digits of the score (in reverse order)
    std::vector<int> digits;
    digits.push_back( tmpScore % 10 );
    tmpScore /= 10;

    while( tmpScore > 0 )
    {
        ++scoreDigits;
        digits.push_back( tmpScore % 10 );
        tmpScore /= 10;
    }

    // Calculate total width needed to render score
    int totalWidth = 0;
    for( int i : digits )
    {
        totalWidth += mTextureClips[ i ].w;
    }
    // Center score for rendering
    int renderX = SCREEN_WIDTH / 2 - totalWidth / 2 + PLAYER_SCORE_OFFSET;
    int renderY = SCREEN_HEIGHT / 8;

    for( std::vector<int>::reverse_iterator iter = digits.rbegin(); iter != digits.rend(); ++iter )
    {
        mSpriteSheet.render( mGamePointer->getRenderer(), renderX, renderY, &mTextureClips[ *iter ] );
        renderX += mTextureClips[ *iter ].w;
    }
}

void ScoreTracker::setClips()
{
    mTextureClips[ ST_0 ].x = 992;
    mTextureClips[ ST_0 ].y = 120;
    mTextureClips[ ST_0 ].w = 25;
    mTextureClips[ ST_0 ].h = 36;

    mTextureClips[ ST_1 ].x = 272;
    mTextureClips[ ST_1 ].y = 910;
    mTextureClips[ ST_1 ].w = 16;
    mTextureClips[ ST_1 ].h = 36;

    mTextureClips[ ST_2 ].x = 584;
    mTextureClips[ ST_2 ].y = 320;
    mTextureClips[ ST_2 ].w = 24;
    mTextureClips[ ST_2 ].h = 36;

    mTextureClips[ ST_3 ].x = 612;
    mTextureClips[ ST_3 ].y = 320;
    mTextureClips[ ST_3 ].w = 24;
    mTextureClips[ ST_3 ].h = 36;

    mTextureClips[ ST_4 ].x = 640;
    mTextureClips[ ST_4 ].y = 320;
    mTextureClips[ ST_4 ].w = 24;
    mTextureClips[ ST_4 ].h = 36;

    mTextureClips[ ST_5 ].x = 668;
    mTextureClips[ ST_5 ].y = 320;
    mTextureClips[ ST_5 ].w = 24;
    mTextureClips[ ST_5 ].h = 36;

    mTextureClips[ ST_6 ].x = 584;
    mTextureClips[ ST_6 ].y = 368;
    mTextureClips[ ST_6 ].w = 24;
    mTextureClips[ ST_6 ].h = 36;

    mTextureClips[ ST_7 ].x = 612;
    mTextureClips[ ST_7 ].y = 368;
    mTextureClips[ ST_7 ].w = 24;
    mTextureClips[ ST_7 ].h = 36;

    mTextureClips[ ST_8 ].x = 640;
    mTextureClips[ ST_8 ].y = 368;
    mTextureClips[ ST_8 ].w = 24;
    mTextureClips[ ST_8 ].h = 36;

    mTextureClips[ ST_9 ].x = 668;
    mTextureClips[ ST_9 ].y = 368;
    mTextureClips[ ST_9 ].w = 24;
    mTextureClips[ ST_9 ].h = 36;
}
