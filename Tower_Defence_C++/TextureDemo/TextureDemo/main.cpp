//Name: heqingwang
//Student#: 101051240

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <string>
#include <thread>
#include <locale>
#include <cstdlib>
#include <cmath>

#include "Shader.h"
#include "ParticleShader.h"
#include "Window.h"
#include "PlayerGameObject.h"
#include "Graph.h"
#include "Enemy.h"
#include "Tower.h"



using namespace std;
// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "TD Game Demo";
const unsigned int window_width_g = 1060;
const unsigned int window_height_g = 720;
const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);


// Global texture info
GLuint tex[11];
GLuint towerTex[4];
GLuint towerParticleTex[3];
GLuint enemyTex[3];

Tower* selectedTower = NULL;
double xpos, ypos;

GLuint sprite_vbo;
GLuint sprite_ebo;
GLuint partic_vbo;
GLuint partic_ebo;

int level = 1;
int enemyDestoried = 0;
int currency = 300;
int Health = 3;

const float speed = 0.1;
// Global game object info
std::vector<Enemy*> Enemies;
std::vector<Tower*> Towers;



// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {

	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
	//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
	//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &sprite_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &sprite_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(face);

}


int CreateParticleArray(void) {

	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[1000 * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < 1000; i++)
	{
		if (i % 4 == 0)
		{
			theta = (6.28*(rand() % 1000) / 1000.0f);//(2*(rand() % 10000) / 10000.0f -1.0f)*0.13f;
			r = 0.2f + 0.8*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}
		// position	
		particleatt[i*vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i*vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		// velocity
		particleatt[i*vertex_attr + 2] = sin(theta)*r;
		particleatt[i*vertex_attr + 3] = cos(theta)*r;

		// phase
		particleatt[i*vertex_attr + 4] = tmod;

		// texture coordinate
		particleatt[i*vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
		particleatt[i*vertex_attr + 6] = vertex[(i % 4) * 7 + 6];


	}


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint manyface[1000 * 6];

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 6; j++)
			manyface[i * 6 + j] = face[j] + i * 4;
	}

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &partic_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &partic_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);

}

void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void setallTexture(void)
{
//	tex = new GLuint[4];
	glGenTextures(10, tex);
	setthisTexture(tex[0], "resource\\road.png");
	setthisTexture(tex[1], "resource\\Particle\\explosion.png");
	setthisTexture(tex[2], "resource\\R.png");
	setthisTexture(tex[3], "alien.png");
	setthisTexture(tex[4], "resource\\portal.png");
	setthisTexture(tex[5], "resource\\base.png");
	setthisTexture(tex[6], "resource\\text.png");
	setthisTexture(tex[7], "resource\\endScreen.png");
	setthisTexture(tex[8], "resource\\startScreen.png");
	setthisTexture(tex[9], "resource\\winScreen.jpg");
	setthisTexture(tex[10], "resource\\plant.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);
}

void readTowers(string filename, Tower** towers, int number, int size) {
	glGenTextures(number, towerTex);
	glGenTextures(number, towerParticleTex);
	FILE* file;
	char* fn = (char*)filename.data();
	file = fopen(fn, "r");
	if (file == NULL) {
		cout << "Error! Failed to load towers!" << endl;
		return;
	}
	char texture[128];
	int cost;
	int damage;
	int range;
	int level;
	int mobile;
	int type;
	float chargeTime;
	char particleTex[128];
	glm::vec3 pos = glm::vec3(0.0f);
	for (int i = 0; i < number; i++) {
		fscanf(file, "%s", &texture);
		fscanf(file, "%d", &damage);
		fscanf(file, "%d", &range);
		fscanf(file, "%d", &cost);
		fscanf(file, "%d", &type);
		fscanf(file, "%d", &level);
		fscanf(file, "%f", &chargeTime);
		fscanf(file, "%d", &mobile);
		fscanf(file, "%s", &particleTex);
		setthisTexture(towerTex[i], texture);
		setthisTexture(towerParticleTex[i], particleTex);
		towers[i] = new Tower(pos, towerTex[i], size);
		towers[i]->setDamage(damage);
		towers[i]->setRange(range);
		towers[i]->setCost(cost);
		towers[i]->setType(type);
		towers[i]->setLevel(level);
		towers[i]->setMove(mobile == 1 ? true : false);
		towers[i]->setFireInterval(chargeTime);
		towers[i]->setParticleTex(towerParticleTex[i]);
		cout << type<<","<<texture << "," << towers[i]->getDamage() << "," << towers[i]->getRange() << "," << towers[i]->getMove() << endl;
	}
	fclose(file);

}

void readEnemies(string filename, Enemy** enemies, int number, int size) {
	glGenTextures(number, enemyTex);
	FILE* file;
	char* fn = (char*)filename.data();
	file = fopen(fn, "r");
	if (file == NULL) {
		cout << "Error! Failed to load enemies!" << endl;
		return;
	}
	char texture[128];
	int health;
	float speed;
	int type;
	int cur;
	glm::vec3 pos = glm::vec3(0.0f);
	for (int i = 0; i < number; i++) {
		fscanf(file, "%s", &texture);
		fscanf(file, "%d", &health);
		fscanf(file, "%d", &type);
		fscanf(file, "%f", &speed);
		fscanf(file, "%d", &cur);
		setthisTexture(enemyTex[i], texture);
		enemies[i] = new Enemy(pos, enemyTex[i], size);
		enemies[i]->setHealth(health);
		enemies[i]->setOriginalSpeed(speed);
		enemies[i]->setType(type);
		enemies[i]->setCurrency(cur);
		cout << type << "," << texture << "," << enemies[i]->getHealth() << "," << enemies[i]->getCurrency() << endl;
	}
	fclose(file);

}


void renderShop(Shader &shader, Tower** tower, int size, int number) {
	for (int i = 0; i < number; i++) {

		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, tower[i]->getTexture());

		// Setup the transformation matrix for the shader
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.4f, -4.0 + 1.5*i, 0.0f));
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.75f));
		// Set the transformation matrix in the shader
		glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
		shader.setUniformMat4("transformationMatrix", transformationMatrix);
		GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
		if (currency < tower[i]->getCost()) {
			glUniform3f(color_loc, -0.5f, -0.5f, -0.5f);
		}
		else{
			glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
		}
		// Draw the entity
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	}

}

void renderScreen(Shader &shader,int size,GLuint texture) {
	glClearColor(viewport_background_color_g[0],
		viewport_background_color_g[1],
		viewport_background_color_g[2], 0.0);
	glDepthMask(GL_TRUE); // allow writes to depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw the square

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);


	// Select proper shader program to use
	glUseProgram(shader.getShaderID());

	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
	shader.AttributeBinding();

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, -0.0 , 1.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.75f));
	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(10,10,1.0));
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
	glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
	// Draw the entity
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0); 

}

void renderText(std::string &stringToRender, Shader &textShader,glm::vec3 pos) {
	// Enable the shader and bind the proper text spritesheet texture
	textShader.enable();
	textShader.AttributeBinding();
	glBindTexture(GL_TEXTURE_2D, tex[6]);


	// Loop through each character and draw it
	for (int i = 0; i < stringToRender.size(); i++) {
		// We need to get the character and map it to a UV coordinate the represents where it is located in the text spritesheet texture
		// First get the character and make sure it is an upper case character (less cases to cover)
		int ascii = (int)std::toupper(stringToRender[i], std::locale());

		// Convert our ascii value into the range of [0, 35] (0, 1, 2, ... , A, B, C, .... , Z)
		if (ascii > 57)
			ascii -= 7;
		ascii -= 48;

		// Get the row and column of the character in our sprite sheet. Then we will let our vertex shader calculate the proper UVs
		int spritesheetSize = 6;
		float charUVSize = 1.0f / spritesheetSize;
		int charRow = ascii / spritesheetSize;
		int charCol = ascii % spritesheetSize;

		// Before we draw, we need to setup the transformation matrix for the text (doesn't use a view matrix)
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos+glm::vec3(0.025*i,0,0));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 1.0f));
		glm::mat4 transformationMatrix = translation * scale;

		// Setup uniforms
		textShader.setUniform1f("UVSize", charUVSize);
		textShader.setUniform1i("charCol", charCol);
		textShader.setUniform1i("charRow", charRow);
		textShader.setUniformMat4("transformationMatrix", transformationMatrix);
		textShader.setUniform3f("textColour", glm::vec3(1.0f, 0.0f, 0.0f));

		// Finally draw the character
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void mouseEvent(Tower** tower, Graph g) {
	glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);
	string s = "";
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && selectedTower == NULL) {
		if ((xpos - 995)* (xpos - 995) + (ypos - 310)*(ypos - 310) <= 1225 && currency >= tower[2]->getCost()) {
			selectedTower = tower[3];
			s = "4";
		}
		if ((xpos - 995)* (xpos - 995) + (ypos - 420)*(ypos - 420) <= 1225 && currency>= tower[2]->getCost()) {
			selectedTower = tower[2];
			s = "3";
		}
		else if ((xpos - 995)* (xpos - 995) + (ypos - 530)*(ypos - 530) <= 1225 && currency >= tower[1]->getCost()) {
			selectedTower = tower[1];
			s = "2";
		}
		else if ((xpos - 995)* (xpos - 995) + (ypos - 640)*(ypos - 640) <= 1225 && currency >= tower[0]->getCost() ){
			selectedTower = tower[0];
			s = "1";
		}
		cout << "select " << s << " tower" << endl;
	}
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		selectedTower = NULL;
	}
	if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && selectedTower != NULL) {
		int n = g.selectNode(xpos + 40 * 1.06, ypos);
		if (n >= 0&& g.getNode(n).isObstacle() && g.getNode(n).getBuildable()) {
			Tower* t = new Tower(glm::vec3(g.getNode(n).getX(), g.getNode(n).getY(), 0.0f), selectedTower->getTexture(), 6);
			t->setDamage(selectedTower->getDamage());
			t->setRange(selectedTower->getRange());
			t->setLevel(selectedTower->getLevel());
			t->setType(selectedTower->getType());
			t->setFireInterval(selectedTower->getFireInterval());
			t->setMove(selectedTower->getMove());
			t->setParticleTex(selectedTower->getParticleTex());
			cout << "place a tower" << endl;

			if (t->getType() == 4) {
				g.getNode(n).setEcost(666);
				t->setOrigin(glm::vec3(g.getNode(n).getX(), g.getNode(n).getY(), 0.0f));
			}

			Towers.push_back(t);
			currency -= selectedTower->getCost();
			selectedTower = NULL;


			// Placing tower will affect the weight of nearby node's weight !!!
			for (int i = 0; i < 1200; i += 1) {
				if ((abs(g.getNode(i).getId() - n) <= 3 || abs(g.getNode(i).getId() - (n + 40)) <= 3 || abs(g.getNode(i).getId() - (n + 80)) <= 3 || abs(g.getNode(i).getId() - (n - 40)) <= 3 || abs(g.getNode(i).getId() - (n - 80)) <= 3) && g.getNode(i).getObstacle()==false) {
					g.getNode(i).setEcost(5);
				} 
			}

			g.getNode(n).setBuildable(false);
		}

	}
}

void initialWorld(Graph g) {
	level = 1;
	enemyDestoried = 0;
	currency = 500;
	Health = 3;

	// Global game object info
	for (int i = 0; i < 1200; i++) {
		if (g.getNode(i).getECost() == 666) { g.getNode(i).setEcost(1000); }
		if (g.getNode(i).getECost() > 1 && g.getNode(i).getECost() < 600) { g.getNode(i).setEcost(1); }
	}

	Enemies.clear();
	Towers.clear();
}

void  renderSelected(Shader& shader) {

	glBindTexture(GL_TEXTURE_2D, selectedTower->getTexture());
	glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);
	float cursor_x_pos = (xpos / (float)(window_width_g / 2)) - 1.0f;	//transforms cursor position to screen coordinates
	float cursor_y_pos = ((ypos / (float)(window_height_g / 2)) - 1.0f)*-1;

	cursor_x_pos /= 0.2;
	cursor_y_pos /= 0.2;
	
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cursor_x_pos, cursor_y_pos, 0.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.4f, 0.6f));
	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
	glUniform3f(color_loc, -0.2f, -0.2f, -0.2f);
	// Draw the entity
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}

void drawParticles(GLuint particleprogram, int particlesize,GameObject* gameObj)
{

	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");
	int typeLocation = glGetUniformLocation(particleprogram, "type");
	int tLocation = glGetUniformLocation(particleprogram, "t");

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = 4-4*gameObj->getParticleTime();

	//rot = glm::translate(rot, glm::vec3(gameObj->getPosition().x, gameObj->getPosition().y, -0.5));
	rot = glm::scale(rot, glm::vec3(0.022, 0.0237, 0.1));
	rot = glm::translate(rot, glm::vec3((gameObj->getPosition().x-0.4)*9 , gameObj->getPosition().y*8.4, -0.50));
	rot = glm::rotate(rot, gameObj->getAngle(), glm::vec3(0, 0, 1));
	if(gameObj->getType() == 1 || gameObj->getType() == 2)	rot = glm::scale(rot, glm::vec3(1-k / 4));


	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);
	glUniform1f(timeLocation, k);
	glUniform1f(tLocation, k);
	glUniform1i(typeLocation, gameObj->getType());
	glBindTexture(GL_TEXTURE_2D, gameObj->getParticleTex());

	// Draw 
	glDrawElements(GL_TRIANGLES, 6 * particlesize, GL_UNSIGNED_INT, 0);

}

// Main function that builds and runs the game
int main(void){
	try {


		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);


		// Create geometry of the square
		int size = CreateSquare();
		Shader shader = Shader("shader.vert","shader.frag");
		setallTexture();


		int p_size = CreateParticleArray();
		ParticleShader p_shader("particleshader.vert", "particleshader.frag");

		// Set event callbacks

		Shader textShader("textShader.vert", "textShader.frag");
		textShader.AttributeBinding();

		//intialize tower resource
		int numOfTower = 4;
		Tower* towerResource[128];
		readTowers("resource\\Tower.txt", towerResource, numOfTower, size);


		//intialize enemy resource
		int numOfEnemy = 3;
		Enemy* enemyResource[128];
		readEnemies("resource\\Enemy.txt", enemyResource, numOfEnemy, size);

		//Setup 40x30 graph
		Graph g = Graph(40, 30, GameObject(glm::vec3(0.0f), tex[0], size),tex[0],tex[2], tex[10]);
		g.setST(tex[4]);
		g.setET(tex[5]);

		initialWorld(g);

		float X = -100;
		float Y = -100;
		float CurrentX = -100;
		float CurrentY = -100;
		float totalCost = 0;
		float Interval = 1;

		glm::vec3 cameraTranslatePos(glm::vec3(0.0f));
		float cameraZoom = 0.2f;

		int state = 1;

		// the enemies in tower's attack range
		std::vector<Enemy*> targetInrange;

		// Run the main loop
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {

			// Setup camera to focus on (0, 0)
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);

			// Zoom the display
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
				cameraZoom += 0.0004f;
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_X) == GLFW_PRESS) {
				cameraZoom -= 0.0004f;
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
				cameraTranslatePos += glm::vec3(0.2f, 0.0f, 0.0f);
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
				cameraTranslatePos -= glm::vec3(0.2f, 0.0f, 0.0f);
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
				cameraTranslatePos += glm::vec3(0.0f, 0.2f, 0.0f);
			}
			if (glfwGetKey(Window::getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
				cameraTranslatePos -= glm::vec3(0.0f, 0.2f, 0.0f);
			}

			if (state == 1) { 
				renderScreen(shader,size,tex[8]);

				glfwPollEvents();
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS) {
					state = 2;
				}
				glfwSwapBuffers(window.getWindow());
			}

			if (state == 2) {

				// Calculate delta time
				double currentTime = glfwGetTime();

				double deltaTime = currentTime - lastTime;
				lastTime = currentTime;

				// Clear background
				glClearColor(viewport_background_color_g[0],
					viewport_background_color_g[1],
					viewport_background_color_g[2], 0.0);
				glDepthMask(GL_TRUE); // allow writes to depth buffer
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// render text
				renderText(std::string("Level"), textShader, glm::vec3(0.80, 0.9, -0.2));
				renderText(std::string(std::to_string(level)), textShader, glm::vec3(0.8, 0.85, -0.2));
				renderText(std::string("Currency"), textShader, glm::vec3(0.80, 0.7, -0.2));
				renderText(std::string(std::to_string(currency)), textShader, glm::vec3(0.8, 0.65, -0.2));
				renderText(std::string("Score"), textShader, glm::vec3(0.80, 0.5, -0.2));
				renderText(std::string(std::to_string(enemyDestoried)), textShader, glm::vec3(0.8, 0.45, -0.2));
				renderText(std::string("Health"), textShader, glm::vec3(0.80, 0.30, -0.2));
				renderText(std::string(std::to_string(Health)), textShader, glm::vec3(0.8, 0.25, -0.2));
				if (selectedTower != NULL) {
					renderText(std::string("Right click to cancel select"), textShader, glm::vec3(0.2, -0.95, -0.2));
				}
				// Draw the square

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBlendEquation(GL_FUNC_ADD);


				// Select proper shader program to use
				glUseProgram(shader.getShaderID());

				glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
				shader.AttributeBinding();
				


				// remove dead enemies
				std::vector<Enemy*>::iterator it;
				for (it = Enemies.begin(); it != Enemies.end();) {
					if ((*it)->getHealth() <= 0) {
						cout << "an Enemy is destoried" << endl;
						enemyDestoried++;
						currency += (*it)->getCurrency();
						it = Enemies.erase(it);
					}
					else {
						++it;
					}
				}

				// level up
				if (enemyDestoried >= level * 20) {
					level++;
				}

				// pathfinding
				// since graph class and gameobject calss both include each other
				// to avoid making cross-reference, this part has to be accomplished in main function
				for (int i = 0; i < Enemies.size(); i++) {
					if (Enemies[i]->getReach() == true) {
						Enemies[i]->setCposX(g.getNode(g.getNodeIdFromCoords(Enemies[i]->getPosition().x, -Enemies[i]->getPosition().y)).getX());
						Enemies[i]->setCposY(g.getNode(g.getNodeIdFromCoords(Enemies[i]->getPosition().x, -Enemies[i]->getPosition().y)).getY());
						Enemies[i]->setNposX(g.getNode(Enemies[i]->getNEXT(g.getNodeIdFromCoords(Enemies[i]->getPosition().x, -Enemies[i]->getPosition().y))).getX());
						Enemies[i]->setNposY(g.getNode(Enemies[i]->getNEXT(g.getNodeIdFromCoords(Enemies[i]->getPosition().x, -Enemies[i]->getPosition().y))).getY());

						if (abs(Enemies[i]->getNposY() - Enemies[i]->getCposY()) < 0.01f && Enemies[i]->getCposX() != Enemies[i]->getNposX()) {
							if (Enemies[i]->getCposX() < Enemies[i]->getNposX()) { Enemies[i]->setVelocity(glm::vec3(0.03f, 0.0f, 0.0f)*(1 + (float)level)*Enemies[i]->getOriginalSpeed()); }
							if (Enemies[i]->getCposX() > Enemies[i]->getNposX()) { Enemies[i]->setVelocity(glm::vec3(-0.03f, 0.0f, 0.0f)*(1 + (float)level)*Enemies[i]->getOriginalSpeed()); }

						}
						if (abs(Enemies[i]->getNposX() - Enemies[i]->getCposX()) < 0.01f && Enemies[i]->getCposY() != Enemies[i]->getNposY()) {
							if (Enemies[i]->getCposY() < Enemies[i]->getNposY()) { Enemies[i]->setVelocity((glm::vec3(0.0f, 0.03f, 0.0f))*(1 + (float)level)*Enemies[i]->getOriginalSpeed()); }
							if (Enemies[i]->getCposY() > Enemies[i]->getNposY()) { Enemies[i]->setVelocity((glm::vec3(0.0f, -0.03f, 0.0f))*(1 + (float)level)*Enemies[i]->getOriginalSpeed()); }
						}

						Enemies[i]->setReach(false);

					}

					//Reach
					if ((abs(Enemies[i]->getPosition().x - Enemies[i]->getNposX()) <= abs(Enemies[i]->getVelocity().x*deltaTime) &&
						Enemies[i]->getVelocity().y == 0)||
						(abs(Enemies[i]->getPosition().y - Enemies[i]->getNposY()) <= abs(Enemies[i]->getVelocity().y*deltaTime) &&
						Enemies[i]->getVelocity().x == 0)) {
						//cout << abs(Enemies[i]->getPosition().x - Enemies[i]->getNposX()) << ","<<abs(Enemies[i]->getPosition().y - Enemies[i]->getNposY()) << endl;
						//cout << Enemies[i]->getVelocity().x << "," << Enemies[i]->getVelocity().y<< endl;
						Enemies[i]->setPosition(glm::vec3(Enemies[i]->getNposX(), Enemies[i]->getNposY(), 0.0f));
						Enemies[i]->setReach(true);

					}

					if (sqrt((Enemies[i]->getNposX() - g.getNode(g.getEndId()).getX()) * (Enemies[i]->getNposX() - g.getNode(g.getEndId()).getX()) + 
						(Enemies[i]->getNposY() - g.getNode(g.getEndId()).getY()) * (Enemies[i]->getNposY() - g.getNode(g.getEndId()).getY())) < 0.33f &&
						Enemies[i]->getHealth()>0) {
						Health -= 1;
						Enemies[i]->setHealth(-1);
					}

				}

				//searching enemies
				for (int i = 0; i < Towers.size(); i++) {
					targetInrange.clear();
					if (Towers[i]->getType() == 4) {
						for (int j = 0; j < Enemies.size(); j++) {
							float distance_3 = (Towers[i]->getOrigin().x - Enemies[j]->getPosition().x)*(Towers[i]->getOrigin().x - Enemies[j]->getPosition().x) +
								(Towers[i]->getOrigin().y - Enemies[j]->getPosition().y)*(Towers[i]->getOrigin().y - Enemies[j]->getPosition().y);
							float r = (float)Towers[i]->getRange() / 200;
							if (distance_3 <= r * r && Enemies[j]->getHealth() > 0) {
								targetInrange.push_back(Enemies[j]);
							}
						}
					}
					else {
						for (int j = 0; j < Enemies.size(); j++) {
							float distance_2 = (Towers[i]->getPosition().x - Enemies[j]->getPosition().x)*(Towers[i]->getPosition().x - Enemies[j]->getPosition().x) +
								(Towers[i]->getPosition().y - Enemies[j]->getPosition().y)*(Towers[i]->getPosition().y - Enemies[j]->getPosition().y);
							float r = (float)Towers[i]->getRange() / 200;
							if (distance_2 <= r * r && Enemies[j]->getHealth() > 0) {
								targetInrange.push_back(Enemies[j]);
							}
						}
					}
					Towers[i]->setTargetsInRange(targetInrange);

					// Updates game objects
					Towers[i]->update(deltaTime);

					// Render game objects
					Towers[i]->render(shader);
				}
				// Update and render all ENEMY objects
				for (int i = 0; i < Enemies.size(); i++) {
					// hp lower than 0
					if (Enemies[i]->getHealth() <= 0) {
						continue;
					}

					// Type 2 enmey will increase his allie's speed
					if (Enemies[i]->getType() == 2) {
						for (int j = 0; j < Enemies.size(); j++) {
							if (sqrt((Enemies[j]->getPosition().x - Enemies[j]->getPosition().x) * (Enemies[j]->getPosition().x - Enemies[j]->getPosition().x) + (Enemies[j]->getPosition().y - Enemies[j]->getPosition().y) * (Enemies[j]->getPosition().y - Enemies[j]->getPosition().y)) <= 0.88f) {
								Enemies[j]->setBuff(1.2);
							}
							else{ Enemies[j]->setBuff(1.0); }
						}
					}

					// Updates game objects
					Enemies[i]->update(deltaTime);

					// Render game objects
					Enemies[i]->render(shader);
				}

				// generate new enemies
				if (Interval == 0) {
					int i;
					float r = rand() / (float)RAND_MAX * 100;
					int gap1 = 110 - level * 10 > 25 ? 110 - level * 10 : 25;
					int gap2 = gap1 + 30;
					if (r <= gap1) {
						i = 0;
					}
					else if(r<=gap2){
						i = 1;
					}
					else {
						i = 2;
					}
					//find a new path
					g.pathfind();

					// create enemy
					Enemies.push_back(new Enemy(glm::vec3(g.getNode(g.getStartId()).getX(), g.getNode(g.getStartId()).getY(), 0.0f), enemyResource[i]->getTexture(), size));
					Enemies[Enemies.size() - 1]->setHealth(enemyResource[i]->getHealth()*(1 + (float)(level-1) / (float)10));
					Enemies[Enemies.size() - 1]->setPATH(g.getPath());
					Enemies[Enemies.size() - 1]->setOriginalSpeed(enemyResource[i]->getOriginalSpeed());
					Enemies[Enemies.size() - 1]->setCurrency(enemyResource[i]->getCurrency());
					Enemies[Enemies.size() - 1]->setLevel(level);
					cout << "generate a new enemy!\n";
				}

				Interval += 0.5+ (float)level/(float)2;

				if (Interval >= 100) {
					Interval = 0;
				}

				//render shop
				renderShop(shader, towerResource, size, numOfTower);

				// listen moouse event
				mouseEvent(towerResource, g);

				//render selectedTower
				if (selectedTower != NULL) {
					renderSelected(shader);
				}


				//update graph
				g.update();
				//render graph
				g.render(shader);

				

				//get ready to draw particles
				glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE); // draw particles without writing to depth buffer

				glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
				p_shader.AttributeBinding();
				
				// draw the particle 

				for (int i = 0; i < Towers.size(); i++) {
					if (Towers[i]->getParticleTime() > 0) {
						drawParticles(p_shader.getShaderID(), p_size, (GameObject*)Towers[i]);
					}
				}


				// Update other events like input handling
				glfwPollEvents();

				// Push buffer drawn in the background onto the display
				glfwSwapBuffers(window.getWindow());

				if (Health <= 0) { state = 3; }
				if (level > 30) { state = 4; }
			}
			
			if(state == 3){
				renderScreen(shader, size, tex[7]);
				glfwPollEvents();
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
					break;
				}
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_R) == GLFW_PRESS) {
					initialWorld(g);
					state = 2;
				}
				glfwSwapBuffers(window.getWindow());
			}

			if (state == 4) {
				renderScreen(shader, size, tex[9]);
				glfwPollEvents();
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
					break;
				}
				glfwSwapBuffers(window.getWindow());
			}
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
