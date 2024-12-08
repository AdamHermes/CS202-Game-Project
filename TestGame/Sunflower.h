#pragma once
#include "Enemy.h"
class Sunflower : public Enemy {
public:
	Sunflower(sf::Vector2f spawnPosition) : Enemy(EnemyType::Sunflower) {
		health = 70;
		totalFrames = 6;
		frameDuration = 0.2f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/sunflower.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, Character& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			manager->notify("EnemyAttack", 5);
			attackCooldownClock.restart();
		}
	}

};