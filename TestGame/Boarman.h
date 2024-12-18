#pragma once
#include "Enemy.h"
class Boarman : public Enemy {
public:
    Boarman(sf::Vector2f spawnPosition) : Enemy(EnemyType::Boarman) {
        health = 50;
        totalFrames = 9;
        frameDuration = 0.1f;
        attackCooldown = 0.8f;
        speed = 0.02f;
        loadTexture("../Assets/Character/Enemies/boarman.png", spawnPosition.x, spawnPosition.y);

    }
    void fighting(int direction, const string target) {
        const int frameWidth = 64;   // Width of a  std::shared_ptr<Character> player frame
        const int frameHeight = 64;  // Height of a  std::shared_ptr<Character> player f    rame
        const int totalFrames = 6;   // Number of frames per direction
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
            
            if (target == "player")  manager->notify("EnemyAttackPlayer", 7);
            else if (target == "guard") manager->notify("EnemyAttackGuard", 7);
            attackCooldownClock.restart();
        }
    }

};