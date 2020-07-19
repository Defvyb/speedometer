[![Build status](https://ci.appveyor.com/api/projects/status/xxcuo3ulvi5lk5qg?svg=true)](https://ci.appveyor.com/project/Defvyb/speedometer)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=arrival_speedometer&metric=alert_status)](https://sonarcloud.io/dashboard?id=arrival_speedometer)

Speedometer project

requires sdl2 library 
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```
cmake 3.8+ 
```
sudo apt install cmake
```
gcc with C++17 support

build:
```
mkdir build_local
cd build_local
cmake ..
make
```

Usage:

generator, run with socket path mandatory parameter, example:
```
./generator /tmp/socktmp.sock
```

speedometer, args:
```
-fps - show frames per seconds
-sock_name=/tmp/socktmp.sock - socket path(default: /tmp/socktmp.sock)
```
example:
```
./speedometer -fps -sock_name=/tmp/socktmp.sock
```

how to test speedometer accuracy:
1) launch speedometer without args
2) run nc -U /tmp/socktmp.sock
3) enter speed value between 0-320

![Alt text](https://github.com/Defvyb/speedometer/blob/master/example.png)
