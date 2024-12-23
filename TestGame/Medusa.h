#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include <cmath>
#include <queue>
#include "Enemy.h"
#include "Character.h"
class SnakeProjectile {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float distanceTraveled = 0.0f;
    int currentFrame = 0; // Track the current animation frame
    float frameDuration = 0.1f; // Duration per frame
    float frameTimeAccumulator = 0.0f;
    int totalFrames = 8; // Total number of frames in the animation

    SnakeProjectile(sf::Sprite& breathSprite, sf::Vector2f initVelocity)
        : sprite(breathSprite), velocity(initVelocity) {}

    void update(float deltaTime) {
        // Update position
        sprite.move(velocity * deltaTime);
        distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * deltaTime;

        // Update animation frame
        frameTimeAccumulator += deltaTime;
        if (frameTimeAccumulator >= frameDuration) {
            frameTimeAccumulator -= frameDuration;
            currentFrame = (currentFrame + 1) % totalFrames; // Cycle through frames

            // Use isFireSnake to determine the frame size
            
            sprite.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32,32)); // Update frame
        }
    }

    void reset(sf::Vector2f position, sf::Vector2f velocity) {
        sprite.setPosition(position);
        this->velocity = velocity;
        distanceTraveled = 0.0f;
        currentFrame = 0; // Reset animation frame
        frameTimeAccumulator = 0.0f;

        // Set the spell type flag

       // Use isFireSnake to determine the frame size
  // fire_Snake.png: 32x32, Snake_bullet.png: 13x13
        sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Reset to the first frame
    }


};


class Medusa : public Enemy {
private:
    sf::Sprite breathSprite;
    sf::Texture breathTexture;
    float breathSpeed = 50.0f;
    std::vector<sf::Texture> vectTextures;
    sf::FloatRect attackRangeBox;
    bool hit;
    std::queue<SnakeProjectile> availableBreaths;  // Object pool
    std::vector<SnakeProjectile> activeBreaths;
    float switchTime = 5.0f; // Time interval to switch spells
    sf::Clock switchClock; // Clock to track spell switch time


public:
    void loadBreath(const std::string& path, sf::Vector2u textureSize) {
        if (breathTexture.loadFromFile(path)) {
            std::cout << "Load Breath Success\n";
        }
        breathSprite.setTexture(breathTexture);
        breathSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y)); // Set initial texture size
    }

    Medusa(sf::Vector2f spawnPosition) : Enemy(EnemyType::Medusa) {
        health = 120;
        totalFrames = 8;
        frameDuration = 0.15f;
        attackCooldown = 0.8f;


        loadTexture("../Assets/Character/Enemies/medusa.png", spawnPosition.x, spawnPosition.y);

        loadBreath("../Assets/Character/Textures/charge-dark.png", { 32, 32 });

        // Initialize object pool with some BreathProjectiles
        for (int i = 0; i < 20; ++i) {  // Adjust pool size based on expected max breath projectiles
            availableBreaths.push(SnakeProjectile(breathSprite, { 0, 0 }));
        }
    }
    void fighting(int direction, const string target) override {
        std::cout << "Dragon Attack";
    }


    void fightingD(int direction, std::shared_ptr<Character>& player, std::shared_ptr<Character>& guard, std::shared_ptr<Map>& gameMap) {
        const float maxBreathDistance = 416.0f; // Maximum distance for the breath
        //const float secondAttackDelay = 0.3f;  // Delay (in seconds) between the first and second attack



        

        if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
            // First attack: Fire the first projectile
            SnakeProjectile newBreath = availableBreaths.empty() ? SnakeProjectile(breathSprite, { 0, 0 }) : availableBreaths.front();
            availableBreaths.pop();

            sf::Vector2f MedusaPos = sprite.getPosition();
            sf::Vector2f breathPosition = MedusaPos;
            sf::Vector2f velocity;

            // Adjust position and velocity based on direction for the first projectile
            if (direction == Right) {
                breathPosition = MedusaPos + sf::Vector2f(8, -16);
                velocity = { breathSpeed, 0 };
            }
            else if (direction == Left) {
                breathPosition = MedusaPos + sf::Vector2f(-28, -16);
                velocity = { -breathSpeed, 0 };
            }
            else if (direction == Up) {
                breathPosition = MedusaPos + sf::Vector2f(-10, -45);
                velocity = { 0, -breathSpeed };
            }
            else if (direction == Down) {
                breathPosition = MedusaPos + sf::Vector2f(-10, 25);
                velocity = { 0, breathSpeed };
            }

            // Initialize or reset the first projectile
            newBreath.reset(breathPosition, velocity);
            activeBreaths.push_back(newBreath);
            

            attackCooldownClock.restart(); // Restart the cooldown timer for the next attack
        }

        // Update and remove breaths
        for (auto it = activeBreaths.begin(); it != activeBreaths.end();) {
            it->update(0.0006f); // Update breath position and animation

            attackRangeBox = sf::FloatRect(it->sprite.getPosition().x, it->sprite.getPosition().y,
                32.0f,32.0f);

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
