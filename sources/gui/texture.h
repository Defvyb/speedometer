#pragma once

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

class Texture final
{
    public:
        Texture();
        ~Texture();

        bool loadFromFile( SDL_Renderer* gRenderer, const std::string & path );
        bool loadFromRenderedText( TTF_Font *font, SDL_Renderer* renderer, std::string textureText, SDL_Color textColor );

        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        void setBlendMode( SDL_BlendMode blending );
        void setAlpha( Uint8 alpha );
        void render( int x, int y, int width, int height, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int getWidth();
        int getHeight();

        SDL_Texture* operator()();


    private:
        void free();
        SDL_Texture* m_texture;

        int m_width;
        int m_height;
};
