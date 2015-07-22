#include <cstdio>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "LTexture.hpp"

LTexture::LTexture()
{
    mTexture = nullptr;

    mWidth = 0; mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

void LTexture::free()
{
    if( mTexture != nullptr )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( SDL_Renderer* renderer, const int x, const int y, const SDL_Rect* clip, const double angle, const SDL_Point* center, const SDL_RendererFlip flip ) const
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    // If clipping is used resize renderQuad so it has the dimensions of the clip ( so no image stretching occurs )
    if( clip != nullptr )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::renderStretched( SDL_Renderer* renderer, const int x, const int y, const SDL_Rect* stretchRect, const SDL_Rect* clip, const double angle, const SDL_Point* center, const SDL_RendererFlip flip ) const
{
    SDL_Rect renderQuad = { x, y, stretchRect->w, stretchRect->h };

    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

bool LTexture::loadFromFile( SDL_Renderer* renderer, const std::string& path, const SDL_Color* colorKey )
{
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr )
    {
        printf( "Could not load surface from path %s ! IMG_Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        // If color key was requested
        if( colorKey != nullptr )
        {
            // Attempt to set color key
            if( SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, colorKey->r, colorKey->g, colorKey->b ) ) != 0 )
            {
                printf( "Could not set color key! SDL_Error: %s\n", SDL_GetError() );
            }
        }
        else
        {
            // Attempt to create a texture from our loaded surface
            mTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
            if( mTexture == nullptr )
            {
                printf( "Could not create texture from surface %s ! SDL_Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
        }
        // Free unnecessary surface
        SDL_FreeSurface( loadedSurface );
    }

    return mTexture != nullptr;
}

bool LTexture::loadFromRenderedText( SDL_Renderer* renderer, const std::string& text, TTF_Font* textFont, const SDL_Color textColor )
{
    // Attempt to create surface from given text
    SDL_Surface* loadedSurface = TTF_RenderText_Solid( textFont, text.c_str(), textColor );
    if( loadedSurface == nullptr )
    {
        printf( "Could not create surface from text %s ! TTF_Error: %s\n", text.c_str(), TTF_GetError() );
    }
    else
    {
        // Attempt to create texture from loaded surface
        mTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( mTexture == nullptr )
        {
            printf( "Could not create texture from rendered text %s ! SDL_Error: %s\n", text.c_str(), SDL_GetError() );
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        // Deallocate loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return mTexture != nullptr;
}

void LTexture::setColorMod( const uint8_t r, const uint8_t g, const uint8_t b )
{
    SDL_SetTextureColorMod( mTexture, r, g, b );
}

void LTexture::setAlpha( const Uint8 a )
{
    SDL_SetTextureAlphaMod( mTexture, a );
}

void LTexture::setBlendMode( const SDL_BlendMode& blend )
{
    SDL_SetTextureBlendMode( mTexture, blend );
}

int LTexture::getWidth() const
{
    return mWidth;
}

int LTexture::getHeight() const
{
    return mHeight;
}


