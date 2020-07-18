#include "app.h"
#include <SDL_image.h>
#include <iostream>
#include "helpers.h"
SDLAppSpeedometer::SDLAppSpeedometer():
    m_window(NULL),
    m_renderer(NULL),
    m_font(NULL),
    m_screenWidth(500),
    m_screenHeight(500),
    m_angle(200)
{

}

SDLAppSpeedometer::~SDLAppSpeedometer()
{
    //Destroy window
    SDL_DestroyRenderer( m_renderer );
    SDL_DestroyWindow( m_window );
    TTF_CloseFont( m_font );
    m_window = NULL;
    m_renderer = NULL;
    m_font = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDLAppSpeedometer::setSpeed(double speed)
{
    if(speed > 320.0) speed = 320.0;
    if(speed < 0.0) speed = 0.0;
    m_angle = speed + 200.0;
}


bool SDLAppSpeedometer::init(const std::string & screenName, int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        dumpError("SDL could not initialize!", SDL_GetError());
        return false;
    }

    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        log_warn("Linear texture filtering not enabled!");
    }

    //Create window
    m_window = SDL_CreateWindow( screenName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN );
    if( NULL == m_window )
    {
        dumpError("Window could not be created!", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED );
    if( m_renderer == NULL )
    {
        dumpError("Renderer could not be created!", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor( m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        dumpError( "SDL_image could not initialize!", IMG_GetError());
        return false;
    }

    if( TTF_Init() == -1 )
    {
        dumpError( "SDL_ttf could not initialize!", TTF_GetError() );
        return false;
    }

    return true;
}

bool SDLAppSpeedometer::loadMedia()
{

    m_font = TTF_OpenFont( "sdf.ttf", 20);
    if( m_font == NULL )
    {
        dumpError( "Failed to load lazy font!", TTF_GetError() );
        return false;
    }

    if( !m_textureSpeedometer.loadFromFile(m_renderer, "speedometer.jpg" ) )
    {
        log_error("Failed to load speedometer texture!");
        return false;
    }

    if( !m_textureArrow.loadFromFile(m_renderer, "arrow.png" ) )
    {
        log_error("Failed to load arrow texture!");
        return false;
    }

    SDL_Color textColor = { 0, 0, 0 };
    if( !m_textureFps.loadFromRenderedText(m_font, m_renderer, "0", textColor ) )
    {
        log_error("Failed to load arrow texture!");
        return false;
    }

    return true;
}

void SDLAppSpeedometer::run()
{
    //Start up SDL and create window

    //Load media
    if( !loadMedia() )
    {
        log_error( "Failed to load media!" );
        return;
    }
    bool quit = false;

    SDL_Event e;

    SDL_Color textColor = { 0, 0, 0 };
    double startclock = 0.0;
    double endclock = 0.0;
    double elapsed = 0.0;
    double showFPS = 0.0;
    int fpsRatioCounter=0;
    while(!quit)
    {
        startclock = static_cast<double>(SDL_GetPerformanceCounter());
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        //Clear screen
        SDL_RenderClear( m_renderer );

        //Render texture to screen
        m_textureSpeedometer.render(0, 0,
                                    m_screenWidth,
                                    m_screenHeight,
                                    m_renderer, NULL, 0, NULL, SDL_FLIP_NONE);
        m_textureArrow.render( (m_screenWidth - m_textureArrow.getWidth())/2,
                               (m_screenHeight - m_textureArrow.getHeight())/2,
                               m_textureArrow.getWidth(),
                               m_textureArrow.getHeight(),
                               m_renderer,
                               NULL, m_angle, NULL, SDL_FLIP_NONE );

        if(fpsRatioCounter++ >= FPS_SHOW_RATIO)
        {
            if(elapsed)
            {
               showFPS = 1.0 / elapsed;
            }

            fpsRatioCounter=0;
        }
        if( !m_textureFps.loadFromRenderedText(m_font, m_renderer, std::to_string(static_cast<int>(floor(showFPS))), textColor ) )
        {
            log_error("Failed to load arrow texture!");
            return;
        }

        m_textureFps.render(0, 0,
                            m_textureFps.getWidth(),
                            m_textureFps.getHeight(),
                            m_renderer, NULL, 0, NULL, SDL_FLIP_NONE);



        //Update screen
        SDL_RenderPresent( m_renderer );

        endclock = static_cast<double>(SDL_GetPerformanceCounter());
        elapsed = (endclock - startclock) / static_cast<double>(SDL_GetPerformanceFrequency());

    }

    return;
}
