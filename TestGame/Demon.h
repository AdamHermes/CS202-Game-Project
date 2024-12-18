#pragma once
#include "Enemy.h"
class Demon : public Enemy {
public:
	Demon(sf::Vector2f spawnPosition) : Enemy(EnemyType::Demon) {
		health = 65;
		totalFrames = 4;
		frameDuration = 0.1f;
		attackCooldown = 0.8f;
		loadTexture("../Assets/Character/Enemies/demon2.png", spawnPosition.x, spawnPosition.y);

	}
	void fighting(int direction, const string target) {
        const int frameWidth = 64;   // Width of a single frame
        const int frameHeight = 64;  // Height of a single frame
        const int totalFrames = 4;   // Number of frames per direction
        const int frameHeight2 = 256;
        sprite.setOrigin(32.0f, 32.0f);
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
            if (target == "player")  manager->notify("EnemyAttackPlayer", 9);
            else if (target == "guard") manager->notify("EnemyAttackGuard", 9);
            attackCooldownClock.restart();
		}
	}

};