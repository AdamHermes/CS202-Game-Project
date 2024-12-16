#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyFactory.h"
#include "Items.h"
#include "ItemFactory.h"
#include "DamageManager.h"
class Room {
private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Items>> items;
    sf::Vector2f position; // Room's position on the level map (optional)

public:
    Room(const sf::Vector2f& pos) : position(pos) {
        // Ensure position is correctly initialized
        std::cout << "Room created at: " << position.x << ", " << position.y << std::endl;
    }
    void clearItems() {
        items.clear();
    }
    void clearEnemies() {
        enemies.clear();  // Clears the existing enemies
    }
    void addItem(ItemType type, const sf::Vector2f& spawnLocation) {
        items.push_back(ItemFactory::createItems(type, spawnLocation));
    }
    void addEnemy(EnemyType type, const sf::Vector2f& spawnPosition) {
        enemies.push_back(EnemyFactory::createEnemy(type, spawnPosition));
    }
    void setMed(DamageManager* damageManager) {
        for (auto const& enemy : enemies) {
            enemy->setMediator(damageManager);
        }
    }
    void renderItems(sf::RenderWindow& window) {
        for (const auto& item : items) {
            if (item) {
                item->drawTo(window);
            }
        }
        cleanupItems();
    }
    void renderEnemies(sf::RenderWindow& window) {
        for (const auto& enemy : enemies) {
            if (enemy) {
                enemy->drawTo(window);
                //enemy->drawBoundingBox(window);
            }
        }
        cleanupEnemies();
    }
    void moveEnemies(std::shared_ptr<Map> gameMap, std::shared_ptr<Character> player, std::shared_ptr<Character> guard) {
        for (const auto& enemy : enemies) {
            enemy->handleMovement(gameMap, player,guard);
            enemy->updateDead();
        }
        
    }   
    void cleanupItems() {
        items.erase(
            std::remove_if(items.begin(), items.end(),
                [](const std::shared_ptr<Items>& item) {
                    return item->isRemoved(); // Remove if enemy is marked as removed
                }),
            items.end());
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
    std::vector<std::shared_ptr<Items>>& getItems() { return items; }

};
