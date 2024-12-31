#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include <cmath>
#include <queue>

class BreathProjectile {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float distanceTraveled = 0.0f;

    BreathProjectile(sf::Sprite& breathSprite, sf::Vector2f initVelocity)
        : sprite(breathSprite), velocity(initVelocity) {}

    void update(float deltaTime) {
        sprite.move(velocity * deltaTime);
        distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * deltaTime;
    }

    void reset(sf::Vector2f position, sf::Vector2f velocity) {
        sprite.setPosition(position);
        this->velocity = velocity;
        distanceTraveled = 0.0f;
    }
};

class Dragon : public Enemy {
private:
    sf::Sprite breathSprite;
    sf::Texture breathTexture;
    float breathSpeed = 60.0f;
    std::vector<sf::Texture> vectTextures;
    sf::FloatRect attackRangeBox;
    bool hit;
    std::queue<BreathProjectile> availableBreaths;  // Object pool
    std::vector<BreathProjectile> activeBreaths;

public:
    void loadBreath(const std::string& path) {
        if (breathTexture.loadFromFile(path)) {
            std::cout << "Load Breath Success\n";
        }
        breathSprite.setTexture(breathTexture);
    }

    Dragon(sf::Vector2f spawnPosition, int t) : Enemy(EnemyType::Dragon1) {
        health = 80;
        totalFrames = 3;
        frameDuration = 0.15f;
        attackCooldown = 0.75f;
        if (t == 2 || t == 1) {
            attackCooldown += 0.25f;
        }
        else if (t == 3 || t == 4) {
            attackCooldown -= 0.25f;
        }

        loadAnimationTexture("../Assets/Character/Enemies/dragon1.png");
        loadAnimationTexture("../Assets/Character/Enemies/dragon2.png");
        loadAnimationTexture("../Assets/Character/Enemies/dragon3.png");
        loadAnimationTexture("../Assets/Character/Enemies/dragon4.png");

        // Set the initial texture from the animation frames
        sprite.setTexture(vectTextures[t - 1]);
        sprite.setTextureRect(sf::IntRect(0, 0, 144, 128));
        sprite.setOrigin(72, 64);
        sprite.setPosition(spawnPosition);
        loadBreath("../Assets/Character/Textures/breath.png");

        // Initialize object pool with some BreathProjectiles
        for (int i = 0; i < 20; ++i) {  // Adjust pool size based on expected max breath projectiles
            availableBreaths.push(BreathProjectile(breathSprite, { 0, 0 }));
        }
    }
    void fighting(int direction, const string target) override {
        std::cout << "Dragon Attack";
    }
    void loadAnimationTexture(const std::string& filePath) {
        sf::Texture texture;
        if (!texture.loadFromFile(filePath)) {
            throw std::runtime_error("Failed to load texture: " + filePath);
        }
        vectTextures.push_back(texture);
    }

    void fightingD(int direction, std::shared_ptr<Character>& player, std::shared_ptr<Character>& guard, std::shared_ptr<Map>& gameMap) {
        const float maxBreathDistance = 416.0f; // Maximum distance for the breath
        if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
            // Reuse breath from the pool if available
            BreathProjectile newBreath = availableBreaths.empty() ? BreathProjectile(breathSprite, { 0, 0 }) : availableBreaths.front();
            availableBreaths.pop();

            sf::Vector2f dragonPos = sprite.getPosition();
            sf::Vector2f breathPosition = dragonPos;
            sf::Vector2f velocity;

            // Adjust velocity and position based on direction
            if (direction == Right) {
                breathPosition = dragonPos + sf::Vector2f(128 - 108, 64 - 88);
                velocity = { breathSpeed, 0 };
                newBreath.sprite.setTextureRect(sf::IntRect(0, 64 * 2, 64, 64));
            }
            else if (direction == Left) {
                breathPosition = dragonPos + sf::Vector2f(16 - 108, 64 - 88);
                velocity = { -breathSpeed, 0 };
                newBreath.sprite.setTextureRect(sf::IntRect(0, 64 * 3, 64, 64));
            }
            else if (direction == Up) {
                breathPosition = dragonPos + sf::Vector2f(72 - 104, 16 - 96);
                velocity = { 0, -breathSpeed };
                newBreath.sprite.setTextureRect(sf::IntRect(0, 64 * 1, 64, 64));
            }
            else if (direction == Down) {
                breathPosition = dragonPos + sf::Vector2f(72 - 112, 112 - 96);
                velocity = { 0, breathSpeed };
                newBreath.sprite.setTextureRect(sf::IntRect(0, 64 * 0, 64, 64));
            }

            // Initialize or reset projectile position and velocity
            newBreath.reset(breathPosition, velocity);

            // Add breath to the active list
            activeBreaths.push_back(newBreath);

            attackCooldownClock.restart();
        }

        // Update and remove breaths
        for (auto it = activeBreaths.begin(); it != activeBreaths.end();) {
            it->update(0.0006f);  // Update breath position

            attackRangeBox = sf::FloatRect(it->sprite.getPosition().x, it->sprite.getPosition().y, 64.0f, 64.0f);
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
