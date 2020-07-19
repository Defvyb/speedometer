#pragma once

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

class Texture final
{
public:
    Texture() = default;
    ~Texture();

    Texture(Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture& operator=(Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    bool loadFromFile(SDL_Renderer* gRenderer, const std::string & path);
    bool loadFromRenderedText(TTF_Font *font,
                              SDL_Renderer* renderer,
                              const std::string & textureText,
                              SDL_Color textColor);

    void render(int x, int y, int width, int height,  SDL_Renderer* gRenderer, double angle);
    int getWidth() const;
    int getHeight() const;

    SDL_Texture* operator()();

private:
    void free();
    SDL_Texture* m_texture = NULL;

    int m_width = 0;
    int m_height = 0;
};
