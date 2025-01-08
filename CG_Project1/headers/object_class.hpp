#pragma once
#include "loader_wavefront.hpp"
#include "vboindexer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>

#include <iostream>

class Object
{
public:
	Object(const char* texfile, float width, float height, std::vector<Object>& objects, unsigned int& objectCount);

	void PopulateVertices();
	void SaveTexSize(int texWidth, int texHeight);

	void translate(float pos_x, float pos_y);

	glm::mat4 model();
	size_t vertexBufferSize();
	float* vertices();
	size_t indexBufferSize();
	unsigned int* indices();

	const char* texture();

	float frameWidth();
	float frameHeight();
	int textureWidth();
	int textureHeight();
	
	~Object() {};

private:
	glm::mat4 m_model;
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices{ 0, 1, 3, 1, 2, 3 };
	const char* m_texture;
	float m_width;
	float m_height;
	int m_textureWidth;
	int m_textureHeight;
	float m_frameWidth;
	float m_frameHeight;
};