#pragma once
#include <string>

struct ProgramOptions final{
    ProgramOptions() = default;
    ~ProgramOptions() = default;

    ProgramOptions(ProgramOptions &) = default;
    ProgramOptions(ProgramOptions &&) = default;
    ProgramOptions& operator=(ProgramOptions&) = default;
    ProgramOptions& operator=(ProgramOptions&&) = default;

    bool showFps = false;
    std::string socketPath = "/tmp/socktmp.sock";
};
