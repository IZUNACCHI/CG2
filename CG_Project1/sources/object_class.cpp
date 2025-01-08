#pragma once
#include "object_class.hpp"

Object::Object(const char* texfile, float width, float height, std::vector<Object>& objects, unsigned int& objectCount)
	: m_model{ glm::mat4(1.0f) }, m_texture{ texfile }, m_width{ width }, m_height{ height }, m_textureWidth{}, m_textureHeight{}, m_frameWidth{}, m_frameHeight{}
{
	objectCount++;
	objects.reserve(objectCount);
}
void Object::translate(float pos_x, float pos_y)
{
	m_model = glm::translate(m_model, glm::vec3(pos_x, pos_y, 0.0));
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

const char* Object::texture()
{
	return m_texture;
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

void Object::PopulateVertices()
{
	const float pos_x{ m_width / 2 };
	const float pos_y{ m_height / 2 };
	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	m_vertices = {
		//vertex coords		//texture coords
		 pos_x,  pos_y,		m_frameWidth,	1.0f,				//top right
		 pos_x, -pos_y,		m_frameWidth,	m_frameHeight,		//bottom right
		-pos_x, -pos_y,		0.0f,			m_frameHeight,		//bottom left
		-pos_x,	 pos_y,		0.0f,			1.0f				//top left
	};
}
void Object::SaveTexSize(int texWidth, int texHeight)
{
	m_textureWidth = texWidth;
	m_textureHeight = texHeight;
}

