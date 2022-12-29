#pragma once

#ifndef __EMSCRIPTEN__
//This define makes my gpu to properly bind vbos to shaders. Changes vertexattribarray order.
//#define HD4850
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#else 
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <emscripten.h>
#endif

#include "GL/glew.h"

#ifdef __WIN32
#include <GL/GL.h>
#include <GL/GLU.h>
#else
#include "GL/glew.h"
#include <GL/glu.h>
// #include <GL/glext.h>
#endif

#ifdef HD4850
#define MODEL_MESHBUFFER 1
#define MODEL_TEXCORDBUFFER 2
#define MODEL_NORMALBUFFER 0
#else
#define MODEL_MESHBUFFER 0
#define MODEL_TEXCORDBUFFER 1
#define MODEL_NORMALBUFFER 2
#endif

#include <stdexcept>
#include <vector>
#include <array>
#include <thread>
#include "StdLibWrapper.h"