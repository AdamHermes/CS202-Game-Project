#pragma once
#include "Enemy.h"
class Sunflower : public Enemy {
public:
	Sunflower(sf::Vector2f spawnPosition) : Enemy(EnemyType::Sunflower) {
		health = 55;
		totalFrames = 6;
		frameDuration = 0.2f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/sunflower.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, const string target) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
			if (target == "player")  manager->notify("EnemyAttackPlayer", 8);
			else if (target == "guard") manager->notify("EnemyAttackGuard", 8);
			attackCooldownClock.restart();
		}
	}

};