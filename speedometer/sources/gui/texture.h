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

        void render( int x, int y, int width, int height,  SDL_Renderer* gRenderer, double angle );
        int getWidth();
        int getHeight();

        SDL_Texture* operator()();


    private:
        void free();
        SDL_Texture* m_texture;

        int m_width;
        int m_height;
};
