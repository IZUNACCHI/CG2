#include "loader_texture.hpp"

void FlipSurface(SDL_Surface* surface)
{
	SDL_LockSurface(surface);

	int pitch = surface->pitch;
	char* temp = new char[pitch];
	char* pixels = (char*)surface->pixels;

	for (int i = 0; i < surface->h / 2; i++)
	{
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (surface->h - i - 1) * pitch;

		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}

	delete[] temp;
	SDL_UnlockSurface(surface);
}

// meter isto numa file própria e usar nas definiçoes do tex_width e tex_height e nas texcoords
// OU
// só cagar e apagar (funciona na msm, i think its used for performance stuff? idk)
static int PowerOfTwo(int input)
{
	int value = 1;
	while (value < input)
	{
		value <<= 1;
	}
	return value;
}

void LoadTexture(const char* fileName, GLuint& texture, int& tex_width, int& tex_height, SDL_Surface* windowSurface)
{
	SDL_Surface* image = SDL_LoadBMP(fileName);
	SDL_Surface* opt_image = nullptr;
	SDL_Surface* alpha_image = nullptr;
	SDL_Rect area;
	Uint32 colorkey;

	FlipSurface(image);
	if (image == NULL)
	{
		std::cout << "Error: NULL Surface" << std::endl;
	}
	else
	{
		opt_image = SDL_ConvertSurface(image, windowSurface->format, 0);

		tex_width = opt_image->w;
		tex_height = opt_image->h;

		alpha_image = SDL_CreateRGBSurface(0, opt_image->w, opt_image->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

		colorkey = SDL_MapRGBA(alpha_image->format, 255, 0, 255, 0);
		SDL_FillRect(alpha_image, NULL, colorkey);

		colorkey = SDL_MapRGBA(opt_image->format, 255, 0, 255, 0);
		SDL_SetColorKey(opt_image, SDL_TRUE, colorkey);

		area.x = 0;
		area.y = 0;
		area.w = opt_image->w;
		area.h = opt_image->h;

		SDL_BlitSurface(opt_image, &area, alpha_image, &area);

		if (alpha_image == NULL)
		{
			std::cout << "Error: NULL optimizedSurface" << std::endl;
		}
		else
		{
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, alpha_image->w, alpha_image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha_image->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	SDL_FreeSurface(image);
	SDL_FreeSurface(alpha_image);

	//stbi_set_flip_vertically_on_load(true);

	//int nrChannels;
	//unsigned char* data = stbi_load(fileName, &tex_width, &tex_height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);
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
