#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Object
{
public:
	Object();
	glm::mat4 model();	
	size_t vertexBufferSize();
	float* vertices();
	size_t indexBufferSize();
	unsigned int* indices();
	float layer();

	GLuint texture();
	GLuint vao();

	float frameWidth();
	float frameHeight();

	static unsigned int objectCount;


	float frameOffset_x = 0;
	float frameOffset_y = 0;
	glm::mat4 m_model;
	bool animate;

protected:
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	float m_width;
	float m_height;

	int m_textureWidth;
	int m_textureHeight;
	float m_frameWidth;
	float m_frameHeight;

	GLuint* p_texture;
	GLuint m_vao;

	float m_orderInLayer;
};
