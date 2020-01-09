#pragma once

#ifndef __EMSCRIPTEN__
//This define makes my gpu to properly bind vbos to shaders. Changes vertexattribarray order.
#define HD4850
#endif