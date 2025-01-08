#include <SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "vboindexer.hpp"
#include "loader_wavefront.hpp"
#include "loader_texture.hpp"
#include "object_class.hpp"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	float screenWidth = 640;
	float screenHeight = 480;


	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	Shader shaderProgram("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");

	std::vector<Object> objects;
	unsigned int objectCount = 0;

	std::vector<GLuint> textures;
	unsigned int textureCount = 0;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// The only thing the user must do is initialize an "Object" and push it back into the vector of objects.

	std::vector<GLuint> VAOs;
	VAOs.reserve(objectCount);

	size_t vbo_size = 4 * objectCount;
	size_t ibo_size = 6 * objectCount;

	for (unsigned int i = 0; i < objectCount; i++)
	{
		GLuint vao;
		VAOs.push_back(vao);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		stbi_set_flip_vertically_on_load(true);
		LoadTexture(objects[i].texture(), textures, textureCount, objects[i]);
		objects[i].PopulateVertices();
		textures.push_back(texture);
	}

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(float), 0, GL_STATIC_DRAW);

	size_t vertexbufferoffset = 0;
	for (unsigned int i = 0; i < objectCount; i++)
	{
		std::cout << i << std::endl;
		glBufferSubData(GL_ARRAY_BUFFER, vertexbufferoffset * sizeof(float),	// offset
											4 * sizeof(float),					// size
												objects[i].vertices());			// data
		vertexbufferoffset += 4;
	}

	GLuint ibo;
	glGenBuffers(1, &ibo); // Generate 1 element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size * sizeof(unsigned int), 0, GL_STATIC_DRAW);

	size_t indexbufferoffset = 0;
	for (unsigned int i = 0; i < objectCount; i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexbufferoffset * sizeof(unsigned int),					// offset
													6 * sizeof(unsigned int),								// size
														objects[i].indices());								// data
		indexbufferoffset += 6;
	}

	vertexbufferoffset = 0;
	indexbufferoffset = 0;
	for (unsigned int i = 0; i < objectCount; i++)
	{
		glGenVertexArrays(1, &VAOs[i]);
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		shaderProgram.setVertexAttribPointer("position", 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vertexbufferoffset * sizeof(float)));
		shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)((vertexbufferoffset * sizeof(float)) + (3 * sizeof(float))));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		vertexbufferoffset += 4;
	}

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 0.1f, 100.0f);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shaderProgram.setInt("Texture", 0);

	SDL_Event event;

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = start; // Time of last frame
	float frameTime = 0.0f;

	std::vector<float> frameOffset{ 0, 0 };

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	bool isRunning = true;

	while (isRunning) // render loop
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		while (SDL_PollEvent(&event)) // event loop
		{
			if (event.type == SDL_QUIT || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
				isRunning = false;
		}
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//view = camera.getViewMatrix();
		//projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

		glActiveTexture(GL_TEXTURE0);

		frameTime += deltaTime;

		indexbufferoffset = 0;
		for (unsigned int i = 0; i < objectCount; i++)
		{
			glBindVertexArray(VAOs[i]);

			glBindTexture(GL_TEXTURE_2D, textures[i]);

			shaderProgram.setMat4("model", objects[i].model());

			if (frameTime >= 0.1f)
			{
				frameTime = 0.0f;
				frameOffset[0] += objects[i].frameWidth();

				if (frameOffset[0] >= 1)
				{
					frameOffset[0] = 0.0f;
					frameOffset[1] -= objects[i].frameHeight();

					if (frameOffset[1] <= 0)
					{
						frameOffset[1] = 0.0f;
					}
				}
			}
			shaderProgram.set2Float("frameOffset", frameOffset[0], frameOffset[1]);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(indexbufferoffset * sizeof(unsigned int)));
			glBindVertexArray(0);

			indexbufferoffset += 6;
		}

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
