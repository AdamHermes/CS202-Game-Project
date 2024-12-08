#pragma once
#include "Enemy.h"
class Golem : public Enemy {
public:
	Golem(sf::Vector2f spawnPosition) : Enemy(EnemyType::Golem) {
		health = 80;
		totalFrames = 7;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/golem.png",spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, Character& player);
};