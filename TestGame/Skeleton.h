#pragma once
#include "Enemy.h"
class Skeleton : public Enemy {
public:
    Skeleton(sf::Vector2f spawnPosition) : Enemy(EnemyType::Skeleton) {
        health = 60;
        totalFrames = 9;
        frameDuration = 0.2f;
        attackCooldown = 0.8f;
        loadTexture("../Assets/Character/Enemies/skeleton.png", spawnPosition.x, spawnPosition.y);

    }
    void fighting(int direction,  std::shared_ptr<Character>& player) {
        const int frameWidth = 64;   // Width of a single frame
        const int frameHeight = 64;  // Height of a single frame
        const int totalFrames = 5;   // Number of frames per direction
        const int frameHeight2 = 256;
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

            manager->notify("EnemyAttack", 7);
            attackCooldownClock.restart();
        }
    }

};