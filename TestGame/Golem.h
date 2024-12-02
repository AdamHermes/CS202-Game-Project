#pragma once
#include "Enemy.h"
class Golem : public Enemy {
public:
	Golem() : Enemy(EnemyType::Golem) {
		health = 100;
		totalFrames = 7;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/golem.png",240,1060);

	}
	void fighting(int direction, Character& player);
};