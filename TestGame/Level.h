#pragma once
#include <vector>
#include "Room.h"
#include "Gate.h"
#include "Items.h"
#include "ItemFactory.h"
class Level {
private:
    std::vector<std::shared_ptr<Room>> rooms;
    std::vector<std::shared_ptr<Items>> items;
    std::shared_ptr<Gate> gate;
    std::shared_ptr<Gate> gate2 = nullptr;
    std::shared_ptr<Gate> gate3 = nullptr;
    int currentRoomIndex = 0;
    int curLevelIndex = 0;
    std::vector<pair<std::shared_ptr<Door>, std::shared_ptr<Door>>> doors;
public:

    void setGate(const sf::FloatRect& gatePosition) {
        gate = std::make_shared<Gate>(gatePosition.left, gatePosition.top, gatePosition.width, gatePosition.height);
    }
    void setGate2(const sf::FloatRect& gatePosition) {
        gate2 = std::make_shared<Gate>(gatePosition.left, gatePosition.top, gatePosition.width, gatePosition.height);
    }
    void setGate3(const sf::FloatRect& gatePosition) {
        gate3 = std::make_shared<Gate>(gatePosition.left, gatePosition.top, gatePosition.width, gatePosition.height);
    }
    shared_ptr<Gate> getGate3() const {
        return gate3;
    }
    void addRoom(int index) {
        rooms.push_back(std::make_shared<Room>(index));
    }

    void moveEnemies(std::shared_ptr<Map> gameMap, std::shared_ptr<Character> player, std::shared_ptr<Character> guard) {
        for (const auto& room : rooms) {
            if (!room->isCleared()) room->moveEnemies(gameMap, player, guard);
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
            renderItems(window);
        }
    }
    void setMed(DamageManager*& damageManager) {
        for (const auto& room : rooms) {
            room->setMed(damageManager);
        }
    }
    void loadDoors(std::vector<pair<tuple<int, int, int, int, int>, tuple<int, int, int, int,int>>> doorPositions) {
        

        doors.clear();
        for (const auto& doorPair : doorPositions) {


            const auto& doorIn = doorPair.first;
            const auto& doorOut = doorPair.second;

            int xIn, yIn, wIn, hIn, dir1;
            std::tie(xIn, yIn, wIn, hIn, dir1) = doorIn;

            int xOut, yOut, wOut, hOut, dir2;
            std::tie(xOut, yOut, wOut, hOut, dir2) = doorOut;

            // Use std::make_shared to create Door objects
            auto doorInPtr = std::make_shared<Door>(xIn, yIn, wIn, hIn,dir1);
            auto doorOutPtr = std::make_shared<Door>(xOut, yOut, wOut, hOut,dir2);

            // Emplace back shared_ptr without moving
            doors.emplace_back(std::make_pair(doorInPtr, doorOutPtr));

            cout << "DOORS CONSTRUCT: " << doors.size() << endl;
        }
    }
    void setCurLevelIndex(int n) {
        curLevelIndex = n;
    }
    bool checkDoorEntry(const std::shared_ptr<Character> player) {

        for (int i = 0; i < doors.size(); i++) {

            if (curLevelIndex == 0 && (currentRoomIndex == 0 || currentRoomIndex == 2)) {
                auto& door1 = doors[i].second;
                auto playerBox = player->boundingBox;

                if (door1->getDir() == 0 && (playerBox.top + playerBox.height <= door1->getY() &&  // Player's top enters 80% of the door
                    playerBox.left + playerBox.width > door1->getX() &&          // Player horizontally overlaps the door
                    playerBox.left < door1->getX() + door1->getWidth())) {
                    currentRoomIndex = i;

                    return true;
                }
                else if (door1->getDir() == 2 && (playerBox.top >= door1->getY() + door1->getHeight() &&  // Player's top enters 80% of the door
                    playerBox.left + playerBox.width > door1->getX() &&          // Player horizontally overlaps the door
                    playerBox.left < door1->getX() + door1->getWidth())) {
                    currentRoomIndex = i;

                    return true;
                }
                else if (door1->getDir() == 3 && (playerBox.left + playerBox.width <= door1->getX() && // Entering from the left
                    playerBox.top + playerBox.height > door1->getY() &&
                    playerBox.top < door1->getY() + door1->getHeight())) {
                    currentRoomIndex = i;

                    return true;
                }
                else if (door1->getDir() == 1 && (playerBox.left >= door1->getX() + door1->getWidth() && // Entering from the right
                    playerBox.top + playerBox.height > door1->getY() &&
                    playerBox.top < door1->getY() + door1->getHeight())) {
                    currentRoomIndex = i;

                    return true;
                }
            }
            auto& door = doors[i].first; // Door In
            auto playerBox = player->boundingBox;
            
            if (door->getDir() == 0 && (playerBox.top + playerBox.height <= door->getY() &&  // Player's top enters 80% of the door
                playerBox.left + playerBox.width > door->getX() &&          // Player horizontally overlaps the door
                playerBox.left < door->getX() + door->getWidth())) {
                currentRoomIndex = i;
                return true;
            }
            else if (door->getDir() == 2 && (playerBox.top >= door->getY() + door->getHeight() &&
                playerBox.top <= door->getY() + door->getHeight() + playerBox.height && 
                playerBox.left + playerBox.width > door->getX() &&          // Player horizontally overlaps the door
                playerBox.left < door->getX() + door->getWidth())){
                currentRoomIndex = i;

                return true;
            }
            else if (door->getDir() == 3 && (playerBox.left + playerBox.width <= door->getX() && // Entering from the left
                playerBox.top + playerBox.height > door->getY() &&
                playerBox.top < door->getY() + door->getHeight())) {
                currentRoomIndex = i;

                return true;
            }
            else if (door->getDir() == 1 && (playerBox.left >= door->getX() + door->getWidth() && // Entering from the right
                playerBox.top + playerBox.height > door->getY() &&
                playerBox.top < door->getY() + door->getHeight())) {
                currentRoomIndex = i;

                return true;
            }
            
        }
        return false;
    }

    std::vector<std::pair<std::shared_ptr<Door>, std::shared_ptr<Door>>>& getDoors() {
        return doors;
    }
    void setRoomIndex(int index) {
        currentRoomIndex = index;
    }
    int getRoomIndex() const { return currentRoomIndex; }
    void populateItems(int currentLevelIndex) {
        cout << "GOT6";
        std::map<int, std::vector<std::pair<sf::Vector2f, ItemType>>> itemPositions = {
            {0, {   // Room 0
                {sf::Vector2f(780.0f, 950.0f), ItemType::health},
                {sf::Vector2f(1395.0f, 320.0f), ItemType::speed},
                {sf::Vector2f(1425.0f, 320.0f), ItemType::health},
                {sf::Vector2f(1290.0f,1322.0f), ItemType::health},
                {sf::Vector2f(1450.0f,1322.0f), ItemType::health}
            }},
            {1, {
                {sf::Vector2f(1030.0f, 930.0f), ItemType::health},
                {sf::Vector2f(1130.0f, 930.0f), ItemType::health},
                {sf::Vector2f(950.0f, 550.0f), ItemType::health},
                {sf::Vector2f(1230.0f,930.0f), ItemType::health},
                {sf::Vector2f(2215.0f,1370.0f), ItemType::power},
                {sf::Vector2f(2175.0f,1370.0f), ItemType::health},
                {sf::Vector2f(2255.0f,1370.0f), ItemType::speed}
                
            }},
            {2, {
                {sf::Vector2f(126.0f, 1084.0f), ItemType::health},
                {sf::Vector2f(350.0f, 400.0f), ItemType::health},
                {sf::Vector2f(800.0f, 1880.0f), ItemType::health},
                {sf::Vector2f(420.0f, 1820.0f), ItemType::health},
                {sf::Vector2f(1500.0f, 790.0f), ItemType::health},
                {sf::Vector2f(1850.0f, 1290.0f), ItemType::health}

            }},
            {3, {
                {sf::Vector2f(126.0f, 1084.0f), ItemType::statue},
                {sf::Vector2f(69.0f, 922.0f), ItemType::statue},
                {sf::Vector2f(352.0f, 1094.0f), ItemType::statue},
                {sf::Vector2f(319.0f, 221.0f), ItemType::statue},
                {sf::Vector2f(376.0f, 282.0f), ItemType::statue},
                {sf::Vector2f(640.0f, 202.0f), ItemType::statue},
                {sf::Vector2f(638.0f, 288.0f), ItemType::statue},
                {sf::Vector2f(896.0f, 263.0f), ItemType::statue},
                {sf::Vector2f(960.0f, 263.0f), ItemType::statue},
                {sf::Vector2f(1056.0f, 417.0f), ItemType::statue},
                {sf::Vector2f(990.0f, 608.0f), ItemType::statue},
                {sf::Vector2f(960.0f, 836.0f), ItemType::statue},
                {sf::Vector2f(926.0f, 1061.0f), ItemType::statue},
                {sf::Vector2f(575.0f, 1091.0f), ItemType::statue},
                {sf::Vector2f(672.0f, 902.0f), ItemType::statue},
                {sf::Vector2f(512.0f, 611.0f), ItemType::statue}
            }}
        };
        cout << "GOT5";
        clearItems();
        cout << items.size() << endl;
        cout << "GOT7";


        for (const std::pair<sf::Vector2f, ItemType>& item : itemPositions[currentLevelIndex]) {
            cout << "GOT3";
            addItem(item.second, item.first);
            cout << "GOT4";
        }
    }
    void populateEnemies(int currentLevelIndex) {
        // Predefine enemy positions for each room
        std::map<int, std::vector<std::pair<sf::Vector2f, EnemyType>>> enemyPositions = {
            {0, {   // Room 0
                {sf::Vector2f(230.0f, 900.0f), EnemyType::Mage},
                //{sf::Vector2f(240.0f, 685.0f), EnemyType::Golem},
                //{sf::Vector2f(210.0f, 900.0f), EnemyType::Boarman},
                
            }},
            {3, {   // Room 0
                
                /*{sf::Vector2f(940.0f, 350.0f), EnemyType::Golem},
                {sf::Vector2f(1000.0f, 280.0f), EnemyType::Goblin},
                {sf::Vector2f(750.0f, 320.0f), EnemyType::Goblin}*/
            }},
            {6,{

                /*{sf::Vector2f(1504.0f, 768.0f), EnemyType::Golem},
                {sf::Vector2f(1256.0f, 754.0f), EnemyType::Demon},
                {sf::Vector2f(1376,928), EnemyType::Demon}*/
            }},
            {1, {
                /*{sf::Vector2f(260.0f, 450.0f), EnemyType::Sunflower},
                {sf::Vector2f(260.0f, 320.0f), EnemyType::Spider},
                {sf::Vector2f(560.0f, 200.0f), EnemyType::Sunflower},
                {sf::Vector2f(470.0f, 380.0f), EnemyType::Spider},*/

            }},
            {4, {
                /*{sf::Vector2f(540.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(400.0f, 1000.0f), EnemyType::Frogman},
                {sf::Vector2f(340.0f, 980.0f), EnemyType::Frogman},*/
                
            }},
            {7, {
                /*{sf::Vector2f(870.0f,1000.0f), EnemyType::Worm},
                {sf::Vector2f(960.0f, 900.0f), EnemyType::Worm},
                {sf::Vector2f(1040.0f,950.0f), EnemyType::Ghost},
                {sf::Vector2f(860.0f, 915.0f), EnemyType::Ghost}*/
            }},
            {2, {
                /*{sf::Vector2f(900.0f, 290.0f), EnemyType::Dragon2},
                {sf::Vector2f(280.0f, 350.0f), EnemyType::Dragon2},
                {sf::Vector2f(610.0f, 420.0f), EnemyType::Dragon2},*/

                /*{sf::Vector2f(340.0f,480.0f),EnemyType::Skeleton},
                {sf::Vector2f(910.0f,400.0f),EnemyType::Skeleton}*/
            }},
            {5, {
                /*{sf::Vector2f(800.0f, 1900.0f), EnemyType::Dragon3},
                {sf::Vector2f(420.0f, 1720.0f), EnemyType::Dragon3},
                {sf::Vector2f(600.0f,1800.0f),EnemyType::Skeleton},
                {sf::Vector2f(530.0f,1640.0f),EnemyType::Skeleton}*/
                
            }},
            {8, {
                //{sf::Vector2f(1600.0f, 750.0f), EnemyType::Dragon1},
                //{sf::Vector2f(1900.0f, 1000.0f), EnemyType::Mage},
                //{sf::Vector2f(1750.0f, 1250.0f), EnemyType::Dragon1}

            }}
        };
        for (const auto& room : rooms) {
            room->clearEnemies();
        }

        cout << rooms.size();
        addRoom(0);
        addRoom(1);
        addRoom(2);
        auto room0 = getRoom(0);
        auto room1 = getRoom(1);
        auto room2 = getRoom(2);        
        


        int curRoom = currentLevelIndex;
        int counter = 3;
        while (counter--) {
            for (const std::pair<sf::Vector2f, EnemyType>& enemy : enemyPositions[curRoom]) {
                if (curRoom == 0 || curRoom == 1 || curRoom == 2) {
                    room0->addEnemy(enemy.second, enemy.first);

                }
                else if (curRoom == 3 || curRoom == 4 || curRoom == 5) {
                    room1->addEnemy(enemy.second, enemy.first);
                }
                else if (curRoom == 6 || curRoom == 7 || curRoom == 8) {
                    room2->addEnemy(enemy.second, enemy.first);
                }
            }
            curRoom += 3;
        }
        

    }

    bool checkGate2Entry(const std::shared_ptr<Character> player) const {
        
        return gate2->isPlayerEntering(player);
    }
    bool checkGate3Entry(const std::shared_ptr<Character> player) const {
        return gate3->isPlayerEntering(player);
    }
    bool checkGateEntry(const std::shared_ptr<Character> player) const {
        return gate->isPlayerEntering(player);
    }
    void addItem(ItemType type, const sf::Vector2f& spawnLocation) {
        cout << "GOT2";
        items.push_back(ItemFactory::createItems(type, spawnLocation));
    }
    void clearItems() {
        cout << "HELL";
        items.clear();
        cout << "CLEAR";
    }
    void renderItems(sf::RenderWindow& window) {
        for (const auto& item : items) {
            if (item) {
                item->drawTo(window);
            }
        }
        cleanupItems();
    }
    void cleanupItems() {
        items.erase(
            std::remove_if(items.begin(), items.end(),
                [](const std::shared_ptr<Items>& item) {
                    return item->isRemoved(); // Remove if enemy is marked as removed
                }),
            items.end());
    }
    std::vector<std::shared_ptr<Items>>& getItems() { return items; }
};
