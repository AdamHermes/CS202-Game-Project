#include <iostream>
#include "Character.h"

void Character::changePos(int direction) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64;  // Height of a single frame
    const int totalFrames = 6;   // Number of frames per direction

    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  // Loop the frames
        animationClock.restart();  // Reset the clock for the next frame
    }

    // Set the texture rectangle based on the direction and current frame
    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 3 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight));
    }
}
void Character::fight(int direction) {
    frameDuration = 0.1f;
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64;  // Height of a single frame
    const int totalFrames = 6;   // Number of frames per direction

    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  // Loop the frames
        animationClock.restart();  // Reset the clock for the next frame
    }
    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 7 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 5 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 4 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 6 * frameHeight, frameWidth, frameHeight));
    }
}
void Character::fightBow(int direction) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64;  // Height of a single frame
    const int totalFrames = 12;   // Number of frames per direction

    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  // Loop the frames
        animationClock.restart();  // Reset the clock for the next frame
    }
    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 11 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 9 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 8 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 10 * frameHeight, frameWidth, frameHeight));
    }
}
void Character::loadTexture(std::string filename, float x, float y) {
    if (texture.loadFromFile(filename)) cout << "Load Success";
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    //sprite.scale(sf::Vector2f(2, 2));
    sprite.setPosition(x, y);
}
void Character::drawTo(sf::RenderWindow& window) const {
    if (!isHiden)
        window.draw(sprite);
}
void Character::handleMovement(Map& gameMap, Enemy& enemy, int& num, bool& isMoving) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0.03f, 0);
        sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            !enemy.checkCollision(newBoundingBox)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 3;
        changePos(num); // Face right
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(-0.03f, 0);
        sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            !enemy.checkCollision(newBoundingBox)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 4;
        changePos(num); // Face left
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0, -0.03f);
        sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            !enemy.checkCollision(newBoundingBox)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 2;
        changePos(num); // Face up
        isMoving = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + sf::Vector2f(0, 0.03f);
        sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            !enemy.checkCollision(newBoundingBox)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
        }
        num = 1;
        changePos(num); // Face down
        isMoving = true;
    }
}
