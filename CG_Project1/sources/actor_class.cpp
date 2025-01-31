#pragma once
#include "actor_class.hpp"
#include "loader_texture.hpp"

//void PopulateVertices(std::vector<float>& vertices, float sprite_w, float sprite_h, float frame_w, float frame_h, float orderInLayer, unsigned int start_index)
//{
//	const float pos_x{ (sprite_w) / 640 };
//	const float pos_y{ (sprite_h) / 480 };
//
//	vertices = {
//		//vertex coords						//texture coords
//		 pos_x,  pos_y, orderInLayer,		frame_w,	1.0f,			//top right
//		 pos_x, -pos_y, orderInLayer,		frame_w,	1 - frame_h,	//bottom ri(ght
//		-pos_x, -pos_y, orderInLayer,		0.0f,		1- frame_h,		//bottom left
//		-pos_x,  pos_y, orderInLayer,		0.0f,		1.0f			//top left
//	};
//}

void PopulateVertices(std::vector<float>& vertices, float sprite_w, float sprite_h, float frame_w, float frame_h, float orderInLayer, unsigned int offsetX, unsigned int offsetY, unsigned int tilesX, unsigned int tilesY)
{
	const float pos_x{ (sprite_w) / 640 };
	const float pos_y{ (sprite_h) / 480 };

	vertices = {
		//vertex coords						//texture coords
		 pos_x * tilesX,  pos_y * tilesY, orderInLayer,		frame_w * (offsetX + tilesX),	1.0f - (frame_h * offsetY),			//top right
		 pos_x * tilesX, -pos_y * tilesY, orderInLayer,		frame_w * (offsetX + tilesX),	1.0f - (frame_h * (offsetY + tilesY)),	//bottom ri(ght
		-pos_x * tilesX, -pos_y * tilesY, orderInLayer,		frame_w * offsetX,		1.0f - (frame_h * (offsetY + tilesY)),		//bottom left
		-pos_x * tilesX,  pos_y * tilesY, orderInLayer,		frame_w * offsetX,		1.0f - (frame_h * offsetY)			//top left
	};
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


	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer, 0, 0, 1, 1);
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

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer, 0, 0, 1, 1);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

Actor::Actor(GLuint& texture, int tex_width, int tex_height, float width, float height, float layer, std::vector<Object>& objects, float x, float y, bool banimate, std::string name) 
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
	m_name = name;

	glGenVertexArrays(1, &m_vao);

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer, 0, 0, 1, 1);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

Actor::Actor(GLuint& texture, int tex_width, int tex_height, float tileSize, float tilesX, float tilesY, float start_index, float layer, std::vector<Object>& objects, float x, float y, bool banimate, std::string name)
	:m_id{ objectCount }
{
	p_texture = &texture;
	m_textureWidth = tex_width;
	m_textureHeight = tex_height;
	m_width = tileSize;
	m_height = tileSize;
	m_orderInLayer = layer / 10;
	m_indices = { 0, 1, 3, 1, 2, 3 };
	animate = banimate;
	m_name = name;

	glGenVertexArrays(1, &m_vao);

	unsigned int offset_x = static_cast<unsigned int>(remainderf(start_index, (tex_width/ tileSize)));
	unsigned int offset_y = static_cast<unsigned int>(start_index / (tex_width/ tileSize));

	m_frameWidth = { m_width / m_textureWidth };
	m_frameHeight = { m_height / m_textureHeight };

	PopulateVertices(m_vertices, m_width, m_height, m_frameWidth, m_frameHeight, m_orderInLayer, offset_x, offset_y, tilesX, tilesY);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

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

