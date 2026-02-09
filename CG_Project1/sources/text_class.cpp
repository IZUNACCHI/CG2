#include "text_class.h"

void PopulateVertices(std::vector<float>& vertices, std::vector<char>& letters, std::map<char, std::vector<float>> mapped_chars, float sprite_size, float layer)
{
	float pos_x{ sprite_size / 640 };
	float pos_y{ sprite_size / 480 };
	vertices.reserve(20 * letters.size());
	std::vector<float> texCoords(8);

	for (unsigned int i = 0; i < letters.size(); i++)
	{
		std::map<char, std::vector<float>>::iterator it = mapped_chars.find(letters[i]);
		if (it != mapped_chars.end())
		{
			texCoords = it->second;
		}
		vertices.push_back(pos_x * (i + 1));
		vertices.push_back(pos_y);
		vertices.push_back(layer);
		vertices.push_back(texCoords[0]);
		vertices.push_back(texCoords[1]);

		vertices.push_back(pos_x * (i + 1));
		vertices.push_back(0);
		vertices.push_back(layer);
		vertices.push_back(texCoords[2]);
		vertices.push_back(texCoords[3]);

		vertices.push_back(pos_x * i);
		vertices.push_back(0);
		vertices.push_back(layer);
		vertices.push_back(texCoords[4]);
		vertices.push_back(texCoords[5]);

		vertices.push_back(pos_x * i);
		vertices.push_back(pos_y);
		vertices.push_back(layer);
		vertices.push_back(texCoords[6]);
		vertices.push_back(texCoords[7]);
	}
}

void PopulateIndices(std::vector<unsigned int>& indices, std::vector<char>& letters)
{
	indices.reserve(6 * letters.size());
	for (unsigned int i = 0; i < letters.size(); i++)
	{
		indices.push_back(i * 4);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
	}
}

Text::Text(std::string& sentence, GLuint& texture, std::map<char, std::vector<float>> mapped_chars, bool smalltxt, std::vector<Object>& objects, float x, float y)
{
	glGenVertexArrays(1, &m_vao);

	std::vector<char> letters(sentence.begin(), sentence.end());

	p_texture = &texture;
	m_orderInLayer = 4.0f / 10.0f;
	if (smalltxt)
	{
		m_textureWidth = 64;
		m_textureHeight = 128;
		m_width = m_height = 16;
	}
	else
	{
		m_textureWidth = 128;
		m_textureHeight = 192;
		m_width = m_height = 32;
	}

	m_frameWidth = m_width / m_textureWidth;
	m_frameHeight = m_height / m_textureHeight;

	PopulateVertices(m_vertices, letters, mapped_chars, m_width, m_orderInLayer);
	/*for (unsigned int i = 0; i < m_vertices.size(); i++)
	{
		std::cout << m_vertices[i] << std::endl;
	}*/
	PopulateIndices(m_indices, letters);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}

Text::Text(const char* sentence, GLuint& texture, std::map<char, std::vector<float>> mapped_chars, bool smalltxt, std::vector<Object>& objects, float x, float y)
{
	glGenVertexArrays(1, &m_vao);

	std::string s{ sentence };
	std::vector<char> letters(s.begin(), s.end());

	p_texture = &texture;
	m_orderInLayer = 4.0f / 10.0f;
	if (smalltxt)
	{
		m_textureWidth = 64;
		m_textureHeight = 128;
		m_width = m_height = 16;
	}
	else
	{
		m_textureWidth = 128;
		m_textureHeight = 192;
		m_width = m_height = 32;
	}

	m_frameWidth = m_width / m_textureWidth;
	m_frameHeight = m_height / m_textureHeight;

	PopulateVertices(m_vertices, letters, mapped_chars, m_width, m_orderInLayer);
	/*for (unsigned int i = 0; i < m_vertices.size(); i++)
	{
		std::cout << m_vertices[i] << std::endl;
	}*/
	PopulateIndices(m_indices, letters);
	objectCount++;
	objects.reserve(objectCount);

	m_model = glm::translate(m_model, glm::vec3(x, y, 0.0f));
}
