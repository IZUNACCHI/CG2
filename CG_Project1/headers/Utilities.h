#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <map>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtx/norm.hpp>
#include <iostream>

#include "object.h"
#include "actor_class.hpp"

// Sorts the objects to be rendered by their order of layer
// ( 0 being the layer to be rendered first, and 10 (or above) last )
std::map<float, Object> SortObjects(std::vector<Object> objects)
{
	std::map<float, Object> sorted;

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		float layer = objects[i].layer() * 10;

		std::map<float, Object>::iterator it = sorted.find(layer);

		while (it != sorted.end())
		{
			layer += 0.01f;
			it = sorted.find(layer);
		}
		sorted[layer] = objects[i];
	}

	return sorted;
}

char characters[]
{
	' ', '!', '"', '#', '$', '%', '&', '@',
	'(', ')', '*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ':', ';', '<', '=', '>', '?',
	'€', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', '[', 'b/', ']', '^', '_',
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	'x', 'y', 'z', '{', '|', '}', '~', '«'
};

void MapChars(std::map<char, std::vector<float>>& map_small_chars, float s_char_size, float s_tex_w, float s_tex_h, std::map<char, std::vector<float>>& map_big_chars, float b_char_size, float b_tex_w, float b_tex_h)
{
	static unsigned int char_no = 0;
	float s_fWidth = s_char_size / s_tex_w;
	float s_fHeight = s_char_size / s_tex_h;
	float b_fWidth = b_char_size / b_tex_w;
	float b_fHeight = b_char_size / b_tex_h;

	//for each row of characters in the texture
	for (float i = 0; i < 12; i++)
	{
		//for each column of characters in the texture
		for (float j = 0; j < 8; j++)
		{
			map_small_chars[characters[char_no]] = {	s_fWidth * (j + 1),	1 - s_fHeight * i,
														s_fWidth * (j + 1),	1 - s_fHeight * (i + 1),
														s_fWidth * j	  ,	1 - s_fHeight * (i + 1),
														s_fWidth * j	  , 1 - s_fHeight * i			};

			map_big_chars[characters[char_no]] = {		b_fWidth * (j + 1),	1 - b_fHeight * i,
														b_fWidth * (j + 1),	1 - b_fHeight * (i + 1),
														b_fWidth * j	  ,	1 - b_fHeight * (i + 1),
														b_fWidth * j	  , 1 - b_fHeight * i			};
			char_no++;
		}
	}
}