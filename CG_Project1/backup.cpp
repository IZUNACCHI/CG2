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

	//SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	//SDL_SetRelativeMouseMode(SDL_TRUE);

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
	LoadTexture(tex_Text_Big, "resources/graphics/graphics/font16x16.bmp");
	GLuint tex_Text_Small;
	LoadTexture(tex_Text_Small, "resources/graphics/graphics/Font8x8.bmp");

	std::map<char, std::vector<float>> small_chars_map;
	std::map<char, std::vector<float>> big_chars_map;
	MapChars(small_chars_map, 8.0f, 64.0f, 128.0f, big_chars_map, 16.0f, 128.0f, 192.0f);


	//for (std::map<char, std::vector<float>>::iterator it = small_chars_map.begin(); it != small_chars_map.end(); it++)
	//{
	//	std::cout << it->first << '\n' <<
	//		it->second[0] << ", " << it->second[1] << ", " << it->second[2] << ", " << it->second[3] << ", " <<
	//		it->second[4] << ", " << it->second[5] << ", " << it->second[6] << ", " << it->second[7] << std::endl;
	//};

	GLuint tex_Drone;
	LoadTexture(tex_Drone, "resources/graphics/graphics/drone.bmp");
	GLuint tex_Rusher;
	LoadTexture(tex_Rusher, "resources/graphics/graphics/rusher.bmp");
	GLuint tex_Loner;
	LoadTexture(tex_Loner, "resources/graphics/graphics/LonerA.bmp");

	GLuint tex_Background;
	LoadTexture(tex_Background, "resources/graphics/graphics/galaxy2.bmp");
	GLuint tex_paralax;
	LoadTexture(tex_paralax, "resources/graphics/graphics/Blocks.bmp");

	GLuint tex_Ship;
	LoadTexture(tex_Ship, "resources/graphics/graphics/Ship1.bmp");
	GLuint tex_missile_1;
	LoadTexture(tex_missile_1, "resources/graphics/graphics/missile.bmp");

	GLuint tex_missile_enemy;
	LoadTexture(tex_missile_enemy, "resources/graphics/graphics/EnWeap6.bmp");

	GLuint tex_ui_life;
	LoadTexture(tex_ui_life, "resources/graphics/graphics/PULife.bmp");

	GLuint tex_PU_shield;
	LoadTexture(tex_PU_shield, "resources/graphics/graphics/PUShield.bmp");
	GLuint tex_PU_weapon;
	LoadTexture(tex_PU_weapon, "resources/graphics/graphics/PUWeapon.bmp");

	GLuint tex_aster_metal_1;
	LoadTexture(tex_aster_metal_1, "resources/graphics/graphics/MAster96.bmp");
	GLuint tex_aster_metal_2;
	LoadTexture(tex_aster_metal_2, "resources/graphics/graphics/MAster64.bmp");
	GLuint tex_aster_metal_3;
	LoadTexture(tex_aster_metal_3, "resources/graphics/graphics/MAster32.bmp");

	GLuint tex_big_stone;
	LoadTexture(tex_big_stone, "resources/graphics/graphics/SAster96.bmp");
	GLuint tex_med_stone;
	LoadTexture(tex_med_stone, "resources/graphics/graphics/SAster64.bmp");
	GLuint tex_s_stone;
	LoadTexture(tex_s_stone, "resources/graphics/graphics/SAster32.bmp");

	GLuint tex_explosion;
	LoadTexture(tex_explosion, "resources/graphics/graphics/explode16.bmp");

	GLuint tex_companion;
	LoadTexture(tex_companion, "resources/graphics/graphics/clone.bmp");

	// The only thing the user must do is initialize an "Object" and push it back into the vector of objects.
	Actor drone(tex_Drone, 256, 64, 32, 32, 2, objects, true);
	objects.push_back(drone);

	Actor loner(tex_Loner, 256, 256, 64, 64, 2, objects, -0.5, 0.0f, true);
	objects.push_back(loner);

	Actor rusher(tex_Rusher, 256, 192, 64, 32, 2, objects, 0.5f, 0.0f, true);
	objects.push_back(rusher);

	Actor background(tex_Background, 640, 480, 640, 480, 0, objects, false);
	objects.push_back(background);
	
	Actor parallax1(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.2, false, objects, 0.65f, 1.1f, false, "parallax1");
	objects.push_back(parallax1);
	Actor parallax1_1(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.2, false, objects, 0.65f, 1.3f, false, "parallax1_1");
	objects.push_back(parallax1_1);
	Actor parallax1_2(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.2, false, objects, 0.65f, 1.5f, false, "parallax1_2");
	objects.push_back(parallax1_2);

	Actor parallax2(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.3, true, objects, -0.65f, 1.7f, false, "parallax2");
	objects.push_back(parallax2);
	Actor parallax2_1(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.3, true, objects, -0.65f, 2.1f, false, "parallax2_1");
	objects.push_back(parallax2_1);
	Actor parallax2_2(tex_paralax, 512, 2048, 32, 7, 4, 706, 0.3, true, objects, -0.65f, 2.5f, false, "parallax2_2");
	objects.push_back(parallax2_2);

	Actor ui_life(tex_ui_life, 32, 32, 32, 32, 4, objects, -0.9f, -0.5f, false);
	objects.push_back(ui_life);
	Actor ui_life1(tex_ui_life, 32, 32, 32, 32, 4, objects, -0.79f, -0.5f, false);
	objects.push_back(ui_life1);

	Actor pu_shield(tex_PU_shield, 128, 64, 32, 32, 1, objects, 0.4f, 0.3f, true);
	objects.push_back(pu_shield);
	Actor pu_weapon(tex_PU_weapon, 128, 64, 32, 32, 1, objects, -0.4f, 0.45f, true);
	objects.push_back(pu_weapon);

	Actor aster_metal_big(tex_aster_metal_1, 480, 480, 96, 96, 1, objects, 0.8f, 0.3f, true);
	objects.push_back(aster_metal_big);
	Actor aster_metal_med(tex_aster_metal_2, 512, 192, 64, 64, 1.1, objects, 0.6f, 0.2f, true);
	objects.push_back(aster_metal_med);
	Actor aster_metal_small(tex_aster_metal_3, 256, 64, 32, 32, 1.2, objects, 0.6f, -0.4f, true);
	objects.push_back(aster_metal_small);
	
	Actor ship(tex_Ship, 448, 64, 64, 64, 3, objects, 0.0f, -0.5f, true, "ship");
	Animation left("left", { 2, 1, 0}, false);
	Animation right("right", { 4, 5, 6}, false);
	Animation center("center", { 3 }, true);
	ship.m_animations.reserve(ship.m_animations.size() + 3);
	ship.m_animations.push_back(left);
	ship.m_animations.push_back(right);
	ship.m_animations.push_back(center);
	ship.currentAnimation = "center";
	objects.push_back(ship);

	Actor missile1(tex_missile_1, 32, 48, 16, 16, 2, objects, 0.0f, -1.5f, true, "miss");
	Animation type1("type1", { 0, 1 }, true);
	Animation type2("type2", { 2, 3 }, true);
	Animation type3("type3", { 4, 5 }, true);
	missile1.m_animations.reserve(missile1.m_animations.size() + 3);
	missile1.currentAnimation = "type1";
	missile1.m_animations.push_back(type1);
	missile1.m_animations.push_back(type2);
	missile1.m_animations.push_back(type3);
	objects.push_back(missile1);

	Actor enemyProj(tex_missile_enemy, 128, 16, 16, 16, 1.9, objects, -0.55f, 0.0f, true, "missEnemy");
	objects.push_back(enemyProj);

	Actor bigStone(tex_big_stone, 480, 480, 96, 96, 2, objects, 0.1f, 0.6f, true, "bigS");
	objects.push_back(bigStone);
	Actor medStone(tex_med_stone, 512, 192, 64, 64, 2.1, objects, 0.0f, 1.8f, true, "medS");
	objects.push_back(medStone);
	Actor sStone(tex_s_stone, 256, 64, 32, 32, 2.2, objects, 0.0f, 1.8f, true, "sS");
	objects.push_back(sStone);

	//companion
	Animation CompAppear("compAppear", { 16, 17, 18, 19 }, false);
	Animation CompDisappear("compDisappear", { 19, 18, 17, 16 }, false);
	Animation CompLoop("compLoop", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, true);

	Actor companionLeft(tex_companion, 128, 160, 32, 32, 4, objects, 0.0f, 0.0f, true, "companionL");
	companionLeft.m_animations.push_back(CompAppear);
	companionLeft.m_animations.push_back(CompDisappear);
	companionLeft.m_animations.push_back(CompLoop);
	companionLeft.currentAnimation = "compLoop";
	//companionLeft.Disable();
	objects.push_back(companionLeft);

	Actor companionRight(tex_companion, 128, 160, 32, 32, 4, objects, 0.0f, 0.0f, true, "companionR");
	companionRight.m_animations.push_back(CompAppear);
	companionRight.m_animations.push_back(CompDisappear);
	companionRight.m_animations.push_back(CompLoop);
	companionRight.currentAnimation = "compLoop";
	//companionRight.Disable();
	objects.push_back(companionRight);

	Actor explosion(tex_explosion, 80, 32, 16, 16, 4, objects, 1.5f, 1.5f, true, "explosion");
	Animation expAnim("expAnim", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, false);
	explosion.currentAnimation = "expAnim";
	explosion.m_animations.push_back(expAnim);
	objects.push_back(explosion);

	Text txt_hi_score("Hi Score", tex_Text_Small, small_chars_map, true, objects, -0.1f, 0.68f);
	objects.push_back(txt_hi_score);

	std::string highScore_text{ "0007497500" };
	Text txt_hi_score1(highScore_text, tex_Text_Small, small_chars_map, true, objects, -0.125f, 0.64f);
	objects.push_back(txt_hi_score1);

	std::string p1_text{ "Player One" };
	Text txt_player_one(p1_text, tex_Text_Small, small_chars_map, true, objects, -0.95f, 0.68f);
	objects.push_back(txt_player_one);

	std::string score_text { "0003855055" };
	Text txt_score(score_text, tex_Text_Big, big_chars_map, false, objects, -0.95f, 0.6f);
	objects.push_back(txt_score);

	
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
	//model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 0.0f));
	//std::cout << glm::to_string(model) << std::endl;
	//std::cout << model[3].z << std::endl;

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
	float shipIndex = 0;
	float missIndex = 0;
	float missEnemyIndex = 0;
	float bigSIndex = 0;
	float medSIndex = 0;
	float sSIndex = 0;
	float explosionIndex = 0;
	float compLIndex = 0;
	float compRIndex = 0;
	float parallax1Index = 0;
	float parallax1_1Index = 0;
	float parallax1_2Index = 0;
	float parallax2Index = 0;
	float parallax2_1Index = 0;
	float parallax2_2Index = 0;
	int stageS = 0;
	bool doOnceSpace = true;
	bool doOnceB = true;

	for (std::map<float, Object>::iterator it = sortedObjects.begin(); it != sortedObjects.end(); it++)
	{
		
		if (it->second.m_name == "ship") {
			shipIndex = it->first;
			
		}
		else if (it->second.m_name == "miss") {
			missIndex = it->first;
		}
		else if (it->second.m_name == "missEnemy") {
			missEnemyIndex = it->first;
		}
		else if (it->second.m_name == "bigS") {
			bigSIndex = it->first;

		}
		else if (it->second.m_name == "medS") {
			medSIndex = it->first;

		}
		else if (it->second.m_name == "sS") {
			sSIndex = it->first;

		}
		else if (it->second.m_name == "explosion") {
			explosionIndex = it->first;
		}
		else if (it->second.m_name == "companionL") {
			compLIndex = it->first;
		}
		else if (it->second.m_name == "companionR") {
			compRIndex = it->first;
		}
		else if (it->second.m_name == "parallax1") {
			parallax1Index = it->first;
		}
		else if (it->second.m_name == "parallax1_1") {
			parallax1_1Index = it->first;
		}
		else if (it->second.m_name == "parallax1_2") {
			parallax1_2Index = it->first;
		}
		else if (it->second.m_name == "parallax2") {
			parallax2Index = it->first;
		}
		else if (it->second.m_name == "parallax2_1") {
			parallax2_1Index = it->first;
		}
		else if (it->second.m_name == "parallax2_2") {
			parallax2_2Index = it->first;
		}
	}
	float rows;
	float companionOffset_x = 0;
	float companionOffset_y = -0.08f;

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
			if (sortedObjects[shipIndex].m_model[3].x > -0.9f) {
				sortedObjects[shipIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime);
				if (sortedObjects[shipIndex].currentAnimation != "left") {
					sortedObjects[shipIndex].resetAnimation();
				}
				sortedObjects[shipIndex].currentAnimation = "left";
			}
		}
		else {
			if (keyState[SDL_SCANCODE_D]) {
				if (sortedObjects[shipIndex].m_model[3].x < 0.9f) {
					sortedObjects[shipIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
					if (sortedObjects[shipIndex].currentAnimation != "right") {
						sortedObjects[shipIndex].resetAnimation();
					}
					sortedObjects[shipIndex].currentAnimation = "right";
				}
			}
			else {
				if (sortedObjects[shipIndex].currentAnimation != "center") {
					sortedObjects[shipIndex].resetAnimation();
				}
				sortedObjects[shipIndex].currentAnimation = "center";
			}
		}
		if (keyState[SDL_SCANCODE_W]) {
			if (sortedObjects[shipIndex].m_model[3].y < 0.6f) {
				sortedObjects[shipIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);
				companionOffset_y = companionOffset_y + 0.15f * deltaTime;
				companionOffset_x = companionOffset_x + 0.025f * deltaTime;
			}
		}
		if (keyState[SDL_SCANCODE_S]) {
			if (sortedObjects[shipIndex].m_model[3].y > -0.6f) {
				sortedObjects[shipIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime);
				companionOffset_y = companionOffset_y - 0.15f * deltaTime;
				companionOffset_x = companionOffset_x - 0.025f * deltaTime;
			}
		}
		if (keyState[SDL_SCANCODE_SPACE]) {
			if (doOnceSpace) {
				sortedObjects[explosionIndex].m_model = sortedObjects[missIndex].m_model;
				sortedObjects[explosionIndex].resetAnimation();
				sortedObjects[missIndex].m_model = sortedObjects[shipIndex].m_model;
				doOnceSpace = false;
			}	
		}
		else {
			doOnceSpace = true;
		}
		if (keyState[SDL_SCANCODE_1]) {
			sortedObjects[missIndex].resetAnimation();
			sortedObjects[missIndex].currentAnimation = "type1";
		}
		else if(keyState[SDL_SCANCODE_2]) {
			sortedObjects[missIndex].resetAnimation();
			sortedObjects[missIndex].currentAnimation = "type2";
		}
		else if (keyState[SDL_SCANCODE_3]) {
			sortedObjects[missIndex].resetAnimation();
			sortedObjects[missIndex].currentAnimation = "type3";
		}
		if (keyState[SDL_SCANCODE_B]) {
			if (doOnceB) {
				switch (stageS) {
				case 0:
					sortedObjects[medSIndex].m_model = sortedObjects[bigSIndex].m_model;
					sortedObjects[bigSIndex].m_model = glm::translate(sortedObjects[bigSIndex].m_model, glm::vec3(1.2f, 0.0f, 0.0f));
					stageS++;
					break;
				case 1:
					sortedObjects[sSIndex].m_model = sortedObjects[medSIndex].m_model;
					sortedObjects[medSIndex].m_model = glm::translate(sortedObjects[medSIndex].m_model, glm::vec3(1.2f, 0.0f, 0.0f));
					stageS++;
					break;
				case 2:
					sortedObjects[bigSIndex].m_model = sortedObjects[sSIndex].m_model;
					sortedObjects[sSIndex].m_model = glm::translate(sortedObjects[sSIndex].m_model, glm::vec3(1.2f, 0.0f, 0.0f));
					stageS = 0;
					break;
				}
				doOnceB = false;
			}
		}
		else {
			doOnceB = true;
		}
		//parallax
		sortedObjects[parallax1Index].m_model = glm::translate(sortedObjects[parallax1Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax1Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax1Index].m_model = parallax1.m_model;
		}
		sortedObjects[parallax1_1Index].m_model = glm::translate(sortedObjects[parallax1_1Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax1_1Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax1_1Index].m_model = parallax1_1.m_model;
		}
		sortedObjects[parallax1_2Index].m_model = glm::translate(sortedObjects[parallax1_2Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax1_2Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax1_2Index].m_model = parallax1_2.m_model;
		}
		//parallax2
		sortedObjects[parallax2Index].m_model = glm::translate(sortedObjects[parallax2Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax2Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax2Index].m_model = parallax2.m_model;
		}
		sortedObjects[parallax2_1Index].m_model = glm::translate(sortedObjects[parallax2_1Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax2_1Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax2_1Index].m_model = parallax2_1.m_model;
		}
		sortedObjects[parallax2_2Index].m_model = glm::translate(sortedObjects[parallax2_2Index].m_model, glm::vec3(0.0f, -0.1f, 0.0f) * deltaTime);
		if (sortedObjects[parallax2_2Index].m_model[3].y <= -1.0) {
			sortedObjects[parallax2_2Index].m_model = parallax2_2.m_model;
		}
		//missile move with time
		sortedObjects[missIndex].m_model = glm::translate(sortedObjects[missIndex].m_model, glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);
		sortedObjects[missEnemyIndex].m_model = glm::translate(sortedObjects[missEnemyIndex].m_model, glm::vec3(0.0f, -0.35f, 0.0f) * deltaTime);
		//reset enemy projectile
		if (sortedObjects[missEnemyIndex].m_model[3].y <= -1.0) {
			sortedObjects[missEnemyIndex].m_model = glm::translate(loner.m_model, glm::vec3(-0.05f, 0.0f, 0.0f));
		}
		//companion left
		sortedObjects[compLIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(0.25f - companionOffset_x, 0.0f + companionOffset_y, 0.0f));
		//companion right
		sortedObjects[compRIndex].m_model = glm::translate(sortedObjects[shipIndex].m_model, glm::vec3(-0.25f + companionOffset_x, 0.0f + companionOffset_y, 0.0f));
		//Cpman
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
				if (it->second.m_animations.size() > 0) {
					it->second.frameOffset_x = it->second.getAnimationByName(it->second.currentAnimation).getFrame() * it->second.frameWidth();
					it->second.frameOffset_y = 0;
					if (it->second.frameOffset_x >= 1)
					{
						rows = floor(it->second.frameOffset_x); //how many rows ex: offset_x = 5.4 then off set x = 0.4 and offset y = -5 
						it->second.frameOffset_x -= rows; 
						it->second.frameOffset_y = it->second.frameHeight() * -rows;

					}
				}
				else {
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
