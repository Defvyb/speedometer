#pragma once
#include <SDL.h>
#include <string_view>
#include <atomic>
#include "gui/texture.h"
class SDLAppSpeedometer final
{
public:
    SDLAppSpeedometer();
    ~SDLAppSpeedometer();

    bool init(const std::string & screenName, int screenWidth = 500, int screenHeight = 500);
    void run();
    void setSpeed(double speed);
private:
    bool loadMedia();
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    Texture m_textureSpeedometer;
    Texture m_textureArrow;
    Texture m_textureFps;
    int m_screenWidth;
    int m_screenHeight;
    std::atomic<double> m_angle;
    static const int FPS_SHOW_RATIO = 40;
};
