#include "Tower.h"

#include "Window.h"

Tower::Tower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements) {
	angle = 90;

	}


void Tower::render(Shader &shader,float s) {
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, 0.0f)), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f*s, 0.4f*s, 0.6f*s));


	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	GLint color_loc = glGetUniformLocation(shader.getShaderID(), "colorMod");
	glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void Tower::update(double time) {
	if (chargeTime > 0) {
		chargeTime -= time;
	}
	if (type == 1) {
		currentTarget = targetsInRange.size() > 0 ? targetsInRange[0] : NULL;
		if (currentTarget == NULL) {
			particleTime = particleTime - time > 0 ? particleTime - time : 0;
			return;
		};
		angle = atan2(getPosition().y - currentTarget->getPosition().y, getPosition().x - currentTarget->getPosition().x) * 57 + 180;
		if (chargeTime <= 0) {
			currentTarget->setHealth(currentTarget->getHealth() - damage);
			if (currentTarget->getHealth() <= 0) {
				currentTarget->setParticleTime(1);
			}
			chargeTime = FireInterval;
			if (particleTime <= 0)	particleTime = 1.0;
		}
	}

	if (type == 2) {
		angle = 90;
		if (chargeTime <= 0) {
			if (targetsInRange.size() > 0) {
				std::cout << targetsInRange.size() << std::endl;
				for (int i = 0; i < targetsInRange.size(); i++) {
					targetsInRange[i]->setHealth(targetsInRange[i]->getHealth() - damage);
				}
				if (particleTime <= 0)	particleTime = 1.0;
			}
			chargeTime = FireInterval;
			
			
		}
	}

	if (type == 3) {
		currentTarget = targetsInRange.size() > 0 ? targetsInRange[0] : NULL;
		if (currentTarget == NULL) {
			particleTime = particleTime - time > 0 ? particleTime - time : 0;
			return;
		};
		angle = atan2(getPosition().y - currentTarget->getPosition().y, getPosition().x - currentTarget->getPosition().x) * 57 + 180;

		glm::vec3 dist = glm::normalize(glm::vec3(position.x + cos(angle / 180 * 3.1415926)*1.98f, position.y + sin(angle / 180 * 3.1415926)*1.98f, 0.0f) - position);

		glm::vec3 dd = glm::vec3(position.x + cos(angle / 180 * 3.1415926)*0.4f, position.y + sin(angle / 180 * 3.1415926)*0.4f, 0.0f);

		if (chargeTime <= 0) {
		
			for (int i = 0; i < targetsInRange.size(); i += 1) {
				glm::vec3 Para = glm::dot(targetsInRange[i]->getPosition() - glm::vec3(position.x + cos(angle / 180 * 3.1415926)*0.99f, position.y + sin(angle / 180 * 3.1415926)*0.99f, 0.0f), dist) * dist;
				glm::vec3 Perp = targetsInRange[i]->getPosition() - glm::vec3(position.x + cos(angle / 180 * 3.1415926)*0.99f, position.y + sin(angle / 180 * 3.1415926)*0.99f, 0.0f) - Para;
				if (sqrt(Perp.x * Perp.x + Perp.y * Perp.y) < 0.46f) {
					targetsInRange[i]->setHealth(targetsInRange[i]->getHealth() - damage);
				}
			}		
			currentTarget->setHealth(currentTarget->getHealth() - damage);
			
			chargeTime = FireInterval;
			if (particleTime <= 0)	particleTime = 1.0;
		}

	}

	if (type == 4) {
		position.x += velocity.x;
		position.y += velocity.y;

		currentTarget = targetsInRange.size() > 0 ? targetsInRange[0] : NULL;

		// target then back to orgin
		if (currentTarget == NULL) {
			particleTime = particleTime - time > 0 ? particleTime - time : 0;

			if (waittime <= 0) {
				angle = atan2(position.y - Origin.y, position.x - Origin.x) * 57 + 180;
				float distance = sqrt((position.x - Origin.x) * (position.x - Origin.x) + (position.y - Origin.y) * (position.y - Origin.y));

				if (distance >= 0.06f) {
					velocity.x = 0.1f * cos(angle / 180 * 3.1415926);
					velocity.y = 0.1f * sin(angle / 180 * 3.1415926);
				}
				else {
					velocity.x = 0;
					velocity.y = 0;
				}
			}

			waittime -= 1;
		}

		//chase target
		else {
			angle = atan2(position.y - currentTarget->getPosition().y, position.x - currentTarget->getPosition().x) * 57 + 180;

			float distance = sqrt(((position.x - currentTarget->getPosition().x) * (position.x - currentTarget->getPosition().x)) + ((position.y - currentTarget->getPosition().y) * (position.y - currentTarget->getPosition().y)));

			if (distance <= 1.0f && chargeTime <= 0) {
				velocity.x = 0;
				velocity.y = 0;
				currentTarget->setHealth(currentTarget->getHealth() - damage);
				if (currentTarget->getHealth() <= 0) {
					currentTarget->setParticleTime(1);
				}
				chargeTime = FireInterval;
				if (particleTime <= 0)	particleTime = 1.0;
			}

			else if (distance >= 1.0f) {
				velocity.x = 0.12f * cos(angle / 180 * 3.1415926);
				velocity.y = 0.12f * sin(angle / 180 * 3.1415926);
			}


			waittime = 36;
		}
	}

	particleTime = particleTime - time > 0 ? particleTime - time : 0;

}

/*
if (currentTarget->getHealth() <= 0) {
	currentTarget->setParticleTime(1);
}
*/