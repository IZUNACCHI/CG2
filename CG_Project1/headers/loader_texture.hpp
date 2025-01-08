#include "stb_image.h"
#include "SDL.h"
#include "object_class.hpp"

#include <iostream>
#include <glad/glad.h>
#include <vector>

void LoadTexture(const char* fileName, std::vector<GLuint>& textures, unsigned int& textureCount, Object obj);

//SDL_Texture* LoadTexture(std::string fileName, SDL_Renderer* renderTarget, std::vector<GLuint>& textures, unsigned int& textureCount);
