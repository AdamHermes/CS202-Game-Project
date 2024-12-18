#pragma once
#include <vector>
#include "Enemy.h"
#include "EnemyFactory.h"

#include "DamageManager.h"
#include "Door.h"
class Room {
private:
    std::map<int, std::pair<std::tuple<int, int, int, int>, std::tuple<int, int, int, int>>> doorPositions;
    std::vector<std::shared_ptr<Enemy>> enemies;

    int index;
    bool doorsActive;
public:
    Room(const int index) : index(index) {
        
    }
    bool areDoorsActive() const { return doorsActive; }
    void setDoorsActive(bool active) { doorsActive = active; }
    bool isCleared() {
        if (enemies.size() == 0) return true;
        return false;
    }
    
    
    void clearEnemies() {
        enemies.clear();  // Clears the existing enemies
    }
    
    void addEnemy(EnemyType type, const sf::Vector2f& spawnPosition) {
        enemies.push_back(EnemyFactory::createEnemy(type, spawnPosition));
    }
    void setMed(DamageManager*& damageManager) {
        for (auto const& enemy : enemies) {
            enemy->setMediator(damageManager);
        }
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
