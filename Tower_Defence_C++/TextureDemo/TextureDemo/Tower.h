#pragma once

#include "GameObject.h"
#include "Enemy.h"
using namespace std;

// Inherits from GameObject
class Tower : public GameObject {
public:
	Tower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	inline int getDamage() { return damage; }
	inline float getRange() { return Range; }
	inline int getCost() { return cost; }
	inline int getLevel() { return level; }
	inline float getFireInterval() { return FireInterval; }
	inline bool getMove() { return Moveable; }
	inline GLuint getTexture() { return texture; }
	inline std::vector<Enemy*> getTargetsInRange(){ return targetsInRange; }
	inline GLuint getElement() { return numElements; }
	inline glm::vec3 getOrigin() { return Origin; }

	inline void setDamage(int d) { damage = d; }
	inline void setRange(float r) { Range = r; }
	inline void setCost(int c) { cost = c; }
	inline void setLevel(int l) { level = l; }
	inline void setMove(bool m) { Moveable = m; }
	inline void setFireInterval(float fi) { FireInterval = fi; }
	inline void setTargetsInRange(std::vector<Enemy*> e) { targetsInRange = e; }
	inline void setOrigin(glm::vec3 o) { Origin = o; }

	inline void render(Shader &shader,float s = 1.0f);



private:
	int damage;
	float Range;
	int cost;
	int level;
	float FireInterval;
	std::vector<Enemy*> targetsInRange;
	Enemy* currentTarget = NULL;
	bool Moveable;
	float chargeTime = 0;
	int waittime = 36;

	glm::vec3 Origin = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);

	int targetPos = -1;
};