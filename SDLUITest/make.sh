#!/bin/bash
FILES="$(ls *.cpp)"
em++ $FILES  -s WASM=1 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["bmp","png"]' -s FORCE_FILESYSTEM=1 -s USE_SDL_TTF=2 --preload-file Assets -std=c++11 -o ./fin/SDLUITest.html

