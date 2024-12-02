#pragma once
#include "Enemy.h"
class Demon : public Enemy {
public:
	Demon() : Enemy(EnemyType::Demon) {
		health = 120;
		totalFrames = 4;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/demon1.png", 240, 1060);

	}
	void fighting(int direction, Character& player) {
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

			player.takeDamage(15);

			attackCooldownClock.restart();
		}
	}

};