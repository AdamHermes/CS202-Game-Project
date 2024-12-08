#pragma once
#include <vector>
#include "Room.h"
#include "Gate.h"
class Level {
private:
    std::vector<std::shared_ptr<Room>> rooms;
    std::shared_ptr<Gate> gate;
public:
    void setGate(const sf::FloatRect& gatePosition) {
        gate = std::make_shared<Gate>(gatePosition.left, gatePosition.top, gatePosition.width, gatePosition.height);
    }
    void addRoom(const sf::Vector2f& position) {
        rooms.push_back(std::make_shared<Room>(position));
    }

    void moveEnemies(Map& gameMap, Character& player) {
        for (const auto& room : rooms) {
            room->moveEnemies(gameMap, player);
        }
    }
    std::shared_ptr<Room> getRoom(size_t index) {
        if (index >= rooms.size()) {
            throw std::out_of_range("Level index out of range!");
        }
        return rooms[index];
    }
    void render(sf::RenderWindow& window) {
        for (const auto& room : rooms) {
            room->renderEnemies(window);
        }
    }
    void populateEnemies(int currentLevelIndex) {
        // Predefine enemy positions for each room
        std::map<int, std::vector<std::pair<sf::Vector2f, EnemyType>>> enemyPositions = {
            {0, {   // Room 0
                {sf::Vector2f(600.0f, 1000.0f), EnemyType::Demon},
                {sf::Vector2f(500.0f, 1000.0f), EnemyType::Goblin},
                {sf::Vector2f(500.0f, 900.0f), EnemyType::Golem},
                {sf::Vector2f(550.0f, 450.0f), EnemyType::Golem},
                {sf::Vector2f(600.0f, 380.0f), EnemyType::Demon},
                {sf::Vector2f(600.0f, 500.0f), EnemyType::Goblin}
            }},
            {1, {
                {sf::Vector2f(600.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(500.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(500.0f, 900.0f), EnemyType::Frogman},
                {sf::Vector2f(550.0f, 450.0f), EnemyType::Goblin},
                {sf::Vector2f(600.0f, 380.0f), EnemyType::Demon},
                {sf::Vector2f(600.0f, 500.0f), EnemyType::Sunflower}
            }}
        };
        for (const auto& room : rooms) {
            room->clearEnemies();
        }
        sf::Vector2f roomPosition(currentLevelIndex  * 200.0f, currentLevelIndex * 200.0f);
        cout << rooms.size();
        addRoom(roomPosition);

        auto room = getRoom(0);
        // Add enemies from predefined positions

        for (const std::pair<sf::Vector2f, EnemyType>& enemy : enemyPositions[currentLevelIndex]) {
            room->addEnemy(enemy.second, enemy.first);
        }

    }
    bool checkGateEntry(const Character& player) const {
        return gate->isPlayerEntering(player);
    }
};