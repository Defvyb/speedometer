#include "texture.h"
#include <commonLib/helpers.h>


Texture::~Texture(){
    free();
}

void Texture::free(){
    if(m_texture != NULL){
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
        m_width = 0;
        m_height = 0;
    }
}

void Texture::render(int x, int y, int width, int height,  SDL_Renderer* gRenderer, double angle){
    SDL_Rect renderQuad = { x, y, width, height };
    SDL_RenderCopyEx(gRenderer, m_texture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
}

int Texture::getWidth() const{
    return m_width;
}

int Texture::getHeight() const{
    return m_height;
}

SDL_Texture* Texture::operator()(){
    return m_texture;
}

bool Texture::loadFromFile(SDL_Renderer* renderer, const std::string& path){
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        dumpError("Unable to load image", IMG_GetError());
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == NULL){
        dumpError("Unable to create texture", SDL_GetError());
    }
    else{
        m_width = loadedSurface->w;
        m_height = loadedSurface->h;
    }

    SDL_FreeSurface(loadedSurface);

    m_texture = newTexture;
    return m_texture != NULL;
}

bool Texture::loadFromRenderedText(TTF_Font *font, SDL_Renderer* renderer, const std::string & textureText, SDL_Color textColor)
{
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if(textSurface == NULL){
        dumpError("Unable to render text surface!", TTF_GetError());
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(m_texture == NULL){
        dumpError("Unable to create texture from rendered text!", SDL_GetError());
    }
    else{
        m_width = textSurface->w;
        m_height = textSurface->h;
    }

    SDL_FreeSurface(textSurface);

    return m_texture != NULL;
}
