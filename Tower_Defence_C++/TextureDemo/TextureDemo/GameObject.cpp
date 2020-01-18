#include "GameObject.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	// Update object position
	position += velocity * (float)deltaTime*buff;
	particleTime = particleTime - deltaTime > 0 ? particleTime - deltaTime : 0;
}



// Renders the GameObject using a shader
void GameObject::render(Shader &shader,float s) {
	
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, 0.0f)), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.22f*s,0.3f*s,0.6f*s));


	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
	glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}
