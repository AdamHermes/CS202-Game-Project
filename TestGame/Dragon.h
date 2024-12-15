#pragma once
#include "Enemy.h"
class Dragon : public Enemy {
public:
	Dragon(sf::Vector2f spawnPosition) : Enemy(EnemyType::Dragon) {
		health = 150;
		totalFrames = 3;
		frameDuration = 0.15f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/dragon.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction,  std::shared_ptr<Character>& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			manager->notify("EnemyAttack", 10);

			attackCooldownClock.restart();
		}
	}
};