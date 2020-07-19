Arrival test task

requires sdl2 library (apt install libsdl2-dev), cmake 3.8+ (apt install cmake)

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