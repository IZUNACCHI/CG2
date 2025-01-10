#include <SDL.h>
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "vboindexer.hpp"
#include "loader_wavefront.hpp"
#include "loader_texture.hpp"
#include "object.h"
#include "actor_class.hpp"
#include "Utilities.h"
#include "text_class.h"

#include <iostream>
#include <vector>
#include <map>

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

	SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

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
	//unsigned int objectCount = 0;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// Load all textures first
	GLuint tex_Text_Big;
	LoadTexture(tex_Text_Big, "resources/graphics/graphics/font16x16.bmp", windowSurface);
	GLuint tex_Text_Small;
	LoadTexture(tex_Text_Small, "resources/graphics/graphics/Font8x8.bmp", windowSurface);
	std::map<char, std::vector<float>> small_chars_map;
	std::map<char, std::vector<float>> big_chars_map;
	MapChars(small_chars_map, big_chars_map);


	//for (std::map<char, std::vector<float>>::iterator it = small_chars_map.begin(); it != small_chars_map.end(); it++)
	//{
	//	std::cout << it->first << '\n' <<
	//		it->second[0] << ", " << it->second[1] << ", " << it->second[2] << ", " << it->second[3] << ", " <<
	//		it->second[4] << ", " << it->second[5] << ", " << it->second[6] << ", " << it->second[7] << std::endl;
	//};

	GLuint tex_Drone;
	LoadTexture(tex_Drone, "resources/graphics/graphics/drone.bmp", windowSurface);
	GLuint tex_Rusher;
	LoadTexture(tex_Rusher, "resources/graphics/graphics/rusher.bmp", windowSurface);
	GLuint tex_Loner;
	LoadTexture(tex_Loner, "resources/graphics/graphics/LonerA.bmp", windowSurface);
	GLuint tex_Background;
	LoadTexture(tex_Background, "resources/graphics/graphics/galaxy2.bmp", windowSurface);

	// The only thing the user must do is initialize an "Object" and push it back into the vector of objects.
	Actor drone(tex_Drone, 256, 64, 32, 32, 2, objects, true);
	objects.push_back(drone);

	Actor rusher(tex_Rusher, 256, 192, 64, 32, 2, objects, 0.5f, 0.0f, true);
	objects.push_back(rusher);

	Actor loner(tex_Loner, 256, 256, 64, 64, 2, objects, -0.5, 0.0f, true);
	objects.push_back(loner);

	Actor background(tex_Background, 640, 480, 640, 480, 0, objects, false);
	objects.push_back(background);

	Actor drone2(tex_Drone, 256, 64, 32, 32, 2, objects, 0.0f, 0.5f, true);
	objects.push_back(drone2);


	size_t vbo_size = 0;
	size_t ibo_size = 0;

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		vbo_size += objects[i].vertexBufferSize();
		ibo_size += objects[i].indexBufferSize();
	}

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(float), 0, GL_STATIC_DRAW);

	size_t vertexbufferoffset = 0;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, vertexbufferoffset * sizeof(float),						// offset
											objects[i].vertexBufferSize() * sizeof(float),			// size
												objects[i].vertices());								// data
		vertexbufferoffset += objects[i].vertexBufferSize();
	}

	GLuint ibo;
	glGenBuffers(1, &ibo); // Generate 1 element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size * sizeof(unsigned int), 0, GL_STATIC_DRAW);

	size_t indexbufferoffset = 0;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexbufferoffset * sizeof(unsigned int),					// offset
													objects[i].indexBufferSize() * sizeof(unsigned int),	// size
														objects[i].indices());								// data
		indexbufferoffset += objects[i].indexBufferSize();
	}

	vertexbufferoffset = 0;
	indexbufferoffset = 0;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		glBindVertexArray(objects[i].vao());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		shaderProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(vertexbufferoffset * sizeof(float)));
		shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)((vertexbufferoffset * sizeof(float)) + (3 * sizeof(float))));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		vertexbufferoffset += objects[i].vertexBufferSize();
	}

	Camera camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 model(1.0f);
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, 0.1f, 100.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	shaderProgram.setInt("Texture", 0);

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		shaderProgram.setMat4("model", objects[i].model());
		//std::cout << glm::to_string(objects[i].model()) << std::endl;
	}

	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	SDL_Event event;

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = start; // Time of last frame
	float frameTime = 0.0f;


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	bool isRunning = true;

	bool animUpdate = false;

	std::map<float, Object> sortedObjects = SortObjects(objects);

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

		shaderProgram.use();

		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);

		frameTime += deltaTime;

		if (frameTime >= 0.1f)
		{
			animUpdate = true;
		}

		indexbufferoffset = 0;

		for (std::map<float, Object>::iterator it = sortedObjects.begin(); it != sortedObjects.end(); it++)
		{
			glBindVertexArray(it->second.vao());

			glBindTexture(GL_TEXTURE_2D, it->second.texture());

			if (animUpdate && it->second.animate)
			{
				frameTime = 0.0f;
				it->second.frameOffset_x += it->second.frameWidth();

				if (it->second.frameOffset_x >= 1)
				{
					it->second.frameOffset_x = 0.0f;
					it->second.frameOffset_y -= it->second.frameHeight();

					if (it->second.frameOffset_y <= -1)
					{
						it->second.frameOffset_y = 0.0f;
					}
				}
			}
			shaderProgram.set2Float("frameOffset", it->second.frameOffset_x, it->second.frameOffset_y);
			shaderProgram.setMat4("model", it->second.model());

			glDrawElements(GL_TRIANGLES, it->second.indexBufferSize(), GL_UNSIGNED_INT, (void*)(indexbufferoffset * sizeof(unsigned int)));
			glBindVertexArray(0);

			indexbufferoffset += it->second.indexBufferSize();
		}

		animUpdate = false;

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
