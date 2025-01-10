#include "stb_image.h"
#include "SDL.h"
#include "object.h"

#include <iostream>
#include <glad/glad.h>
#include <vector>

//void LoadTexture(const char* fileName, GLuint& texture, int& tex_width, int& tex_height, SDL_Surface* windowSurface);

void LoadTexture(GLuint& texture, const char* fileName, SDL_Surface* windowSurface);