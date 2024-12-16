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
void Character::fightSpear(int direction, const std::vector<std::shared_ptr<Enemy>>& enemies) {
    frameDuration = 0.1f;
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64;  // Height of a single frame
    const int totalFrames = 9;   // Number of frames per direction
    attackRangeBox = boundingBox;
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  // Loop the frames
        animationClock.restart();  // Reset the clock for the next frame
    }
    // Update the frame based on the time elapsed
    if (direction == Right) {
        attackRangeBox.top += 20.0f;// Set position relative to character's position
        attackRangeBox.width += 30.0f;  // Long range for the spear
        attackRangeBox.height = 16.0f; // Narrow height
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 7 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        attackRangeBox.top += 20.0f;
        attackRangeBox.left -= 30.0f;// Extend spear attack to the left
        attackRangeBox.width = 30.0f;
        attackRangeBox.height = 16.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 5 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        attackRangeBox.top -= 30.0f;
        attackRangeBox.height += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 4 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        attackRangeBox.height += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 6 * frameHeight, frameWidth, frameHeight));
    }
    
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
        

        if (manager) {
            manager->notify("PlayerAttack", curWeapon->getDamage());
            std::cout << "Attack launched" << std::endl;

        }
        attackCooldownClock.restart();
    }
}
void Character::fightBow(int direction, CharacterType type, const std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Map>& gameMap) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64; // Height of a single frame
    const int totalFrames = 13; // Total frames for bow animation
    const float arrowSpeed = 300.0f; // Arrow speed
    const float maxArrowDistance = 416.0f; // Maximum arrow travel distance
    // Update animation frame
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;
        animationClock.restart();
    }

    // Set sprite texture based on direction and animation frame
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

    // Launch arrow when frame is 10
    if (isFighting && currentFrame == 10 && attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
        sf::Sprite newArrow = arrowSprite; // Create a new arrow sprite
        newArrow.setPosition(sprite.getPosition()); // Set initial position of arrow
        if (direction == Left) {
            newArrow.setPosition(sprite.getPosition() + sf::Vector2f(-64,0));
        }
        else if (direction == Up) {
            newArrow.setPosition(sprite.getPosition() + sf::Vector2f(0, -64));
        }
        // Determine arrow texture rect and velocity based on direction
        sf::Vector2f velocity;
        if (direction == Right) {
            velocity = { arrowSpeed, 0 };
            newArrow.setTextureRect(sf::IntRect(0, frameHeight * 2, frameWidth, frameHeight)); // Right-facing arrow
        }
        else if (direction == Left) {
            velocity = { -arrowSpeed, 0 };
            newArrow.setTextureRect(sf::IntRect(0, frameHeight * 3, frameWidth, frameHeight)); // Left-facing arrow
        }
        else if (direction == Up) {
            velocity = { 0, -arrowSpeed };
            newArrow.setTextureRect(sf::IntRect(0, frameHeight * 1, frameWidth, frameHeight)); // Up-facing arrow
        }
        else if (direction == Down) {
            velocity = { 0, arrowSpeed };
            newArrow.setTextureRect(sf::IntRect(0, frameHeight * 0, frameWidth, frameHeight)); // Down-facing arrow
        }

        // Add arrow to vectorArrow
        vectorArrow.push_back({ newArrow, velocity, 0.0f }); // Distance starts at 0
        attackCooldownClock.restart(); // Reset attack cooldown
    }

    // Update all arrows in flight
    for (auto it = vectorArrow.begin(); it != vectorArrow.end();) {
        sf::Sprite& arrow = std::get<0>(*it);       // Arrow sprite
        sf::Vector2f& velocity = std::get<1>(*it); // Arrow velocity
        float& distanceTraveled = std::get<2>(*it); // Distance traveled

        // Move arrow and update distance
        arrow.move(velocity * 0.0006f);
        distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * 0.0006f;
        if (direction == Right || direction == Left) {
            attackRangeBox = sf::FloatRect(arrow.getPosition().x, arrow.getPosition().y, 64.0f, 16.0f); // Horizontal arrow
        }
        else if (direction == Up || direction == Down) {
            attackRangeBox = sf::FloatRect(arrow.getPosition().x, arrow.getPosition().y, 16.0f, 64.0f); // Vertical arrow
        }
        // Check for collisions with enemies
        hit = false;
        if (gameMap->checkCollision(attackRangeBox.left, attackRangeBox.top, attackRangeBox.width, attackRangeBox.height)) {
            hit = true;
        }
        if (!hit){

            if (manager) {
                if (type == CharacterType::player) {
                    manager->notify("PlayerAttack", curWeapon->getDamage());

                }
                else if (type == CharacterType::guard) {
                    manager->notify("GuardAttack", curWeapon->getDamage());
                }
            }
        }

        // Remove arrow if it hits an enemy or exceeds range
        if (hit || distanceTraveled >= maxArrowDistance) {
            it = vectorArrow.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Character::setShooting(bool shooting) {
    isShooting = shooting;
    if (!isShooting) {
        vectorArrow.clear(); // Clear all arrows when spacebar is released
    }
}

void Character::applyItemEffect(std::shared_ptr<Items> item) {
    switch (item->getType()) {
    case ItemType::health:
        if (healingTimer.getElapsedTime().asSeconds() >= 0.5f) {
            health += 10;
            if (health > 100) health = 100;
            sprite.setColor(sf::Color(0, 255, 0, 128));  // Green glow effect

            // Restart the healing timer after each healing step
            healingTimer.restart();
        }
        break;
    case ItemType::speed:
        speed += 0.025f;
        break;
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
        attackRangeBox.left -= (30.0f+boundingBox.width);
        attackRangeBox.width += 30.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        attackRangeBox.left -= 24.0f;
        attackRangeBox.top -= 18.0f; 
        attackRangeBox.height = 18.0f;
        attackRangeBox.width += 48.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        attackRangeBox.left -= 24.0f;
        attackRangeBox.top += boundingBox.height; 
        attackRangeBox.height = 18.0f;
        attackRangeBox.width += 48.0f;
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
            manager->notify("PlayerAttack", curWeapon->getDamage());
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
    if (!arrowTexture.loadFromFile("../Assets/Character/Textures/arrow.png")) {
        throw std::runtime_error("Failed to load arrow texture from assets/arrow.png");
    }
    arrowSprite.setTexture(arrowTexture); // Assign the texture to the sprite
}

void Character::updateState(bool fighting, int num, WeaponType weaponType) {
    if (fighting != isFighting || (curWeapon->getType() != weaponType && weaponType != WeaponType::None)) {
        isFighting = fighting;
        if (isFighting) {
            if (weaponType == WeaponType::Sword) {
                loadTexture("../Assets/Character/Textures/slash.png", true, num, sprite.getPosition().x, sprite.getPosition().y);
            }
            else {
                loadTexture("../Assets/Character/Textures/character1.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
            }
        }
        else {
            loadTexture("../Assets/Character/Textures/character1.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
        }
    }
}
std::shared_ptr<Items> Character::checkItemNearby(std::vector<shared_ptr<Items>>& items_inventory) {
    for (std::shared_ptr<Items> item : items_inventory) {
        if (boundingBox.intersects(item->getSprite().getGlobalBounds())) {
            item->getSprite().setColor(sf::Color(255, 255, 0, 255)); // Yellow glow
            return item;
            
        }
        else {
            item->getSprite().setColor(sf::Color(255, 255, 255, 255)); // Normal color
            
        }
    }
    return nullptr;
}
void Character::takePortions(std::shared_ptr<Items>& item) {
    for (int i = 0; i < 3; ++i) {
        if (!storedItems[i]) {
            storedItems[i] = item;
            item->removed();
            return;
        }
    }
}
void Character::updateItemPositions(const sf::View& cameraView) {
    sf::Vector2f viewCenter = cameraView.getCenter();
    sf::Vector2f viewSize = cameraView.getSize();

    // Bottom-center of the screen
    float baseX = viewCenter.x + (viewSize.x / 2) - 160.0f; // Starting X position (left edge + margin)
    float baseY = viewCenter.y - (viewSize.y / 2) + 30.0f; // Bottom edge - margin for height

    float itemSpacing = 50.0f; 
    for (int i = 0; i < 3; ++i) {
        if (storedItems[i]) {
            float x = baseX + (itemSpacing * i); 
            float y = baseY;
            storedItems[i]->setPosition(x, y);
        }
    }
}

void Character::updateSpriteHealth(const Camera& camera) {
    if (type == CharacterType::player) {
        healthBar.update(health);
        healthBar.updatePosition(camera.getView());
        healthBar.stopShake();
    }
    if (damageFlashTimer.getElapsedTime().asSeconds() > 0.2f && healingTimer.getElapsedTime().asSeconds() > 0.2f) {
        sprite.setColor(sf::Color::White);  // Reset to the original sprite color if no healing is applied
    }
}
void Character::drawTo(sf::RenderWindow& window) const {
    if (alive) {
        window.draw(sprite);
    }
    if (type == CharacterType::player) {
        window.draw(healthBar.getSprite());
        for (int i = 0; i < 3; ++i) {
            if (storedItems[i]) {
                window.draw(storedItems[i]->getSprite());
            }
        }
    }


    // Draw arrows if fighting
    for (const auto& arrow : vectorArrow) {
        if (isFighting) {
            window.draw(std::get<0>(arrow));
        }
    }
}

bool Character::checkCollision(const sf::FloatRect& otherBox) const {
    return boundingBox.intersects(otherBox);
}
void Character::handleMovement(std::shared_ptr<Map>& gameMap, const std::vector<std::shared_ptr<Enemy>>& enemies, int& num, bool& isMoving) {
    sf::Vector2f movement(0.f, 0.f);
    bool isDiagonal = false;

    // Check for diagonal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(speed, -speed);
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-speed, -speed);
        num = 1; // Face left
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement = sf::Vector2f(speed, speed); 
        num = 3; // Face right
        isDiagonal = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement = sf::Vector2f(-speed, speed);
        num = 1; // Face left
        isDiagonal = true;
    }
    if (movement.x != 0.f && movement.y != 0.f) {
        movement /= std::sqrt(2.f); // Scale down diagonal movement
    }
    // Check for single key movement if no diagonal keys are pressed
    if (!isDiagonal) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement = sf::Vector2f(speed, 0);
            num = 3; // Face right
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement = sf::Vector2f(-speed, 0);
            num = 1; // Face left
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement = sf::Vector2f(0, -speed);
            num = 0; // Face up
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement = sf::Vector2f(0, speed);
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
        if (!gameMap->checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height) &&
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


    isFighting = false;
}

void Character::equipWeapon(WeaponType type){    
    std::shared_ptr<Weapon> weapon = make_shared<Weapon>(type);
    equippedWeapons.push_back(weapon);
}

void Character::handleGuardianMovement(std::shared_ptr<Map>& gameMap,
    std::shared_ptr<Character>& player,
    std::vector<std::shared_ptr<Enemy>>& enemies) {
    sf::Vector2f guardianPosition = sprite.getPosition();
    sf::Vector2f playerPosition = player->getSprite().getPosition();

    // Frame counter for enemy search optimization
    float closestEnemyDistanceSquared = 300.0f * 300.0f;

    frameCounter++;
   

    
    if (frameCounter % 10 == 0) { 
        frameCounter /= 10;// Run enemy search logic every 10th frame
        targetEnemy = nullptr;
        float closestEnemyDistanceSquared = 300.0f * 300.0f;
        
        for (const auto& enemy : enemies) {
            if (!enemy) continue; // Skip invalid pointers
            sf::Vector2f enemyPosition = enemy->getSprite().getPosition();
            sf::Vector2f distanceToEnemy = enemyPosition - guardianPosition;
            float squaredDistance = distanceToEnemy.x * distanceToEnemy.x + distanceToEnemy.y * distanceToEnemy.y;

            if (squaredDistance < closestEnemyDistanceSquared) {
                closestEnemyDistanceSquared = squaredDistance;
                targetEnemy = enemy;
            }
        }
    }

    // Decide movement target (enemy or player)
    sf::Vector2f targetPosition = targetEnemy ? targetEnemy->getSprite().getPosition() : playerPosition;

    // Calculate direction to the target
    sf::Vector2f direction = targetPosition - guardianPosition;
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude == 0) return; // Prevent division by zero
    direction /= magnitude; // Normalize direction vector

    // Calculate new position
    sf::Vector2f newPosition = guardianPosition + direction * speed * 0.5f;

    // Check collisions
    sf::FloatRect newBoundingBox(newPosition.x - 32.0f + offsetX, newPosition.y - 32.0f + offsetY, boundingBox.width, boundingBox.height);
    bool collidesWithMap = gameMap->checkCollision(newBoundingBox.left, newBoundingBox.top, newBoundingBox.width, newBoundingBox.height);
    bool collidesWithPlayer = player->checkCollision(newBoundingBox);
    bool collideEnemy = false;
    for (const auto& enemy : enemies) {
        if (enemy && enemy->checkCollision(newBoundingBox)) {
            collideEnemy = true; // Early exit if a collision with an enemy is detected
        }
    }
    // Move if no collisions
    if (!collidesWithMap && !collidesWithPlayer && !collideEnemy) {
        sprite.setPosition(newPosition);
        updateBoundingBox();
        isMoving = true;
    }

    // Attack logic
    if (targetEnemy && magnitude < 300.0f) {
        int num = getFightingDirection(targetEnemy->getSprite().getPosition() - guardianPosition);
        isFighting = true;
        isMoving = false;

        setCurWeapon(player->getWeapon(0));
        setShooting(true);
        fightBow(num, CharacterType::guard, enemies, gameMap); // Use bow to fight
    }

    // Update guardian facing direction
    if (isMoving) {
        sprite.setOrigin(32.0f, 32.0f); // Set origin to the center
        if (std::abs(direction.x) > std::abs(direction.y)) {
            if (direction.x > 0) {
                changePos(3); // Face right
            }
            else {
                changePos(1); // Face left
            }
        }
        else {
            if (direction.y > 0) {
                changePos(2); // Face down
            }
            else {
                changePos(0); // Face up
            }
        }
    }
}




