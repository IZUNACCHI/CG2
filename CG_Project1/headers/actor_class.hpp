#pragma once
#include "loader_wavefront.hpp"
#include "vboindexer.hpp"
#include "SDL.h"
#include "object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>

#include <iostream>

class Actor : public Object
{
public:
	Actor();
	Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, bool banimate);
	Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, float x, float y, bool banimate);
	Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, float x, float y, bool banimate, std::string name);
	Actor(GLuint& texture, int tex_width, int tex_height, float tileSize, float tilesX, float tilesY, float start_index, float layer, std::vector<Object>& objects, float x, float y, bool banimate);
	
	void printVs();
	
	unsigned int id();
	
	~Actor() {};

private:
	unsigned int m_id;
};