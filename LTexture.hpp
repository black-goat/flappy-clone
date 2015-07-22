#ifndef _LTEXTURE_HPP_INCLUDED
#define _LTEXTURE_HPP_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class LTexture{

public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    void free();

    // Renders texture at given position with optional clipping, rotation, and flip
    void render( SDL_Renderer* renderer, const int x, const int y, const SDL_Rect* clip = nullptr, const double angle = 0.0, const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE ) const;

    // Renders texture stretched out to fit given rectangle
    void renderStretched( SDL_Renderer* renderer, const int x, const int y, const SDL_Rect* stretchRect, const SDL_Rect* clip = nullptr, const double angle = 0.0, const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE ) const;

    // Loads image from given path. Returns whether load was successful. Optional color keying
    bool loadFromFile( SDL_Renderer* renderer, const std::string& path, const SDL_Color* colorKey = nullptr );

    // Creates texture from text with given font, text size, and text color
    bool loadFromRenderedText( SDL_Renderer* renderer, const std::string& text, TTF_Font* textFont, const SDL_Color textColor = { 0, 0, 0, 255 } );

    // Sets color modulation
    void setColorMod( const uint8_t r, const uint8_t g, const uint8_t b );

    // Sets opacity
    void setAlpha( const uint8_t a );

    // Sets blend mode
    void setBlendMode( const SDL_BlendMode& blend );

    // Texture dimension accessors
    int getWidth() const;
    int getHeight() const;

    //Set texture width/height for custom rendering dimensions
    // void setWidth( int w );
    // void setHeight( int h );

private:
    // The hardware texture
    SDL_Texture* mTexture;

    // The texture dimensions
    int mWidth, mHeight;
};

#endif // _LTEXTURE_HPP_INCLUDED
