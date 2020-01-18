#pragma once

#include "GameObject.h"
#include "Node.h"

// Inherits from GameObject
class Enemy : public GameObject {
public:
	Enemy(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;

	inline int getHealth() { return health; }
//	inline int getspeed() { return health; }
	inline int getdamage() { return damage; }
	inline bool getReach() { return Reach; }
	inline int getCurrency() { return currency; }

	inline float getCposX() { return CposX; }
	inline float getCposY() { return CposY; }
	inline float getNposX() { return NposX; }
	inline float getNposY() { return NposY; }
	inline float getOriginalSpeed() { return originalSpeed; }

	inline int getType() { return type; }

	int getNEXT(int i);

	inline void setHealth(int h) { health = h; }
//	inline void setSpeed(int s) { speed = s; }
	inline void setdamage(int d) { damage = d; }
	inline void setLevel(int l) { level = l; }

	inline void setReach(bool TF) { Reach = TF; }

	inline void setCposX(float C) { CposX = C; }
	inline void setCposY(float CC) { CposY = CC; }
	inline void setNposX(float N) { NposX = N; }
	inline void setNposY(float NN) { NposY = NN; }
	inline void setCurrency(int c) { currency = c; }
	inline void setType(int t) { type = t; }
	inline void setBuff(float b) { buffed = b; }

	inline void setPATH(int* p) {
		for (int i = 0; i < 128; i += 1) {
			PATHH[i] = p[i];
		}
	}

private:
	int health;
	int damage; //Damage to core

	int level = 1.0;
	bool Reach = true;

	//handle buff
	float buffed=1;

	float CposX;
	float CposY;

	float NposX;
	float NposY;

	int currency = 0;
	int type;

	//here
	int PATHH[128];
};