#include "stb_image.h"
#include "SDL.h"
#include "object_class.hpp"

#include <iostream>
#include <glad/glad.h>
#include <vector>

void LoadTexture(const char* fileName, GLuint& texture, int& tex_width, int& tex_height);
void LoadTexture(const char* fileName, int& tex_width, int& tex_height);

//SDL_Texture* LoadTexture(std::string fileName, SDL_Renderer* renderTarget, std::vector<GLuint>& textures, unsigned int& textureCount);
