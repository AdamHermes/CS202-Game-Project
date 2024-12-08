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
void Character::fightSword(int direction, const std::vector<std::shared_ptr<Enemy>>& enemies) {
    const int frameWidth = 192;   // Width of a single frame
    const int frameHeight = 192;  // Height of a single frame
    const int totalFrames = 6;   // Number of frames per direction
    attackRangeBox = boundingBox;
    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  
        animationClock.restart();  
    }   

    if (direction == Right) {
        attackRangeBox.left += boundingBox.width; 
        attackRangeBox.width += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 3 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        attackRangeBox.left -= 30.0f;
        attackRangeBox.width += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        attackRangeBox.top -= 30.0f; 
        attackRangeBox.height += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        attackRangeBox.top += boundingBox.height; 
        attackRangeBox.height += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight));
    }
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
        // Loop through all enemies and check if the attack intersects with any of them
        //for (const auto& enemy : enemies) {
        //    if (enemy && attackRangeBox.intersects(enemy->boundingBox)) {
        //        // If the attack intersects, the enemy takes damage
        //        enemy->takeDamage(equippedWeapon->getDamage());
        //        std::cout << "Attack hit the enemy!" << std::endl;
        //    }

        if (manager) {
            manager->notify("PlayerAttack", equippedWeapon->getDamage());
            std::cout << "Attack launched" << std::endl;

        }
        // Restart the cooldown timer after the attack
        attackCooldownClock.restart();
    }
    
}
void Character::loadTexture(const std::string& path, bool isBig, int num, float x, float y) {
    if (texture.loadFromFile(path)) {
        std::cout << "Load Success: " << path << std::endl;
        sprite.setTexture(texture);

        // Adjust texture rect based on size
        if (isBig) {
            sprite.setTextureRect(sf::IntRect(0, 0, 192, 192)); // Fighting sprite
            sprite.setOrigin(96.0f, 96.0f); // Set origin to the center (192/2)
        }
        else {
            sprite.setTextureRect(sf::IntRect(num*64, num* 64, 64, 64)); // Moving sprite
            sprite.setOrigin(32.0f, 32.0f); // Center of 64x64

        }

        sprite.setPosition(x, y);
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
}

void Character::updateState(bool fighting, int num) {
    if (fighting != isFighting) {
        isFighting = fighting;
        if (isFighting) {
            loadTexture("../Assets/Character/Textures/slash.png", true, num, sprite.getPosition().x, sprite.getPosition().y);
        }
        else {
            loadTexture("../Assets/Character/Textures/characters.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
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
void Character::handleMovement(Map& gameMap, const std::vector<std::shared_ptr<Enemy>>& enemies, int& num, bool& isMoving) {
    sf::Vector2f movement(0.f, 0.f);
    bool isDiagonal = false;

    // Check for diagonal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(0.05f, -0.05f);
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-0.05f, -0.05f);
        num = 1; // Face left
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(0.05f, 0.05f);
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-0.05f, 0.05f);
        num = 1; // Face left
        isDiagonal = true;
    }
    if (movement.x != 0.f && movement.y != 0.f) {
        movement /= std::sqrt(2.f); // Scale down diagonal movement
    }
    // Check for single key movement if no diagonal keys are pressed
    if (!isDiagonal) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement = sf::Vector2f(0.05f, 0);
            num = 3; // Face right
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement = sf::Vector2f(-0.05f, 0);
            num = 1; // Face left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement = sf::Vector2f(0, -0.05f);
            num = 0; // Face up
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement = sf::Vector2f(0, 0.05f);
            num = 2; // Face down
        }
    }

    // Calculate new position
    if (movement != sf::Vector2f(0.f, 0.f)) {
        sf::Vector2f position = sprite.getPosition();
        sf::Vector2f newPosition = position + movement;
        sf::FloatRect newBoundingBox(newPosition.x - 32.0f + offsetX, newPosition.y -32.0f + offsetY, boundingBox.width, boundingBox.height);
        bool collideEnemy = false;
        // Check collisions
        for (const auto& enemy : enemies) {
            if (enemy && enemy->checkCollision(newBoundingBox)) {
                collideEnemy = true; // Early exit if a collision with an enemy is detected
            }
        }
        if (!gameMap.checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
            (!collideEnemy)) {
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
