#pragma once
#include <string>

struct ProgramOptions final
{
    ProgramOptions():
        showFps(false),
        socketAddr("/tmp/socktmp.sock") {}

    bool showFps;
    std::string socketAddr;
};
