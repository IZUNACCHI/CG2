#include "object.h"

Object::Object()
	:m_model{(1.0f)}, p_texture{}, m_textureWidth{}, m_textureHeight{}, m_width{}, m_height{},
	m_orderInLayer{}, m_indices{}, animate{ false }, m_vao{}, m_frameHeight{}, m_frameWidth{}, currentAnimation{}, m_animations{}
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

Animation& Object::getAnimationByName(std::string name) {
	for (int i = 0; i < m_animations.size(); i++)
	{
		if (m_animations[i].m_name == name) {
			return m_animations[i];
		}
	}
}

void Object::setAnimation(std::string name) {
	auto anim = getAnimationByName(name);
	currentAnimation = anim.m_name;
}

void Object::resetAnimation() {
 	getAnimationByName(currentAnimation).m_currentFrame = 0;

}

unsigned int Object::objectCount = 0;
