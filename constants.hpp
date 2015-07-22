#ifndef _CONSTANTS_HPP_INCLUDED
#define _CONSTANTS_HPP_INCLUDED

#include <SDL.h>
#include <vector>

const int BIRD_LENGTH = 68;

const int SCREEN_WIDTH = BIRD_LENGTH * 7; //336;
const int SCREEN_HEIGHT = BIRD_LENGTH * 11; //480;

// Camera velocity to the left ( int px per second )
const int CAMERA_VELOCITY = BIRD_LENGTH * 2;
// Gravity
const double GRAVITY = 10.5 * BIRD_LENGTH;

const SDL_Rect FULL_SCREEN_STRETCH_RECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };




#endif // _CONSTANTS_HPP_INCLUDED
