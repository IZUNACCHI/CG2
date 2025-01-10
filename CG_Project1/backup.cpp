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

	// The only thing the user must do is initialize an "Object" and push it back into the vector of objects.
	Object drone("resources/graphics/graphics/drone.bmp", 32, 32, objects, windowSurface);
	objects.push_back(drone);

	Object rusher("resources/graphics/graphics/rusher.bmp", 64, 32, objects, 0.5f, 0.0f, windowSurface);
	//Object rusher("resources/graphics/graphics/rusher.bmp", 64, 32, objects);
	objects.push_back(rusher);
	//rusher.m_model = glm::translate(rusher.m_model, glm::vec3(0.5f, 0.0f, 0.0f));

	Object loner("resources/graphics/graphics/LonerA.bmp", 64, 64, objects, -0.5f, 0.0f, windowSurface);
	objects.push_back(loner);
	
	Object ship("resources/graphics/graphics/Ship1.bmp", 64, 64, objects, 0.0f, -0.5f, windowSurface);
	Animation left("left", { 2, 1, 0}, false);
	Animation right("right", { 4, 5, 6}, false);
	Animation center("center", { 3 }, false);
	ship.m_animations.reserve(ship.m_animations.size() + 3);
	ship.m_animations.push_back(left);
	ship.m_animations.push_back(right);
	ship.m_animations.push_back(center);
	left.~Animation();
	right.~Animation();
	center.~Animation();
	ship.currentAnimation = "center";
	objects.push_back(ship);
	auto shipIndex = objects.size() - 1;

	Object missile1("resources/graphics/graphics/missile.bmp", 16, 16, objects, 0.0f, -1.5f, windowSurface);
	Animation type1("type1", { 0, 1 }, true);
	missile1.m_animations.reserve(missile1.m_animations.size() + 1);
	missile1.currentAnimation = "type1";
	missile1.m_animations.push_back(type1);
	objects.push_back(missile1);
	
	size_t vbo_size = 0;
	size_t ibo_size = 0;

	for (unsigned int i = 0; i < Object::objectCount; i++)
	{
		vbo_size += objects[i].vertexBufferSize();
		ibo_size += objects[i].indexBufferSize();
	}

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(float), 0, GL_STATIC_DRAW);

	size_t vertexbufferoffset = 0;
	for (unsigned int i = 0; i < Object::objectCount; i++)
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
	for (unsigned int i = 0; i < Object::objectCount; i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexbufferoffset * sizeof(unsigned int),					// offset
													objects[i].indexBufferSize() * sizeof(unsigned int),	// size
														objects[i].indices());								// data
		indexbufferoffset += objects[i].indexBufferSize();
	}

	vertexbufferoffset = 0;
	indexbufferoffset = 0;
	for (unsigned int i = 0; i < Object::objectCount; i++)
	{
		glBindVertexArray(objects[i].vao());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		shaderProgram.setVertexAttribPointer("position", 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vertexbufferoffset * sizeof(float)));
		shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)((vertexbufferoffset * sizeof(float)) + (2 * sizeof(float))));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		vertexbufferoffset += objects[i].vertexBufferSize();
	}

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	glm::mat4 view;
	view = camera.getViewMatrix();

	//glm::mat4 model(1.0f);
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 0.1f, 100.0f);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shaderProgram.setInt("Texture", 0);

	SDL_Event event;

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = start; // Time of last frame
	float frameTime = 0.0f;

	//std::vector<float> frameOffset{ 0, 0 };

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	bool isRunning = true;

	bool animUpdate = false;

	std::cout << Object::objectCount << std::endl;

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
		//ship
		if (keyState[SDL_SCANCODE_A]) {
			objects[shipIndex].m_model = glm::translate(objects[shipIndex].m_model, glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime);
			if (objects[shipIndex].currentAnimation != "left") {
				objects[shipIndex].resetAnimation();
			}
			objects[shipIndex].currentAnimation = "left";
		}
		else {
			if (keyState[SDL_SCANCODE_D]) {
				objects[shipIndex].m_model = glm::translate(objects[shipIndex].m_model, glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
				if (objects[shipIndex].currentAnimation != "right") {
					objects[shipIndex].resetAnimation();
				}
				objects[shipIndex].currentAnimation = "right";
			}
			else {
				if (objects[shipIndex].currentAnimation != "center") {
					objects[shipIndex].resetAnimation();
				}
				objects[shipIndex].currentAnimation = "center";
			}
		}
		if (keyState[SDL_SCANCODE_W]) {
			objects[shipIndex].m_model = glm::translate(objects[shipIndex].m_model, glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);
		}
		if (keyState[SDL_SCANCODE_S]) {
			objects[shipIndex].m_model = glm::translate(objects[shipIndex].m_model, glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		view = camera.getViewMatrix();
		projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 0.1f, 100.0f);

		glActiveTexture(GL_TEXTURE0);

		frameTime += deltaTime;

		if (frameTime >= 0.1f)
		{
			animUpdate = true;
		}

		indexbufferoffset = 0;
		for (unsigned int i = 0; i < Object::objectCount; i++)
		{
			glBindVertexArray(objects[i].vao());

			glBindTexture(GL_TEXTURE_2D, objects[i].texture());

			if (animUpdate)
			{
				frameTime = 0.0f;
				if (objects[i].m_animations.size() > 0) {
					objects[i].frameOffset_x = objects[i].getAnimationByName(objects[i].currentAnimation).getFrame() * objects[i].frameWidth();
					if (objects[i].frameOffset_x >= 1)
					{
						objects[i].frameOffset_x -= (objects[i].frameWidth() / objects[i].frameOffset_x);
						objects[i].frameOffset_y -= objects[i].frameHeight() * (objects[i].frameWidth() / objects[i].frameOffset_x);

						if (objects[i].frameOffset_y <= -1)
						{
							objects[i].frameOffset_y = 0.0f;
						}
					}
				}
				else {
					objects[i].frameOffset_x += objects[i].frameWidth();
					if (objects[i].frameOffset_x >= 1)
					{
						objects[i].frameOffset_x = 0.0f;
						objects[i].frameOffset_y -= objects[i].frameHeight();

						if (objects[i].frameOffset_y <= -1)
						{
							objects[i].frameOffset_y = 0.0f;
						}
					}
				}

			}
			shaderProgram.set2Float("frameOffset", objects[i].frameOffset_x, objects[i].frameOffset_y);
			shaderProgram.setMat4("model", objects[i].model());

			glDrawElements(GL_TRIANGLES, objects[i].indexBufferSize(), GL_UNSIGNED_INT, (void*)(indexbufferoffset * sizeof(unsigned int)));
			glBindVertexArray(0);

			indexbufferoffset += objects[i].indexBufferSize();
		}

		animUpdate = false;

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
