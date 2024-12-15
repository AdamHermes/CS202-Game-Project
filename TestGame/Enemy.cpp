#include "Enemy.h"
#include "Character.h"
#include <iostream>

void Enemy::changePos(int direction) {
    int frameWidth = 64;   // Width of a single frame
    int frameHeight = 64; // Height of a single frame  
    if (enemyType == EnemyType::Frogman) {
        frameWidth = 80;
        frameHeight = 96;
    }
    else if (enemyType == EnemyType::Sunflower) {
        frameWidth = 128;
        frameHeight = 128;
    }
    else if (enemyType == EnemyType::Dragon) {
        frameWidth = 144;
        frameHeight = 128;
    }
    // Update the current frame based on elapsed time
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames; // Loop through frames
        animationClock.restart();  // Restart clock for the next frame
    }
    int var = 0;
    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, var + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, var + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, var + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, var + direction * frameHeight, frameWidth, frameHeight));
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
    if (enemyType == EnemyType::Frogman) {
        sprite.setTextureRect(sf::IntRect(0, 0, 80, 96)); 
    }
    else if (enemyType == EnemyType::Sunflower) {
        sprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
    }
    else if (enemyType == EnemyType::Dragon) {
        sprite.setTextureRect(sf::IntRect(0, 0, 144, 128));
    }
    else sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // Default frame
    sprite.setOrigin(32.0f, 32.0f); // Set origin to the center (192/2)
    if (enemyType == EnemyType::Frogman) {
        sprite.setOrigin(40, 48); // Set origin to the center (192/2)

    }
    else if (enemyType == EnemyType::Sunflower) {
        sprite.setOrigin(64, 64);
    }
    else if (enemyType == EnemyType::Dragon) {
        sprite.setOrigin(72,64 );
    }
    sprite.setPosition(x, y);
}

void Enemy::drawTo(sf::RenderWindow& window) const {
    if (opacity > 0)
        window.draw(sprite);
}

int getFighitngDirection(sf::Vector2f direction) {
    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0) {
            return 2;
        }
        else {
            return 3;
        }
    }
    else {
        if (direction.y > 0) {
            return 0;
        }
        else {
            return 1;
        }
    }
}
void Enemy::handleMovement(std::shared_ptr<Map> gameMap, std::shared_ptr<Character> player) {
    sf::Vector2f enemyPosition = sprite.getPosition();
    sf::Vector2f playerPosition = player->getSprite().getPosition();

    sf::Vector2f direction = playerPosition - enemyPosition;

    // Calculate the normalized direction vector

    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 250) {
        //randomPatrol(gameMap);
        return;
    }
    if (magnitude == 0) return; // Prevent division by zero

    direction /= magnitude; // Normalize the direction vector

    // Move the enemy incrementally
    
    sf::Vector2f newPosition = enemyPosition + direction * speed;
    
    sf::FloatRect newBoundingBox(newPosition.x - 32.0f + offsetX, newPosition.y - 32.0f + offsetY, boundingBox.width, boundingBox.height);
    if (enemyType == EnemyType::Demon) {
        newBoundingBox.left += 12.0f;
        newBoundingBox.top += 20.0f;
    }
    else if (enemyType == EnemyType::Frogman) {
        newBoundingBox.left += 8.0f;
        newBoundingBox.top += 28.0f;
    }
    else if (enemyType == EnemyType::Sunflower) {
        newBoundingBox.left -= 8.0f;
        newBoundingBox.top -= 4.0f;
    }
    else if (enemyType == EnemyType::Dragon) {
        newBoundingBox.left -= 36.0f;
        newBoundingBox.top -= 4.0f;
        
    }
    bool collidesWithMap = gameMap->checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height);
    bool collidesWithPlayer = player->checkCollision(newBoundingBox);
    
    if (!collidesWithPlayer && !collidesWithMap) {

        sprite.setPosition(newPosition);
        updateBoundingBox();
        setState(EnemyState::Moving);

    }
    else if (collidesWithPlayer) {

        if (gameMap->checkCollision(player->boundingBox.left, player->boundingBox.top, player->boundingBox.width, player->boundingBox.height)) {
            sf::Vector2f overlap = enemyPosition - player->getSprite().getPosition();    
            float overlapMagnitude = std::sqrt(overlap.x * overlap.x + overlap.y * overlap.y);

            if (overlapMagnitude != 0) {
                sf::Vector2f pushback = overlap / overlapMagnitude * 0.02f; // Normalize and scale
                sprite.setPosition(enemyPosition + pushback);
                updateBoundingBox();
            }
        }
        if (enemyType == EnemyType::Golem || enemyType == EnemyType::Frogman || enemyType == EnemyType::Demon || enemyType == EnemyType::Skeleton || enemyType == EnemyType::Boarman) {
            setState(EnemyState::Fighting);

        }
        int num = getFighitngDirection(direction);
        fighting(num, player);

    }
    if (getState() == EnemyState::Moving) {
        sprite.setOrigin(32.0f, 32.0f); // Set origin to the center (192/2)
        if (enemyType == EnemyType::Frogman) {
            sprite.setOrigin(40, 48); // Set origin to the center (192/2)

        }
        else if (enemyType == EnemyType::Sunflower) {
            sprite.setOrigin(64, 64);
        }
        else if (enemyType == EnemyType::Dragon) {
            sprite.setOrigin(72, 64);
        }
        if (std::abs(direction.x) > std::abs(direction.y)) {
            if (direction.x > 0) {
                changePos(2); // Face right
            }
            else {
                changePos(3); // Face left
            }
        }
        else {
            if (direction.y > 0) {
                changePos(0); // Face down
            }
            else {
                changePos(1); // Face up
            }
        }
    }
    
}
void Enemy::updateDead() {
    if (alive) {
        if (damageFlashTimer.getElapsedTime().asSeconds() > 0.2f) {
            sprite.setColor(sf::Color::White);  // Reset to original color
        }
    }
    if (!alive && opacity > 0) {
        // Gradually decrease opacity
        opacity -= 100.0f * 0.01f;// Adjust fading speed
        if (opacity < 0) {
            opacity = 0; // Clamp to 0
        }

        // Apply new opacity to the sprite
        sf::Color color = sprite.getColor();
        color.a = static_cast<sf::Uint8>(opacity); // Cast to Uint8 for SFML
        sprite.setColor(color);
    }
    else if (!alive && opacity <= 0){
        removed = true;
    }
}
//void Enemy::randomPatrol(Map& gameMap) {
//    // Decrease the cooldown time each frame
//    if (directionCooldown > 0.0f) {
//        directionCooldown -= 0.05f;  // Adjust this value for the desired cooldown duration
//    }
//
//    sf::Vector2f enemyPosition = sprite.getPosition();
//    // Patrol parameters
//    float patrolRadius = 100.0f;  // Radius of the patrol circle
//    float patrolSpeed = 0.0001f;    // Speed at which the enemy moves along the circle  
//
//    // Update the patrol angle over time for continuous circular motion
//    patrolAngle += patrolSpeed;  // Increase the angle
//
//    if (patrolAngle >= 2 * 3.14159f) {  // Ensure the angle stays within 0 to 2 * Pi
//        patrolAngle -= 2 * 3.14159f;
//    }
//
//    // Calculate the new patrol target position using polar coordinates
//    float targetX = enemyPosition.x + patrolRadius * std::cos(patrolAngle);
//    float targetY = enemyPosition.y + patrolRadius * std::sin(patrolAngle);
//
//    // Set the patrol target position
//    sf::Vector2f patrolTarget(targetX, targetY);
//
//    // Calculate the direction to the new patrol target
//    sf::Vector2f direction = patrolTarget - enemyPosition;
//    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//
//    if (magnitude == 0) return;
//
//    // Normalize the direction
//    direction /= magnitude;
//
//    // Calculate the next position based on the direction
//    sf::Vector2f newPosition = enemyPosition + direction * 0.01f;
//
//    // Check for collisions with the map at the new position
//    sf::FloatRect newBoundingBox(newPosition.x - 32.0f + offsetX, newPosition.y - 32.0f + offsetY, boundingBox.width, boundingBox.height);
//    bool collidesWithMap = gameMap->checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height);
//
//    if (!collidesWithMap) {
//        sprite.setPosition(newPosition);
//        updateBoundingBox();
//        setState(EnemyState::Moving);
//
//        // Patrol logic (face direction)
//        if (getState() == EnemyState::Moving) {
//            sprite.setOrigin(32.0f, 32.0f);
//
//            if (std::abs(direction.x) > std::abs(direction.y)) {
//                if (direction.x > 0) {
//                    changePos(2); // Face right
//                }
//                else {
//                    changePos(3); // Face left
//                }
//            }
//            else {
//                if (direction.y > 0) {
//                    changePos(0); // Face down
//                }
//                else {
//                    changePos(1); // Face up
//                }
//            }
//        }
//    }
//    else {
//        // If the cooldown is active, don't attempt to change direction
//        if (directionCooldown > 0.0f) return;
//
//        // Handle collision by trying to adjust movement direction
//        int currentDirection = getFighitngDirection(direction);
//        switch (currentDirection) {
//        case Up:
//            currentDirection = Right; // up -> right
//            break;
//        case Right:
//            currentDirection = Down; // right -> down
//            break;
//        case Down:
//            currentDirection = Left; // down -> left
//            break;
//        case Left:
//            currentDirection = Up; // left -> up
//            break;
//        }
//
//        // New direction after adjustment
//        sf::Vector2f newDirection;
//        switch (currentDirection) {
//        case Up:
//            newDirection = sf::Vector2f(0, -1);
//            break;
//        case Right:
//            newDirection = sf::Vector2f(1, 0);
//            break;
//        case Down:
//            newDirection = sf::Vector2f(0, 1);
//            break;
//        case Left:
//            newDirection = sf::Vector2f(-1, 0);
//            break;
//        }
//
//        // Move in the adjusted direction
//        sf::Vector2f oppositePosition = enemyPosition + newDirection * 0.01f;
//
//        sf::FloatRect oppositeBoundingBox(oppositePosition.x - 32.0f + offsetX, oppositePosition.y - 32.0f + offsetY, boundingBox.width, boundingBox.height);
//        if (enemyType == EnemyType::Demon) {
//            oppositeBoundingBox.left += 12.0f;
//            oppositeBoundingBox.top += 24.0f;
//        }
//
//        bool collidesWithOppositeMap = gameMap->checkCollision(oppositeBoundingBox.left, oppositeBoundingBox.top, oppositeBoundingBox.width, oppositeBoundingBox.height);
//
//        if (!collidesWithOppositeMap) {
//            sprite.setPosition(oppositePosition);
//            updateBoundingBox();
//            setState(EnemyState::Moving);
//        }
//
//        sprite.setOrigin(32.0f, 32.0f);
//
//        if (std::abs(newDirection.x) > std::abs(newDirection.y)) {
//            if (newDirection.x > 0) {
//                changePos(2); // Face right
//            }
//            else {
//                changePos(3); // Face left
//            }
//        }
//        else {
//            if (newDirection.y > 0) {
//                changePos(0); // Face down
//            }
//            else {
//                changePos(1); // Face up
//            }
//        }
//
//        // Set the cooldown after a direction change
//        directionCooldown = 0.2f;  // Adjust the cooldown time as needed
//    }
//}




