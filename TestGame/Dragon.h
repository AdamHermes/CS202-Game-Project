#pragma once
#include "Enemy.h"
class Dragon : public Enemy {
private:
	sf::Sprite breathSprite;
	sf::Texture breathTexture;
	float breathSpeed = 30.0f;
    std::vector<sf::Texture> vectTextures;
	sf::FloatRect attackRangeBox;
	bool hit;
	std::vector<std::tuple<sf::Sprite, sf::Vector2f, float>> vectorBreath;
public:
	void loadBreath(const string& path) {
		if (breathTexture.loadFromFile(path)) {
			std::cout << "Load Breath Success\n";
		}
		breathSprite.setTexture(breathTexture);
	}
	Dragon(sf::Vector2f spawnPosition, int t) : Enemy(EnemyType::Dragon1) {
		health = 110;
		totalFrames = 3;    
		frameDuration = 0.15f;
        attackCooldown = 1.0f;
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
        sprite.setTexture(vectTextures[t-1]);
        sprite.setTextureRect(sf::IntRect(0, 0, 144, 128));
        sprite.setOrigin(72, 64);
        sprite.setPosition(spawnPosition);
        loadBreath("../Assets/Character/Textures/breath.png");
    }

    void loadAnimationTexture(const std::string& filePath) {
        sf::Texture texture;
        if (!texture.loadFromFile(filePath)) {
            throw std::runtime_error("Failed to load texture: " + filePath);
        }
        vectTextures.push_back(texture);
    }
	void fighting(int direction, std::shared_ptr<Character>& player) override {
		std::cout << "Dragon Attack";
	}
    void fightingD(int direction, std::shared_ptr<Character>& player, std::shared_ptr<Character>& guard, std::shared_ptr<Map>& gameMap) {
        
        
        const float maxBreathDistance = 416.0f; // Maximum distance for the breath
        if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {
            sf::Sprite newBreath = breathSprite;

            sf::Vector2f dragonPos = sprite.getPosition();
            sf::Vector2f breathPosition = dragonPos; 

            sf::Vector2f velocity;
            if (direction == Right) {

                breathPosition = dragonPos + sf::Vector2f(128 - 108, 64 - 88);
                velocity = { breathSpeed, 0 };
                newBreath.setTextureRect(sf::IntRect(0, 64 * 2, 64, 64));
            }
            else if (direction == Left) {

                breathPosition = dragonPos + sf::Vector2f(16 - 108, 64 - 88);
                velocity = { -breathSpeed, 0 };
                newBreath.setTextureRect(sf::IntRect(0, 64 * 3, 64, 64)); 
            }
            else if (direction == Up) {

                breathPosition = dragonPos + sf::Vector2f(72 - 104, 16 - 96);
                velocity = { 0, -breathSpeed };
                newBreath.setTextureRect(sf::IntRect(0, 64 * 1, 64, 64));
            }
            else if (direction == Down) {

                breathPosition = dragonPos + sf::Vector2f(72 - 112, 112 - 96);
                velocity = { 0, breathSpeed };
                newBreath.setTextureRect(sf::IntRect(0, 64 * 0, 64, 64)); 
            }


            newBreath.setPosition(breathPosition);


            vectorBreath.push_back({ newBreath, velocity, 0.0f }); 

            attackCooldownClock.restart();
        }


        for (auto it = vectorBreath.begin(); it != vectorBreath.end();) {
            sf::Sprite& Breath = std::get<0>(*it);       // Breath sprite
            sf::Vector2f& velocity = std::get<1>(*it);    // Breath velocity
            float& distanceTraveled = std::get<2>(*it);   // Distance traveled


            Breath.move(velocity * 0.0006f);
            distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * 0.0006f;


            attackRangeBox = sf::FloatRect(Breath.getPosition().x, Breath.getPosition().y, 64.0f, 64.0f);


            hit = false;
            if (gameMap->checkCollision(attackRangeBox.left, attackRangeBox.top, attackRangeBox.width, attackRangeBox.height)) {
                hit = true;
            }

            bool spec = false;
            bool collidesWithPlayer = player->checkCollision(attackRangeBox);
            bool collidesWithGuard = guard->checkCollision(attackRangeBox);
            if (hit && collidesWithPlayer) spec = true;

            if ((!hit && collidesWithPlayer) || (!hit && collidesWithGuard) || spec) {
                // Notify damage only once per breath
                if (manager) {
                    if (collidesWithPlayer) {
                        manager->notify("EnemyAttackPlayer", 10);
                    }
                    if (collidesWithGuard) {
                        manager->notify("EnemyAttackGuard", 10);

                    }
                    hit = true;
                }
            }


            if (hit || distanceTraveled >= maxBreathDistance) {
                it = vectorBreath.erase(it);
            }
            else {
                ++it;
            }
        }
    }

	void drawTo(sf::RenderWindow& window) const override {
		if (opacity > 0)
			window.draw(sprite);
		for (const auto& breath : vectorBreath) {
			const sf::Sprite& breathSprite = std::get<0>(breath);
			window.draw(breathSprite);
		}
	}
};