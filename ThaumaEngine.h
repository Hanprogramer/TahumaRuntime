#pragma once
#include <iostream>
using namespace std;
#include <string>
#include <SDL2/SDL.h>
#include "src/gl.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
namespace Thauma {
	const int ERROR_OK = 0;
	const int ERROR_FAILED = 1;

	struct Rect {
		int x = 0;
		int y = 0;
		int w = 1;
		int h = 1;
	};

	class Texture {
	public:
		Texture();
		~Texture();
		int load(string path);
		Rect src_rect{0,0,1,1};
		bool is_loaded = false;
		GLuint textureID = 0;
	private:
		SDL_Surface* texture;
		int mode = GL_RGB;
	};

}