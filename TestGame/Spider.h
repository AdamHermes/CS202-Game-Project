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
	void fighting(int direction,  std::shared_ptr<Character>&  player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			manager->notify("EnemyAttack", 3);

			attackCooldownClock.restart();
		}
	}
};