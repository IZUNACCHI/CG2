#pragma once
#include "actor_class.hpp"
#include "loader_texture.hpp"

void PopulateVertices(std::vector<float>& vertices, float sprite_w, float sprite_h, float frame_w, float frame_h, float orderInLayer)
{
	const float pos_x{ (sprite_w) / 640 };
	const float pos_y{ (sprite_h) / 480 };

	vertices = {
		//vertex coords						//texture coords
		 pos_x,  pos_y, orderInLayer,		frame_w,	1.0f,			//top right
		 pos_x, -pos_y, orderInLayer,		frame_w,	1 - frame_h,	//bottom right
		-pos_x, -pos_y, orderInLayer,		0.0f,		1- frame_h,		//bottom left
		-pos_x,	 pos_y, orderInLayer,		0.0f,		1.0f			//top left
	};

	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	std::cout << vertices[i] << std::endl;
	//}
}

void Translate(glm::mat4& model, glm::vec3 transPos)
{
	model = glm::translate(model, transPos);
}

Actor::Actor()
	:m_id{}
{}

Actor::Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, bool banimate)
	:m_id{ objectCount }
{
	p_texture = &texture;
	m_textureWidth = tex_width;
	m_textureHeight = tex_height;
	m_width = width;
	m_height = height;
	m_orderInLayer = layer / 10;
	m_indices = { 0, 1, 3, 1, 2, 3 };
	animate = banimate;

	glGenVertexArrays(1, &m_vao);

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };


	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer);
	objectCount++;
	objects.reserve(objectCount);
}

Actor::Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, float x, float y, bool banimate)
	:m_id{ objectCount }
{
	p_texture = &texture;
	m_textureWidth = tex_width;
	m_textureHeight = tex_height;
	m_width = width;
	m_height = height;
	m_orderInLayer = layer / 10;
	m_indices = { 0, 1, 3, 1, 2, 3 };
	animate = banimate;

	glGenVertexArrays(1, &m_vao);

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

//Actor::Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Actor>& objects, bool animate)
//	:m_model(1.0f), p_texture{ &texture }, m_textureWidth{ tex_width }, m_textureHeight{ tex_height }, m_width{ width }, m_height{ height },
//	m_orderInLayer{ (layer / 10) }, m_indices{ 0, 1, 3, 1, 2, 3 }, animate{ animate }, m_vao{}
//{
//
//	p_texture = &texture;
//	m_textureWidth = tex_width;
//	m_textureHeight = tex_height;
//	m_width = width;
//	m_height = height;
//
//	glGenVertexArrays(1, &m_vao);
//
//	m_frameWidth = { m_width / m_textureWidth };
//	m_frameHeight = { m_height / m_textureHeight };
//
//
//	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer);
//	objectCount++;
//	objects.reserve(objectCount);
//}
//
//Actor::Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Actor>& objects, float x, float y, bool animate)
//	:m_model(1.0f), p_texture{ &texture }, m_textureWidth{ tex_width }, m_textureHeight{ tex_height }, m_width{ width }, m_height{ height },
//	m_orderInLayer{ (layer / 10) }, m_indices{ 0, 1, 3, 1, 2, 3 }, animate{ animate }, m_vao{}
//{
//
//	glGenVertexArrays(1, &m_vao);
//
//	m_frameWidth = { m_width / m_textureWidth };
//	m_frameHeight = { m_height / m_textureHeight };
//
//	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer);
//	objectCount++;
//	objects.reserve(objectCount);
//
//	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
//}

//glm::mat4 Actor::model()
//{
//	return m_model;
//}
//
//size_t Actor::vertexBufferSize()
//{
//	return m_vertices.size();
//}
//
//float* Actor::vertices()
//{
//	return m_vertices.data();
//}
//
//size_t Actor::indexBufferSize()
//{
//	return m_indices.size();
//}
//
//unsigned int* Actor::indices()
//{
//	return m_indices.data();
//}
//
//float Actor::layer()
//{
//	return m_orderInLayer;
//}
//
//GLuint Actor::texture()
//{
//	return *p_texture;
//}
//
//GLuint Actor::vao()
//{
//	return m_vao;
//}
//
//float Actor::frameWidth()
//{
//	return m_frameWidth;
//}
//
//float Actor::frameHeight()
//{
//	return m_frameHeight;
//}

void Actor::printVs() {
	for (int i = 0; i < m_vertices.size(); i++)
	{
		std::cout << m_vertices[i] << std::endl;
	}
}

unsigned int Actor::id()
{
	return m_id;
}

//unsigned int Actor::objectCount = 0;



