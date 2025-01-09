#pragma once
#include "loader_wavefront.hpp"
#include "vboindexer.hpp"
#include "SDL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>

#include <iostream>

class Object
{
public:
	Object(const char* texfile, float width, float height, std::vector<Object>& objects, SDL_Surface* windowSurface);
	Object(const char* texfile, float width, float height, std::vector<Object>& objects, float x, float y, SDL_Surface* windowSurface);

	void translate(float pos_x, float pos_y);

	glm::mat4 model();
	size_t vertexBufferSize();
	float* vertices();
	size_t indexBufferSize();
	unsigned int* indices();

	GLuint texture();
	GLuint vao();

	float frameWidth();
	float frameHeight();
	int textureWidth();
	int textureHeight();

	void printVs();

	float frameOffset_x = 0;
	float frameOffset_y = 0;
	glm::mat4 m_model;

	static unsigned int objectCount;
	
	~Object() {};

private:
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	float m_width;
	float m_height;

	int m_textureWidth;
	int m_textureHeight;
	float m_frameWidth;
	float m_frameHeight;
	
	GLuint m_texture;
	GLuint m_vao;
};