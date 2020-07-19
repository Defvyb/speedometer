[![Build status](https://ci.appveyor.com/api/projects/status/xxcuo3ulvi5lk5qg?svg=true)](https://ci.appveyor.com/project/Defvyb/speedometer)

Speedometer

requires sdl2 library (apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev), cmake 3.8+ (apt install cmake)

build:
mkdir build_local
cd build_local
cmake ..
make

Usage:

generator
run with socket path, example:
./generator /tmp/socktmp.sock (mandatory parameter)

speedometer
-fps - show frames per seconds
-sock_name=/tmp/socktmp.sock - socket path(default: /tmp/socktmp.sock)