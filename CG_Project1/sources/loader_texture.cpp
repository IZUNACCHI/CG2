#include "loader_texture.hpp"

static int PowerOfTwo(int input)
{
	int value = 1;
	while (value < input)
	{
		value <<= 1;
	}
	return value;
}

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

void LoadTexture(GLuint& texture, const char* fileName, SDL_Surface* windowSurface)
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, alpha_image->w, alpha_image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha_image->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	SDL_FreeSurface(image);
	SDL_FreeSurface(alpha_image);
};

//void LoadTexture(const char* fileName, GLuint& texture, int& tex_width, int& tex_height, SDL_Surface* windowSurface)
//{
//	SDL_Surface* image = SDL_LoadBMP(fileName);
//	SDL_Surface* opt_image = nullptr;
//	SDL_Surface* alpha_image = nullptr;
//
//	SDL_Rect area;
//	Uint32 colorkey;
//
//	FlipSurface(image);
//	if (image == NULL)
//	{
//		std::cout << "Error: NULL Surface" << std::endl;
//	}
//	else
//	{
//		opt_image = SDL_ConvertSurface(image, windowSurface->format, 0);
//
//		tex_width = opt_image->w;
//		tex_height = opt_image->h;
//
//		//tex_width = PowerOfTwo(opt_image->w);
//		//tex_height = PowerOfTwo(opt_image->h);
//
//		alpha_image = SDL_CreateRGBSurface(0, opt_image->w, opt_image->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
//
//		colorkey = SDL_MapRGBA(alpha_image->format, 255, 0, 255, 0);
//		SDL_FillRect(alpha_image, NULL, colorkey);
//
//		colorkey = SDL_MapRGBA(opt_image->format, 255, 0, 255, 0);
//		SDL_SetColorKey(opt_image, SDL_TRUE, colorkey);
//
//		area.x = 0;
//		area.y = 0;
//		area.w = opt_image->w;
//		area.h = opt_image->h;
//
//		SDL_BlitSurface(opt_image, &area, alpha_image, &area);
//
//		if (alpha_image == NULL)
//		{
//			std::cout << "Error: NULL optimizedSurface" << std::endl;
//		}
//		else
//		{
//			glGenTextures(1, &texture);
//			glBindTexture(GL_TEXTURE_2D, texture);
//
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, alpha_image->w, alpha_image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha_image->pixels);
//			glGenerateMipmap(GL_TEXTURE_2D);
//		}
//	}
//
//	SDL_FreeSurface(image);
//	SDL_FreeSurface(alpha_image);
//};
