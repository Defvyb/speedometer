#pragma once

#include <string_view>
#include <iostream>

void dumpError(std::string_view message, auto error){
   std::cerr << message << " Error: "<< error << std::endl;
}
void log_warn(std::string_view message);
void log_error(std::string_view message);
