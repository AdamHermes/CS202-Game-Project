#include <iostream>
#include "Character.h"
#include "Enemy.h"
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
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, direction * frameHeight, frameWidth, frameHeight));
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
void Character::fightSword(int direction, Enemy& enemy) {
    const int frameWidth = 192;   // Width of a single frame
    const int frameHeight = 192;  // Height of a single frame
    const int totalFrames = 6;   // Number of frames per direction
    sf::FloatRect attackRangeBox = boundingBox;
    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  
        animationClock.restart();  
    }
    

    if (direction == Right) {
        attackRangeBox.width += 20.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 3 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        attackRangeBox.left -= 20.0f;
        attackRangeBox.width += 20.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        attackRangeBox.top -= 20.0f;
        attackRangeBox.height += 20.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        attackRangeBox.height += 20.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight));
    }
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {

        if (attackRangeBox.intersects(enemy.boundingBox)) {
            enemy.takeDamage(equippedWeapon->getDamage());
            std::cout << "Attack hit the enemy!" << std::endl;
        }
        else {
            std::cout << "Attack missed!" << std::endl;
        }

        attackCooldownClock.restart(); // Restart cooldown timer
    }
    
}
void Character::loadTexture(const std::string& path, bool isBig, int num, float x, float y) {
    if (texture.loadFromFile(path)) {
        std::cout << "Load Success: " << path << std::endl;
        sprite.setTexture(texture);

        // Adjust texture rect based on size
        if (isBig) {
            sprite.setTextureRect(sf::IntRect(0, 0, 192, 192)); // Fighting sprite
        }
        else {
            sprite.setTextureRect(sf::IntRect(num*64, num* 64, 64, 64)); // Moving sprite
        }

        sprite.setPosition(x, y);
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
}
void Character::adjustPositionForNewSize(int oldWidth, int oldHeight, int newWidth, int newHeight) {
    sf::Vector2f currentPosition = sprite.getPosition();
    float dx = (newWidth - oldWidth) / 2.0f;
    float dy = (newHeight - oldHeight) / 2.0f;
    sprite.setPosition(currentPosition.x + dx, currentPosition.y + dy);
}

void Character::updateState(bool fighting, int num) {
    if (fighting != isFighting) {
        isFighting = fighting;
        if (isFighting) {
            loadTexture("../Assets/Character/Textures/slash.png", true, num, sprite.getPosition().x-64.0f, sprite.getPosition().y-64.0f);
        }
        else {
            loadTexture("../Assets/Character/Textures/characters.png", false, num, sprite.getPosition().x+64.0f, sprite.getPosition().y+64.0f);
        } 
    }
}

void Character::drawTo(sf::RenderWindow& window) const {
    if (!isHiden)
        window.draw(sprite);
}
bool Character::checkCollision(const sf::FloatRect& otherBox) const {
    return boundingBox.intersects(otherBox);
}
void Character::handleMovement(Map& gameMap, Enemy& enemy, int& num, bool& isMoving) {
    sf::Vector2f movement(0.f, 0.f);
    bool isDiagonal = false;

    // Check for diagonal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(0.03f, -0.03f);
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-0.03f, -0.03f);
        num = 1; // Face left
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(0.03f, 0.03f);
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-0.03f, 0.03f);
        num = 1; // Face left
        isDiagonal = true;
    }
    if (movement.x != 0.f && movement.y != 0.f) {
        movement /= std::sqrt(2.f); // Scale down diagonal movement
    }
    // Check for single key movement if no diagonal keys are pressed
    if (!isDiagonal) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement = sf::Vector2f(0.03f, 0);
            num = 3; // Face right
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement = sf::Vector2f(-0.03f, 0);
            num = 1; // Face left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement = sf::Vector2f(0, -0.03f);
            num = 0; // Face up
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement = sf::Vector2f(0, 0.03f);
            num = 2; // Face down
        }
    }

    // Calculate new position
    if (movement != sf::Vector2f(0.f, 0.f)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + movement;
        sf::FloatRect newBoundingBox(newPosition.x + offsetX, newPosition.y + offsetY, boundingBox.width, boundingBox.height);

        // Check collisions
        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            !enemy.checkCollision(newBoundingBox)) {
            sprite.setPosition(newPosition);
            updateBoundingBox();
            isMoving = true;
        }
        changePos(num);
    }
    else {
        isMoving = false;
    }

    // Update animation based on direction

    isFighting = false;
}

void Character::equipWeapon(WeaponType type){    
    if (equippedWeapon) {
        delete equippedWeapon; // Clean up old weapon if it exists
    }
    equippedWeapon = new Weapon(type);
    
}
