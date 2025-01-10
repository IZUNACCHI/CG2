#include "object.h"

Object::Object()
	:m_model{(1.0f)}, p_texture{}, m_textureWidth{}, m_textureHeight{}, m_width{}, m_height{},
	m_orderInLayer{}, m_indices{}, animate{ false }, m_vao{}, m_frameHeight{}, m_frameWidth{}
{}

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

float Object::layer()
{
	return m_orderInLayer;
}

GLuint Object::texture()
{
	return *p_texture;
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

unsigned int Object::objectCount = 0;
