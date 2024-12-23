#pragma once
#include "Enemy.h"
class Ghost : public Enemy {
public:
	Ghost(sf::Vector2f spawnPosition) : Enemy(EnemyType::Ghost) {
		health = 15;
		totalFrames = 6;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/ghost.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, const string target) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
			if (target == "player")  manager->notify("EnemyAttackPlayer", 3);
			else if (target == "guard") manager->notify("EnemyAttackGuard", 3);
			attackCooldownClock.restart();
		}
	}

};