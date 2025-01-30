#pragma once
#include <vector>
#include <glad/glad.h>
#include "object.h"
#include <map>

class Text : public Object 
{
public:
	Text(std::string& letters, GLuint& texture, std::map<char, std::vector<float>> mapped_chars, bool smalltxt, std::vector<Object>& objects, float x, float y);
	Text(const char*, GLuint& texture, std::map<char, std::vector<float>> mapped_chars, bool smalltxt, std::vector<Object>& objects, float x, float y);
	~Text() {};

private:
};
