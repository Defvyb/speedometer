#pragma once
#include <SDL.h>
#include <string_view>
#include <atomic>
#include "gui/texture.h"

class SDLAppSpeedometer final {
public:
    SDLAppSpeedometer() = default;
    ~SDLAppSpeedometer();

    SDLAppSpeedometer(SDLAppSpeedometer &) = delete;
    SDLAppSpeedometer(SDLAppSpeedometer &&) = delete;
    SDLAppSpeedometer& operator=(SDLAppSpeedometer&) = delete;
    SDLAppSpeedometer& operator=(SDLAppSpeedometer&&) = delete;

    bool init(const std::string & screenName, int screenWidth = 500, int screenHeight = 500);
    void run(bool showFps, std::atomic<bool> & quit);
    void setSpeed(double speed);
    void setOdoKm(double odo);



private:
    bool loadMedia();
    double convertSpeedToAngle(double speed) const;
    bool drawFps(bool showFps, double elapsed);
    bool drawOdo();

    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    SDL_Color m_blackTextColor = {0, 0, 0};
    SDL_Color m_whiteTextColor = {255, 255, 255};
    TTF_Font* m_font = NULL;
    Texture m_textureSpeedometer;
    Texture m_textureArrow;
    Texture m_textureFps;
    Texture m_textureOdo;
    int m_screenWidth = 500;
    int m_screenHeight = 500;
    std::atomic<double> m_angle = 200;
    std::atomic<double> m_odoKm = 0;
    static const int FPS_SHOW_RATIO = 40;
};
