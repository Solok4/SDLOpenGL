#pragma once

#ifndef __EMSCRIPTEN__
//This define makes my gpu to properly bind vbos to shaders. Changes vertexattribarray order.
#define HD4850
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