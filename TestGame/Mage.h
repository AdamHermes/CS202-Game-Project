#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include <cmath>
#include <queue>
#include "Enemy.h"
#include "Character.h"
class MagicProjectile {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float distanceTraveled = 0.0f;
    int currentFrame = 0; // Track the current animation frame
    float frameDuration = 0.1f; // Duration per frame
    float frameTimeAccumulator = 0.0f;
    int totalFrames = 5; // Total number of frames in the animation

    MagicProjectile(sf::Sprite& breathSprite, sf::Vector2f initVelocity)
        : sprite(breathSprite), velocity(initVelocity) {}

    void update(float deltaTime, bool isFireMagic) {
        // Update position
        sprite.move(velocity * deltaTime);
        distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * deltaTime;

        // Update animation frame
        frameTimeAccumulator += deltaTime;
        if (frameTimeAccumulator >= frameDuration) {
            frameTimeAccumulator -= frameDuration;
            currentFrame = (currentFrame + 1) % totalFrames; // Cycle through frames

            // Use isFireMagic to determine the frame size
            int frameWidth = isFireMagic ? 32 : 13;  // fire_magic.png: 32x32, magic_bullet.png: 13x13
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, isFireMagic ? 32 : 13)); // Update frame
        }
    }

    void reset(sf::Vector2f position, sf::Vector2f velocity, bool isFireMagic) {
        sprite.setPosition(position);
        this->velocity = velocity;
        distanceTraveled = 0.0f;
        currentFrame = 0; // Reset animation frame
        frameTimeAccumulator = 0.0f;

         // Set the spell type flag

        // Use isFireMagic to determine the frame size
        int frameWidth = isFireMagic ? 32 : 13;  // fire_magic.png: 32x32, magic_bullet.png: 13x13
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, isFireMagic ? 32 : 13)); // Reset to the first frame
    }


};


class Mage : public Enemy {
private:
    sf::Sprite breathSprite;
    sf::Texture breathTexture;
    float breathSpeed = 85.0f;
    std::vector<sf::Texture> vectTextures;
    sf::FloatRect attackRangeBox;
    bool hit;
    std::queue<MagicProjectile> availableBreaths;  // Object pool
    std::vector<MagicProjectile> activeBreaths;
    float switchTime = 5.0f; // Time interval to switch spells
    sf::Clock switchClock; // Clock to track spell switch time

    bool isFireMagic = true; // Flag to track which spell is active
public:
    void loadBreath(const std::string& path, sf::Vector2u textureSize) {
        if (breathTexture.loadFromFile(path)) {
            std::cout << "Load Breath Success\n";
        }
        breathSprite.setTexture(breathTexture);
        breathSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y)); // Set initial texture size
    }

    Mage(sf::Vector2f spawnPosition) : Enemy(EnemyType::Mage) {
        health = 100;
        totalFrames = 8;
        frameDuration = 0.15f;
        attackCooldown = 0.8f;
        

        loadTexture("../Assets/Character/Enemies/mage.png", spawnPosition.x, spawnPosition.y);

        loadBreath("../Assets/Character/Textures/fire_magic.png", { 32, 32 });

        // Initialize object pool with some BreathProjectiles
        for (int i = 0; i < 40; ++i) {  // Adjust pool size based on expected max breath projectiles
            availableBreaths.push(MagicProjectile(breathSprite, { 0, 0 }));
        }
    }
    void fighting(int direction, const string target) override {
        std::cout << "Dragon Attack";
    }
    

    void fightingD(int direction, std::shared_ptr<Character>& player, std::shared_ptr<Character>& guard, std::shared_ptr<Map>& gameMap) {
        const float maxBreathDistance = 416.0f; // Maximum distance for the breath
        const float secondAttackDelay = 0.3f;  // Delay (in seconds) between the first and second attack

        // Switch spell type every 5 seconds
        static sf::Clock spellSwitchClock;
        if (spellSwitchClock.getElapsedTime().asSeconds() >= 5.0f) {
            isFireMagic = !isFireMagic; // Toggle between fire_magic and magic_bullet

            // Load the appropriate texture and set texture size based on magic type
            if (isFireMagic) {
                loadBreath("../Assets/Character/Textures/fire_magic.png", sf::Vector2u(32, 32));  // fire magic texture is 32x32
            }
            else {
                loadBreath("../Assets/Character/Textures/mage_bullet.png", sf::Vector2u(13, 13));  // magic bullet texture is 13x13
            }

            spellSwitchClock.restart(); // Restart the clock to switch magic every 5 seconds
        }

        if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
            // First attack: Fire the first projectile
            MagicProjectile newBreath = availableBreaths.empty() ? MagicProjectile(breathSprite, { 0, 0 }) : availableBreaths.front();
            availableBreaths.pop();

            sf::Vector2f magePos = sprite.getPosition();
            sf::Vector2f breathPosition = magePos;
            sf::Vector2f velocity;

            // Adjust position and velocity based on direction for the first projectile
            if (direction == Right) {
                breathPosition = magePos + sf::Vector2f(60, -16);
                velocity = { breathSpeed, 0 };
            }
            else if (direction == Left) {
                breathPosition = magePos + sf::Vector2f(-60, -16);
                velocity = { -breathSpeed, 0 };
            }
            else if (direction == Up) {
                breathPosition = magePos + sf::Vector2f(-10, -45);
                velocity = { 0, -breathSpeed };
            }
            else if (direction == Down) {
                breathPosition = magePos + sf::Vector2f(-10, 45);
                velocity = { 0, breathSpeed };
            }

            // Initialize or reset the first projectile
            newBreath.reset(breathPosition, velocity, isFireMagic);
            activeBreaths.push_back(newBreath);

            // Now handle the second projectile after a delay
            static sf::Clock secondAttackClock;
            if (secondAttackClock.getElapsedTime().asSeconds() >= secondAttackDelay) {
                // Fire second attack after the delay
                MagicProjectile secondBreath = availableBreaths.empty() ? MagicProjectile(breathSprite, { 0, 0 }) : availableBreaths.front();
                availableBreaths.pop();

                // Adjust position and velocity for the second projectile (offset)
                sf::Vector2f secondBreathPosition = magePos;
                sf::Vector2f secondVelocity;

                if (direction == Right) {
                    secondBreathPosition = magePos + sf::Vector2f(60, -16 + 20);  // Second bullet slightly offset
                    secondVelocity = { breathSpeed, 0 };
                }
                else if (direction == Left) {
                    secondBreathPosition = magePos + sf::Vector2f(-60, -16 + 20);
                    secondVelocity = { -breathSpeed, 0 };
                }
                else if (direction == Up) {
                    secondBreathPosition = magePos + sf::Vector2f(-24, -45);
                    secondVelocity = { 0, -breathSpeed };
                }
                else if (direction == Down) {
                    secondBreathPosition = magePos + sf::Vector2f(-24, 45);
                    secondVelocity = { 0, breathSpeed };
                }

                // Initialize or reset the second projectile
                secondBreath.reset(secondBreathPosition, secondVelocity, isFireMagic);
                activeBreaths.push_back(secondBreath);

                // Restart the clock for the next attack
                secondAttackClock.restart();
            }

            attackCooldownClock.restart(); // Restart the cooldown timer for the next attack
        }

        // Update and remove breaths
        for (auto it = activeBreaths.begin(); it != activeBreaths.end();) {
            it->update(0.0006f,isFireMagic); // Update breath position and animation

            attackRangeBox = sf::FloatRect(it->sprite.getPosition().x, it->sprite.getPosition().y,
                isFireMagic ? 32.0f : 13.0f,
                isFireMagic ? 32.0f : 13.0f);

            hit = false;

            if (gameMap->checkCollision(attackRangeBox.left, attackRangeBox.top, attackRangeBox.width, attackRangeBox.height)) {
                hit = true;
            }
            bool spec = false;
            bool collidesWithPlayer = player->checkCollision(attackRangeBox);
            bool collidesWithGuard = false;
            if (guard) {
                collidesWithGuard = guard->checkCollision(attackRangeBox);
            }

            if (hit && collidesWithPlayer) spec = true;

            if ((!hit && collidesWithPlayer) || (!hit && collidesWithGuard) || spec) {
                // Notify damage only once per breath

                if (manager) {

                    if (collidesWithPlayer) {

                        manager->notify("EnemyAttackPlayer", 11);


                    }
                    if (collidesWithGuard) {

                        manager->notify("EnemyAttackGuard", 11);
                    }
                    hit = true;
                }
            }

            if (hit || it->distanceTraveled >= maxBreathDistance) {
                availableBreaths.push(*it);  // Return to pool
                it = activeBreaths.erase(it);
            }
            else {
                ++it;
            }
        }
    }



    void drawTo(sf::RenderWindow& window) const override {
        if (opacity > 0)
            window.draw(sprite);
        for (const auto& breath : activeBreaths) {
            window.draw(breath.sprite);
        }
    }
};
