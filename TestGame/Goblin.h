#pragma once
#include "Enemy.h"
class Goblin : public Enemy {
public:
	Goblin(sf::Vector2f spawnPosition) : Enemy(EnemyType::Goblin) {
		health = 55;
		totalFrames = 11;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/goblin.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction,  const string target) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
			if (target == "player")  manager->notify("EnemyAttackPlayer", 7);
			else if (target == "guard") manager->notify("EnemyAttackGuard", 7);
			attackCooldownClock.restart();
		}
	}
	
};