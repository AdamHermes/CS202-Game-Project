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
    shared_ptr<Gate> getGate() const {
        return gate;
    }
    void addRoom(const sf::Vector2f& position) {
        rooms.push_back(std::make_shared<Room>(position));
    }

    void moveEnemies(std::shared_ptr<Map> gameMap, std::shared_ptr<Character> player, std::shared_ptr<Character> guard) {
        for (const auto& room : rooms) {
            room->moveEnemies(gameMap, player, guard);
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
            room->renderItems(window);
        }
    }
    void populateItems(int currentLevelIndex) {
        std::map<int, std::vector<std::pair<sf::Vector2f, ItemType>>> itemPositions = {
            {0, {   // Room 0
                {sf::Vector2f(820.0f, 1050.0f), ItemType::health},
                {sf::Vector2f(1350.0f, 420.0f), ItemType::speed},
                {sf::Vector2f(1380.0f, 420.0f), ItemType::health},
                
            }},
            {1, {
                {sf::Vector2f(1030.0f, 930.0f), ItemType::health},
                {sf::Vector2f(1130.0f, 930.0f), ItemType::health},
                {sf::Vector2f(950.0f, 550.0f), ItemType::health},
                
            }},
            {2, {
                {sf::Vector2f(850.0f, 300.0f), ItemType::health},
                {sf::Vector2f(350.0f, 400.0f), ItemType::health},
                {sf::Vector2f(800.0f, 1880.0f), ItemType::health},
                {sf::Vector2f(420.0f, 1820.0f), ItemType::health},
                {sf::Vector2f(1500.0f, 790.0f), ItemType::health},
                {sf::Vector2f(1850.0f, 1290.0f), ItemType::health},

            }}
        };
        for (const auto& room : rooms) {
            room->clearItems();
        }
        auto room = getRoom(0);

        for (const std::pair<sf::Vector2f, ItemType>& item : itemPositions[currentLevelIndex]) {
            room->addItem(item.second, item.first);
        }
    }
    void populateEnemies(int currentLevelIndex) {
        // Predefine enemy positions for each room
        std::map<int, std::vector<std::pair<sf::Vector2f, EnemyType>>> enemyPositions = {
            {0, {   // Room 0
                {sf::Vector2f(370.0f, 1000.0f), EnemyType::Golem},
                {sf::Vector2f(520.0f, 1080.0f), EnemyType::Boarman},
                {sf::Vector2f(250.0f, 1000.0f), EnemyType::Boarman},
                {sf::Vector2f(920.0f, 500.0f), EnemyType::Golem},
                {sf::Vector2f(880.0f, 380.0f), EnemyType::Goblin},
                {sf::Vector2f(740.0f, 420.0f), EnemyType::Goblin}
            }},
            {1, {
                {sf::Vector2f(540.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(400.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(440.0f, 900.0f), EnemyType::Frogman},
                {sf::Vector2f(260.0f, 450.0f), EnemyType::Sunflower},
                {sf::Vector2f(260.0f, 320.0f), EnemyType::Spider},
                {sf::Vector2f(560.0f, 200.0f), EnemyType::Sunflower},
                {sf::Vector2f(570.0f, 520.0f), EnemyType::Spider},
                {sf::Vector2f(870.0f,1000.0f), EnemyType::Demon},
                {sf::Vector2f(960.0f, 900.0f), EnemyType::Demon}
            }},
            {2, {
                {sf::Vector2f(1600.0f, 750.0f), EnemyType::Dragon1},
                {sf::Vector2f(1900.0f, 1000.0f), EnemyType::Dragon4},
                {sf::Vector2f(1750.0f, 1250.0f), EnemyType::Dragon1},
                {sf::Vector2f(800.0f, 1900.0f), EnemyType::Dragon3},
                {sf::Vector2f(420.0f, 1720.0f), EnemyType::Dragon3},
                {sf::Vector2f(900.0f, 290.0f), EnemyType::Dragon2},
                {sf::Vector2f(280.0f, 350.0f), EnemyType::Dragon2},
                {sf::Vector2f(610.0f, 420.0f), EnemyType::Dragon2}
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
    bool checkGateEntry(const std::shared_ptr<Character> player) const {
        return gate->isPlayerEntering(player);
    }
};
