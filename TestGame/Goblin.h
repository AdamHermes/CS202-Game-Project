#pragma once
#include "Enemy.h"
class Goblin : public Enemy {
public:
	Goblin(sf::Vector2f spawnPosition) : Enemy(EnemyType::Goblin) {
		health = 70;
		totalFrames = 11;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/goblin.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, Character& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			manager->notify("EnemyAttack", 5);

			attackCooldownClock.restart();
		}
	}
	
};