#pragma once
#include "Enemy.h"
class Frogman: public Enemy {
public:
	Frogman(sf::Vector2f spawnPosition) : Enemy(EnemyType::Frogman) {
        health = 35;
		totalFrames = 6;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
        speed = 0.03f;
		loadTexture("../Assets/Character/Enemies/frogman.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, const string target) {
        const int frameWidth = 80;   // Width of a single frame
        const int frameHeight = 96;  // Height of a single frame
        const int totalFrames = 4;   // Number of frames per direction
        const int frameHeight2 = 384;
        if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
            currentFrame = (currentFrame + 1) % totalFrames;  // Loop attack frames
            animationClock.restart();
        }

        if (direction == Right) {
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
        }
        else if (direction == Left) {
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
        }
        else if (direction == Up) {
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
        }
        else if (direction == Down) {
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
        }
		if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
            if (target == "player")  manager->notify("EnemyAttackPlayer", 5);
            else if (target == "guard") manager->notify("EnemyAttackGuard", 5);
            attackCooldownClock.restart();
		}
	}

};