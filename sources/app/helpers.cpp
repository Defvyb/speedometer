#include <app/helpers.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

void log_warn(std::string_view message)
{
    std::cout << "Warning!:" << message << std::endl;
}

void log_error(std::string_view message)
{
    std::cout << "Error!:" << message << std::endl;
}

