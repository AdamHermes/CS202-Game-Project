#pragma once
#include "Enemy.h"
class Goblin : public Enemy {
public:
	Goblin() : Enemy(EnemyType::Goblin) {
		health = 70;
		totalFrames = 11;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/goblin.png", 440, 1100);

	}
	void fighting(int direction, Character& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			player.takeDamage(5);

			attackCooldownClock.restart();
		}
	}
	
};