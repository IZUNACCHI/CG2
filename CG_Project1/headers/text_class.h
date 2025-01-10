#pragma once
#include <vector>
#include <glad/glad.h>
#include "object.h"
#include <map>

class Text : public Object 
{
public:
	Text(std::vector<char>& letters, GLuint& texture, std::map<char, std::vector<float>> mapped_chars, bool smalltxt, std::vector<Object>& objects);
	~Text() {};

private:
	std::vector<char> m_letters;
	//std::map<char, std::vector<float>> s
};
