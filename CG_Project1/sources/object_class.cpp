#pragma once
#include "object_class.hpp"
#include "loader_texture.hpp"

void PopulateVertices(std::vector<float>& vertices, float sprite_w, float sprite_h, float frame_w, float frame_h)
{
	const float pos_x{ (sprite_w) / 640 };
	const float pos_y{ (sprite_h) / 480 };

	vertices = {
		//vertex coords		//texture coords
		 pos_x,  pos_y,		frame_w,	1.0f,			//top right
		 pos_x, -pos_y,		frame_w,	1 - frame_h,	//bottom right
		-pos_x, -pos_y,		0.0f,		1- frame_h,		//bottom left
		-pos_x,	 pos_y,		0.0f,		1.0f			//top left
	};
}

void Translate(glm::mat4& model, glm::vec3 transPos)
{
	glm::translate(model, transPos);
}

Object::Object(const char* texfile, float width, float height, std::vector<Object>& objects, SDL_Surface* windowSurface)
	:m_model{ glm::mat4(1.0f) }, m_width{ width }, m_height{ height }, m_indices{ 0, 1, 3, 1, 2, 3 }, m_texture{}, m_vao{}
{
	LoadTexture(texfile, m_texture, m_textureWidth, m_textureHeight, windowSurface);

	glGenVertexArrays(1, &m_vao);

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight);
	objectCount++;
	objects.reserve(objectCount);
}

Object::Object(const char* texfile, float width, float height, std::vector<Object>& objects, float x, float y, SDL_Surface* windowSurface)
	:m_model{ glm::mat4(1.0f) }, m_width{ width }, m_height{ height }, m_indices{ 0, 1, 3, 1, 2, 3 }, m_texture{}, m_vao{}
{
	LoadTexture(texfile, m_texture, m_textureWidth, m_textureHeight, windowSurface);

	glGenVertexArrays(1, &m_vao);

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

void Object::translate(float pos_x, float pos_y)
{
	Translate(m_model, glm::vec3(pos_x, pos_y, 0.0f));
}

glm::mat4 Object::model()
{
	return m_model;
}

size_t Object::vertexBufferSize()
{
	return m_vertices.size();
}

float* Object::vertices()
{
	return m_vertices.data();
}

size_t Object::indexBufferSize()
{
	return m_indices.size();
}

unsigned int* Object::indices()
{
	return m_indices.data();
}

GLuint Object::texture()
{
	return m_texture;
}

GLuint Object::vao()
{
	return m_vao;
}

float Object::frameWidth()
{
	return m_frameWidth;
}

float Object::frameHeight()
{
	return m_frameHeight;
}

int Object::textureWidth()
{
	return m_textureWidth;
}

int Object::textureHeight()
{
	return m_textureHeight;
}

void Object::printVs() {
	for (int i = 0; i < m_vertices.size(); i++)
	{
		std::cout << m_vertices[i] << std::endl;
	}
}

unsigned int Object::objectCount = 0;

