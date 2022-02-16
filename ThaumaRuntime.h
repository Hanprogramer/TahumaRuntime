#pragma once
#include <iostream>
#include "src/gl.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "src/Game.h"
#include "src/common/loadshader.h"

#ifdef _WIN32
#include <WinBase.h>
#include <WinUser.h>
#include <fileapi.h>
#include "resource.h"
#include "cli.h"
#endif
using namespace std;

#ifdef _WIN32
#pragma comment(lib, "lua542/liblua54.a")
#endif
Uint64 _clock_NOW = SDL_GetPerformanceCounter();
Uint64 _clock_LAST = 0;
double _clock_deltaTime = 0;