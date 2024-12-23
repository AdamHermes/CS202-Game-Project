#pragma once
#include "Enemy.h"
class Worm : public Enemy {
public:
	Worm(sf::Vector2f spawnPosition) : Enemy(EnemyType::Worm) {
		health = 25;
		totalFrames = 6;
		frameDuration = 0.05f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/worm.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, const string target) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
			if (target == "player")  manager->notify("EnemyAttackPlayer", 5);
			else if (target == "guard") manager->notify("EnemyAttackGuard", 5);
			attackCooldownClock.restart();
		}
	}

};