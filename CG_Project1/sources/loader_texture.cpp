#include "loader_texture.hpp"

void LoadTexture(const char* fileName, std::vector<GLuint>& textures, unsigned int& textureCount, Object obj)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	obj.SaveTexSize(width, height);
	textureCount++;
	textures.reserve(textureCount);
};

//SDL_Texture* LoadTexture(std::string fileName, SDL_Renderer* renderTarget, std::vector<GLuint>& textures, unsigned int& textureCount)
//{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	SDL_Texture* texture = nullptr;
//	SDL_Surface* surface = SDL_LoadBMP(fileName.c_str());
//
//	if (surface == NULL)
//		std::cout << "Error: NULL Surface" << std::endl;
//	else
//	{
//		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
//		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
//		if (texture == NULL)
//			std::cout << "Error: NULL Texture" << std::endl;
//	}
//	SDL_FreeSurface(surface);
//	textureCount++;
//	textures.reserve(textureCount);
//};
