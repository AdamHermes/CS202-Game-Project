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
    const int frameWidth = 192;   // Width of a single frame
    const int frameHeight = 192;  // Height of a single frame
    const int totalFrames = 8;   // Number of frames per direction
    attackRangeBox = boundingBox;
    // Update the frame based on the time elapsed
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;
        animationClock.restart();
    }
    // Update the frame based on the time elapsed
    if (direction == Right) {
        attackRangeBox.top += 20.0f;// Set position relative to character's position
        attackRangeBox.width += 60.0f;  // Long range for the spear
        attackRangeBox.height = 16.0f; // Narrow height

        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 3 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        attackRangeBox.top += 20.0f;
        attackRangeBox.left -= 60.0f;// Extend spear attack to the left
        attackRangeBox.width = 90.0f;
        attackRangeBox.height = 16.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 1 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        attackRangeBox.top -= 60.0f;
        attackRangeBox.height += 60.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        attackRangeBox.height += 60.0f;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight));
    }
    
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
        

        if (manager) {
            manager->notify("PlayerAttack", curWeapon->getDamage());
            //std::cout << "Attack launched" << std::endl;

        }
        attackCooldownClock.restart();
    }
}
void Character::fightBow(int direction, CharacterType type, const std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Map>& gameMap) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 64; // Height of a single frame
    const int totalFrames = 13; // Total frames for bow animation
    const float arrowSpeed = 300.0f; // Arrow speed
    const float maxArrowDistance = 384.0f; // Maximum arrow travel distance
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
        ArrowProjectile newArrow = availableArrows.empty() ? ArrowProjectile(arrowSprite, { 0, 0 }) : availableArrows.front();
        availableArrows.pop();

        sf::Vector2f playerPos = sprite.getPosition();
        sf::Vector2f arrowPos = playerPos;
        sf::Vector2f velocity;
        if (direction == Left) {
            arrowPos = playerPos + sf::Vector2f(-64,0);
        }
        else if (direction == Up) {
            arrowPos = playerPos + sf::Vector2f(0, -64);
        }
        
        if (direction == Right) {
            velocity = { arrowSpeed, 0 };
            newArrow.sprite.setTextureRect(sf::IntRect(0, frameHeight * 2, frameWidth, frameHeight)); // Right-facing arrow
        }
        else if (direction == Left) {
            velocity = { -arrowSpeed, 0 };
            newArrow.sprite.setTextureRect(sf::IntRect(0, frameHeight * 3, frameWidth, frameHeight)); // Left-facing arrow
        }
        else if (direction == Up) {
            velocity = { 0, -arrowSpeed };
            newArrow.sprite.setTextureRect(sf::IntRect(0, frameHeight * 1, frameWidth, frameHeight)); // Up-facing arrow
        }
        else if (direction == Down) {
            velocity = { 0, arrowSpeed };
            newArrow.sprite.setTextureRect(sf::IntRect(0, frameHeight * 0, frameWidth, frameHeight)); // Down-facing arrow
        }

        // Add arrow to vectorArrow
        newArrow.reset(arrowPos, velocity);
        activeArrows.push_back(newArrow);
        attackCooldownClock.restart(); // Reset attack cooldown
    }

    // Update all arrows in flight
    for (auto it = activeArrows.begin(); it != activeArrows.end();) {
        it->update(0.0006f);
        
        if (direction == Right || direction == Left) {
            attackRangeBox = sf::FloatRect(it->sprite.getPosition().x, it->sprite.getPosition().y, 64.0f, 16.0f); // Horizontal arrow
        }
        else if (direction == Up || direction == Down) {
            attackRangeBox = sf::FloatRect(it->sprite.getPosition().x, it->sprite.getPosition().y, 16.0f, 64.0f); // Vertical arrow
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
        if (hit || it->distanceTraveled >= maxArrowDistance) {
            availableArrows.push(*it);
            it = activeArrows.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Character::setShooting(bool shooting) {
    isShooting = shooting;
    if (!isShooting) {
        activeArrows.clear(); // Clear all arrows when spacebar is released
    }
}

void Character::applyItemEffect(std::shared_ptr<Items> item) {
    switch (item->getType()) {
    case ItemType::health:
        if (healingTimer.getElapsedTime().asSeconds() >= 3.0f) {
            health += 10;
            if (health > 100) health = 100;
            sprite.setColor(sf::Color(0, 255, 0, 128));  
            healingTimer.restart();
        }
        break;
    case ItemType::speed:
        if (speedingTimer.getElapsedTime().asSeconds() >= 3.0f) {
            speed += 0.02f;
            sprite.setColor(sf::Color(0, 191, 255, 128)); 
            speedingTimer.restart();
        }
        break;
    case ItemType::power:
        if (poweringTimer.getElapsedTime().asSeconds() >= 3.0f) {
            baseStrength += 5;
            sprite.setColor(sf::Color(255, 165, 0, 128)); 

            poweringTimer.restart();
        }
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
    

        if (manager) {
            manager->notify("PlayerAttack", curWeapon->getDamage());
            
        }
        attackCooldownClock.restart();
    }
    
}
void Character::loadTexture(const std::string& path, bool isBig, int num, float x, float y) {

    if (texture.loadFromFile(path)) {
        //std::cout << "Load Success: " << path << std::endl;
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

void Character::updateState(bool fighting, int num, WeaponType weaponType, int char_id) {
    if (fighting != isFighting || (curWeapon->getType() != weaponType && weaponType != WeaponType::None)) {
        isFighting = fighting;
        if (isFighting) {
            if (weaponType == WeaponType::Sword) {
                loadTexture("../Assets/Character/Textures/slash.png", true, num, sprite.getPosition().x, sprite.getPosition().y);
            }
            else if (weaponType == WeaponType::Spear) {
                cout << "Fight Weapon Spear" << endl;
                if (char_id == 1) {
                    loadTexture("../Assets/Character/Textures/thrust.png", true, num, sprite.getPosition().x, sprite.getPosition().y);
                }
                else if (char_id == 3) {
                    cout << "GOT HERE 3";
                    loadTexture("../Assets/Character/Textures/thrust1.png", true, num, sprite.getPosition().x, sprite.getPosition().y);

                }


            }
            else {
                if (char_id == 1) {
                    loadTexture("../Assets/Character/Textures/character1.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
                }
                else if (char_id == 3) {
                    loadTexture("../Assets/Character/Textures/character3.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
                }
                else {
                    loadTexture("../Assets/Character/Textures/character2.png", false, num, sprite.getPosition().x, sprite.getPosition().y);

                }

            }
        }
        else {
            if (char_id == 1) {
                loadTexture("../Assets/Character/Textures/character1.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
            }
            else if (char_id == 3) {
                loadTexture("../Assets/Character/Textures/character3.png", false, num, sprite.getPosition().x, sprite.getPosition().y);
            }
            else {
                loadTexture("../Assets/Character/Textures/character2.png", false, num, sprite.getPosition().x, sprite.getPosition().y);

            }

        }
    }
}
std::shared_ptr<Items> Character::checkItemNearby(std::vector<shared_ptr<Items>>& items_inventory) {
    std::shared_ptr<Items> nearbyItem = nullptr;
    for (auto& item : items_inventory) {
        if (boundingBox.intersects(item->getSprite().getGlobalBounds())) {
            nearbyItem = item;

            if (!item->getHighlightState()) {
                item->highlight(); 
            }
        }
        else {
            if (item->getHighlightState()) {
                item->removeHighlight(); 
            }
        }
    }
    return nearbyItem;
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
    float baseX = viewCenter.x + (viewSize.x / 2) - 160.0f; 
    float baseY = viewCenter.y - (viewSize.y / 2) + 30.0f; 

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
    if (damageFlashTimer.getElapsedTime().asSeconds() > 0.2f && healingTimer.getElapsedTime().asSeconds() > 0.35f &&
        speedingTimer.getElapsedTime().asSeconds() > 0.35f && 
        poweringTimer.getElapsedTime().asSeconds() > 0.35f ) {
        sprite.setColor(sf::Color::White);  
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
    for (const auto& arrow : activeArrows) {
        if (isFighting) {
            window.draw(arrow.sprite);
        }
    }
    if (isUsingSkill) {
        window.draw(skillSprite);
    }
}
void Character::fightSkill() {
    if (!isUsingSkill) return;
    if (skillDuration.getElapsedTime().asSeconds() > 3.0f) {
        // Skill duration has ended, hide the skill and reset
        skillSprite.setPosition(-1000, -1000);  
        isUsingSkill = false;// Move out of screen
        return; // Exit the function, no more animation
    }
    frameDuration = 0.1f;
    const int frameWidth = 128;   // Width of a single frame
    const int frameHeight = 128;  // Height of a single frame
    const int totalFrames = 8;   // Number of frames per direction
    attackRangeBox = boundingBox;
    if (skillClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrameSkill = (currentFrameSkill + 1) % totalFrames;  // Loop the frames
        skillClock.restart();  // Reset the clock for the next frame
    }
    // Update the frame based on the time elapsed
    skillSprite.setTextureRect(sf::IntRect(currentFrameSkill * frameWidth, 0, frameWidth, frameHeight));
    skillSprite.setPosition(sprite.getPosition() - sf::Vector2f(64,64));
    attackRangeBox = sf::FloatRect(skillSprite.getPosition().x, skillSprite.getPosition().y, 128, 128);
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {


        if (manager) {
            manager->notify("PlayerAttack", 15);
            //std::cout << "Attack launched" << std::endl;

        }
        attackCooldownClock.restart();
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
    if (magnitude > 400.0f) return;
    if (magnitude == 0) return; // Prevent division by zero
    direction /= magnitude; // Normalize direction vector

    // Calculate new position
    sf::Vector2f newPosition = guardianPosition + direction * speed;

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




