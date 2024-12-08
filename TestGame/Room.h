#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyFactory.h"
#include "DamageManager.h"
class Room {
private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    sf::Vector2f position; // Room's position on the level map (optional)

public:
    Room(const sf::Vector2f& pos) : position(pos) {
        // Ensure position is correctly initialized
        std::cout << "Room created at: " << position.x << ", " << position.y << std::endl;
    }
    void clearEnemies() {
        enemies.clear();  // Clears the existing enemies
    }
    void addEnemy(EnemyType type, const sf::Vector2f& spawnPosition) {
        enemies.push_back(EnemyFactory::createEnemy(type, spawnPosition));
    }
    void setMed(DamageManager* damageManager) {
        for (auto const& enemy : enemies) {
            enemy->setMediator(damageManager);
        }
    }
    void renderEnemies(sf::RenderWindow& window) {
        for (const auto& enemy : enemies) {
            if (enemy) {
                enemy->drawTo(window);
                enemy->drawBoundingBox(window);
            }
        }
        cleanupEnemies();
    }
    void moveEnemies(Map& gameMap, Character& player) {
        for (const auto& enemy : enemies) {
            enemy->handleMovement(gameMap, player);
            enemy->updateDead();
        }
    }   
    void cleanupEnemies() {
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::shared_ptr<Enemy>& enemy) {
                    return enemy->isRemoved(); // Remove if enemy is marked as removed
                }),
            enemies.end());
    }
    std::vector<std::shared_ptr<Enemy>>& getEnemies() { return enemies; }
};
