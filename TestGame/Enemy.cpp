#include "Enemy.h"
#include <iostream>

void Enemy::changePos(int direction) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64; // Height of a single frame
    const int totalFrames = 4;  // Total number of frames per animation

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

void Enemy::handleMovement(Map& gameMap, int& num, bool& isMoving) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0.03f, 0);
        sf::FloatRect newBoundingBox(newPosition.x, newPosition.y, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 3;
        changePos(num); // Face right
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(-0.03f, 0);
        sf::FloatRect newBoundingBox(newPosition.x, newPosition.y, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 4;
        changePos(num); // Face left
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0, -0.03f);
        sf::FloatRect newBoundingBox(newPosition.x, newPosition.y, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 2;
        changePos(num); // Face up
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0, 0.03f);
        sf::FloatRect newBoundingBox(newPosition.x, newPosition.y, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 1;
        changePos(num); // Face down
        isMoving = true;
    }
}
