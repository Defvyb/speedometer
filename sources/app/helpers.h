#pragma once

#include <string_view>
#include <iostream>

template<typename T>
void dumpError(std::string_view message, T error)
{
   std::cerr << message << " Error: "<< error << std::endl;
}
void log_warn(std::string_view message);
void log_error(std::string_view message);
