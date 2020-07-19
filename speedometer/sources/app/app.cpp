#include "app.h"
#include <SDL_image.h>
#include <iostream>
#include <commonLib/helpers.h>


SDLAppSpeedometer::~SDLAppSpeedometer(){
    //Destroy window
    TTF_CloseFont(m_font);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    m_font = NULL;
    m_renderer = NULL;
    m_window = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

double SDLAppSpeedometer::convertSpeedToAngle(double speed) const{
    if(speed > 320.0) speed = 320.0;
    if(speed < 0.0) speed = 0.0;
    return speed + 200.0;
}

void SDLAppSpeedometer::setSpeed(double speed){
    m_angle = convertSpeedToAngle(speed);
}

bool SDLAppSpeedometer::init(const std::string & screenName, int screenWidth, int screenHeight){
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        helpers::dumpError("SDL could not initialize!", SDL_GetError());
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
        helpers::log_warn("Linear texture filtering not enabled!");
    }

    //Create window
    m_window = SDL_CreateWindow(screenName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
    if(NULL == m_window){
        helpers::dumpError("Window could not be created!", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if(m_renderer == NULL){
        helpers::dumpError("Renderer could not be created!", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        helpers::dumpError("SDL_image could not initialize!", IMG_GetError());
        return false;
    }

    if(TTF_Init() == -1){
        helpers::dumpError("SDL_ttf could not initialize!", TTF_GetError());
        return false;
    }

    return true;
}

bool SDLAppSpeedometer::loadMedia()
{

    m_font = TTF_OpenFont("sdf.ttf", 20);
    if(m_font == NULL){
        helpers::dumpError("Failed to load lazy font!", TTF_GetError());
        return false;
    }

    if(!m_textureSpeedometer.loadFromFile(m_renderer, "speedometer.jpg")){
        helpers::log_error("Failed to load speedometer texture!");
        return false;
    }

    if(!m_textureArrow.loadFromFile(m_renderer, "arrow.png")){
        helpers::log_error("Failed to load arrow texture!");
        return false;
    }

    if(!m_textureFps.loadFromRenderedText(m_font, m_renderer, "0", m_textColor)){
        helpers::log_error("Failed to load arrow texture!");
        return false;
    }

    return true;
}

bool SDLAppSpeedometer::drawFps(bool showFps, double elapsed){
    static double fpsToShow = 0.0;
    static int fpsRatioCounter = 0;
    if(showFps){
        if(fpsRatioCounter++ >= FPS_SHOW_RATIO){
            if(elapsed != 0.0){
                fpsToShow = 1.0 / elapsed;
            }
            fpsRatioCounter=0;
        }

        if(!m_textureFps.loadFromRenderedText(m_font,
                                              m_renderer,
                                              std::to_string(static_cast<int>(floor(fpsToShow))),
                                              m_textColor)){
            helpers::log_error("Failed to load arrow texture!");
            return false;
        }

        m_textureFps.render(0, 0,
                            m_textureFps.getWidth(),
                            m_textureFps.getHeight(),
                            m_renderer, 0);
    }
    return true;
}

void SDLAppSpeedometer::run(bool showFps, std::atomic<bool> & quit){
    if(!loadMedia()){
        helpers::log_error("Failed to load media!");
        return;
    }

    SDL_Event e;
    double startclock = 0.0;
    double endclock = 0.0;
    double elapsed = 0.0;
    while(!quit){
        if(showFps) startclock = static_cast<double>(SDL_GetPerformanceCounter());
        while(SDL_PollEvent(&e) != 0){
            //User requests quit
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }

        SDL_RenderClear(m_renderer);

        m_textureSpeedometer.render(0, 0,
                                    m_screenWidth,
                                    m_screenHeight,
                                    m_renderer, 0);
        m_textureArrow.render((m_screenWidth - m_textureArrow.getWidth())/2,
                              (m_screenHeight - m_textureArrow.getHeight())/2,
                              m_textureArrow.getWidth(),
                              m_textureArrow.getHeight(),
                              m_renderer, m_angle);

        if(!drawFps(showFps, elapsed)) return;

        SDL_RenderPresent(m_renderer);

        if(showFps){
            endclock = static_cast<double>(SDL_GetPerformanceCounter());
            elapsed = (endclock - startclock) / static_cast<double>(SDL_GetPerformanceFrequency());
        }
    }
}
