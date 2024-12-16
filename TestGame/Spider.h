#pragma once
#include "Enemy.h"
class Spider : public Enemy {
public:
	Spider(sf::Vector2f spawnPosition) : Enemy(EnemyType::Spider) {
		health = 25;
		totalFrames = 10;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/spider01.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction,  const string target) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
			if (target == "player")  manager->notify("EnemyAttackPlayer", 3);
			else if (target == "guard") manager->notify("EnemyAttackGuard", 3);
			attackCooldownClock.restart();
		}
	}
};