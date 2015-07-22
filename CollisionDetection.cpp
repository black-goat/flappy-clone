#include <SDL.h>

#include "CollisionDetection.hpp"

bool CD_checkCollision( const SDL_Rect& a, const SDL_Rect& b )
{
    // If a is to the left of b
    if( a.x + a.w <= b.x )
    {
        return false;
    }

    // If b is to the left of a
    if( b.x + b.w <= a.x )
    {
        return false;
    }

    // If a is higher than b
    if( a.y + a.h <= b.y )
    {
        return false;
    }

    // If b is higher than a
    if( b.y + b.h <= a.y )
    {
        return false;
    }

    // If there is collision
    return true;
}
