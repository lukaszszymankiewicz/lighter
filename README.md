```
 _     ___ ____ _   _ _____ _____ ____  
| |   |_ _/ ___| | | |_   _| ____|  _ \ 
| |    | | |  _| |_| | | | |  _| | |_) |
| |___ | | |_| |  _  | | | | |___|  _ < 
|_____|___\____|_| |_| |_| |_____|_| \_\
                                        

```
Lighter version 0.1.3
Game written in C using SDL2 library.
It will contain awsome light effects and post-apocaliptic vibe.

This repo contains only game code, development tools are put here: [repo](https://github.com/lukaszszymankiewicz/ltools)

Current work progress:

![A test image](screenshot.gif)

Controls:

A - move left

D - move right

W - swipe light upwards

S - swipe light downwards

Space - Jump

Q - cycle DEBUG functions

How to contirbute:

Linux (tested on Ubuntu 18):

1) Ensure that you`ve got proper libraries installed:

```
sudo apt-get install libsdl2-dev libsdl2-image-2.0.0 
```

1a) Ensure you`ve got all compilation toolchain (gcc, make):

```
sudo apt install build-essential
```

2) In very rare case of not having all core package install it by using command:

```
sudo apt install libc6-dev libasound-dev libpthread-stubs0-dev libpulse-dev xorg libdrm-dev libgbm-dev
```

3) To compile use command:

```
make lighter
```
