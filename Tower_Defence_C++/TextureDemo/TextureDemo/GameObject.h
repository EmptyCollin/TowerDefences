#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

class GameObject {
public:
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements);

	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime);


	// Renders the GameObject using a shader
	virtual void render(Shader &shader,float s =1.0f);


	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getVelocity() { return velocity; }
	inline GLuint getTexture() { return texture; }
	inline GLuint getElement() { return numElements; }
	inline float getParticleTime() { return particleTime; }
	inline GLuint getParticleTex() {return particleTex; }
	inline int getType() { return type; }
	inline float getAngle() { return angle; }
	inline float getBuff() { return buff; }
	inline float getBuffDur() { return buffDuration; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setOriginalSpeed(float s) { originalSpeed = s; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity;}
	inline void setTexture(GLuint t) { texture = t; }
	inline void setParticleTime(float t) { particleTime = t; }
	inline void setParticleTex(GLuint t) { particleTex = t; }
	inline void setType(int t) { type = t; }
	inline void setAngle(float a) { angle = a; }
	inline void setBuff(float b) { buff = b; }
	inline void setBuffDuration(float bd) { buffDuration = bd; }
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	glm::vec3 velocity;

	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)

					  // Object's texture
	GLuint texture;

	float particleTime = 0.0f;
	GLuint particleTex;
	int type;
	float angle = 90;
	float originalSpeed = 1;
	float buff = 1; //Coefificant of speed changechange
	float buffDuration;
};