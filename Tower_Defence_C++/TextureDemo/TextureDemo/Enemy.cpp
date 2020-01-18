#include "Enemy.h"

#include "Window.h"

Enemy::Enemy(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements) {
	type = -1;
}

// Update function for moving the player object around
void Enemy::update(double deltaTime) {

	//handle buff
	position += velocity * (float)deltaTime * buff;
	particleTime = particleTime - deltaTime > 0 ? particleTime - deltaTime : 0;
}

int Enemy::getNEXT(int CU) {
	for (int i = 127; i > 0; i -= 1) {
		if (PATHH[i] == CU) {
			return PATHH[i + 1];
		}
	}
}