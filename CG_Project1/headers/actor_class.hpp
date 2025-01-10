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
	
	//void translate(float pos_x, float pos_y);
	void printVs();

	//glm::mat4 model();
	//size_t vertexBufferSize();
	//float* vertices();
	//size_t indexBufferSize();
	//unsigned int* indices();
	//float layer();

	//GLuint texture();
	//GLuint vao();

	//float frameWidth();
	//float frameHeight();

	//static unsigned int objectCount;
	
	unsigned int id();

	//float frameOffset_x = 0;
	//float frameOffset_y = 0;
	//glm::mat4 m_model;
	//bool animate;
	
	~Actor() {};

private:
	//std::vector<float> m_vertices;
	//std::vector<unsigned int> m_indices;

	//float m_width;
	//float m_height;
	unsigned int m_id;

	//int m_textureWidth;
	//int m_textureHeight;
	//float m_frameWidth;
	//float m_frameHeight;
	//
	//GLuint* p_texture;
	//GLuint m_vao;

	//float m_orderInLayer;
};