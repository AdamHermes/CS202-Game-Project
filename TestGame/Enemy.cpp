#include "Enemy.h"
#include "Character.h"
#include <iostream>

void Enemy::changePos(int direction) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64; // Height of a single frame  

    // Update the current frame based on elapsed time
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames; // Loop through frames
        animationClock.restart();  // Restart clock for the next frame
    }
    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 3 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
}

void Enemy::loadTexture(std::string filename, float x, float y) {
    if (texture.loadFromFile(filename)) {
        std::cout << "Load Success\n";
    }
    else {
        std::cout << "Load Failed\n";
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // Default frame
    sprite.setPosition(x, y);
}

void Enemy::drawTo(sf::RenderWindow& window) const {
    if (!isHiden)
        window.draw(sprite);
}

void Enemy::handleMovement(Map& gameMap, Character& player) {
    sf::Vector2f enemyPosition = sprite.getPosition();
    sf::Vector2f playerPosition = player.getSprite().getPosition(); 
    sf::Vector2f direction = playerPosition - enemyPosition;

    // Calculate the normalized direction vector
    
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 200) return;
    if (magnitude == 0) return; // Prevent division by zero

    direction /= magnitude; // Normalize the direction vector

    // Move the enemy incrementally
    sf::Vector2f newPosition = enemyPosition + direction * 0.01f;
    sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

    bool collidesWithMap = gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height);
    bool collidesWithPlayer = player.checkCollision(newBoundingBox);

    if (!collidesWithMap && !collidesWithPlayer) {
        sprite.setPosition(newPosition);
        updateBoundingBox();
    }
    else if (collidesWithPlayer) {
        sf::Vector2f pushback = -direction * 0.02f;
        sprite.setPosition(enemyPosition + pushback);
        updateBoundingBox();
    }

    // Change the monster's facing direction based on movement
    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0) {
            changePos(3); // Face right
        }
        else {
            changePos(4); // Face left
        }
    }
    else {
        if (direction.y > 0) {
            changePos(1); // Face down
        }
        else {
            changePos(2); // Face up
        }
    }
}

