#pragma once
#include "Level.h"
#include "DamageManager.h"
#include "Map.h"
class GameLoop {
private:
    std::vector<std::shared_ptr<Level>> levels;
    std::shared_ptr<Level> curLevel;
    std::shared_ptr<Character> player;
    std::shared_ptr<Character> guard;// Player character
    std::shared_ptr<Map> gameMap;
    int currentLevelIndex = 0;                // Index of the current level
    DamageManager* damageManager;
    void cleanupLevel() {
        curLevel.reset(); // Release the current level
    }
    sf::Sprite doorSprite;
    sf::Texture doorTexture;

public:
    std::vector<pair<tuple<int, int, int, int,int>, tuple<int, int, int, int,int>>> doorPositions;
    bool playerInRoom = false;
    void drawBoundingBox(sf::RenderWindow& window, sf::FloatRect boundingBox) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent); 
        boundingBoxShape.setOutlineColor(sf::Color::Red); 
        boundingBoxShape.setOutlineThickness(2); 

        window.draw(boundingBoxShape); 
    }
    GameLoop(std::shared_ptr<Character>& player, std::shared_ptr<Map>& gameMap, std::shared_ptr<Character>& guard)
        : player(player), gameMap(gameMap), guard(guard) {
        if (doorTexture.loadFromFile("../Assets/Character/Textures/door1.png")) {
            doorSprite.setTexture(doorTexture);
        }
    }   

    void addLevel(const sf::FloatRect& gatePosition) {
        auto newLevel = std::make_shared<Level>();
        newLevel->setGate(gatePosition); 
        if (levels.size() == 1) {
            sf::FloatRect modifiedGatePosition = gatePosition;
            modifiedGatePosition.left -= 100.f;
            newLevel->setGate2(modifiedGatePosition);
            sf::FloatRect gateReturn(2368, 1120, 70, 10);
            newLevel->setGate3(gateReturn);
        }
        levels.push_back(newLevel);
    }

    void addLevels() {
        sf::FloatRect gatePositionLevel1(160.0f, 200.0f, 100.0f, 50.0f);  // Gate for level 1
        sf::FloatRect gatePositionLevel2(1030.0f, 1330.0f, 100.0f, 50.0f);  // Gate for level 2
        sf::FloatRect gatePositionLevel3(0, 0, 0, 0);

        addLevel(gatePositionLevel1);  // Level 1
        addLevel(gatePositionLevel2);  // Level 2
        addLevel(gatePositionLevel3);
    }
    void loadEnemiesForRooms(int currentLevelIndex) {

        cout << levels.size();
        curLevel = levels[currentLevelIndex];
        if (currentLevelIndex >= 1) {
            std::shared_ptr<Level> prevLevel = levels[currentLevelIndex - 1];
            prevLevel->getRoom(0)->clearEnemies();
            prevLevel->getRoom(1)->clearEnemies();
        }

        curLevel->populateEnemies(currentLevelIndex);
    }
    void loadItemsForRooms(int currentLevelIndex) {
        if (currentLevelIndex >= 1) {
            std::shared_ptr<Level> prevLevel = levels[currentLevelIndex - 1];
            prevLevel->clearItems();
        }
        
        curLevel->populateItems(currentLevelIndex);
    }
    void loadNextLevel() {
        if (currentLevelIndex + 1 < levels.size()) {
            cleanupLevel();
            currentLevelIndex++;
            gameMap->obstacles.clear();
            if (damageManager) {
                delete damageManager;
                damageManager = nullptr; 
            }

            std::string texturePath = "../Assets/Character/Textures/map" + std::to_string(currentLevelIndex) + ".png";
            std::string tmxPath = "../Assets/Character/TMX MAP/map" + std::to_string(currentLevelIndex) + ".tmx";
            std::string doorPath = "../Assets/Character/Textures/door" + std::to_string(currentLevelIndex + 1) + ".png";
            if (!gameMap->loadTexture(texturePath) || !gameMap->loadFromTMX(tmxPath)) {
                std::cerr << "Failed to load map for level " << currentLevelIndex + 1 << std::endl;
                return;
            }
            if (doorTexture.loadFromFile(doorPath)) {
                doorSprite.setTexture(doorTexture);
            }
            if (currentLevelIndex == 1) {
                player->setPosition( 1280, 260); //1300 260
                guard->loadTexture("../Assets/Character/Textures/characters.png", false, 2, 1470, 930);
                guard->updateBoundingBox();
                guard->equipWeapon(WeaponType::Bow);
                auto start_weapon_guard = guard->getWeapon(0);
                guard->setCurWeapon(start_weapon_guard);
                doorPositions.clear();
                doorPositions.push_back({
                    std::make_tuple(640, 288, 32, 64,3), // Door In for Room 1
                    std::make_tuple(352, 544, 96, 32,3)   // Door Out for Room 1
                });
                doorPositions.push_back({
                    std::make_tuple(352, 768, 96, 32,2), // Door In for Room 1
                    std::make_tuple(352, 1168, 96, 32,2)   // Door Out for Room 1
                });
                playerInRoom = false;
            }
            else if (currentLevelIndex == 2) {
                doorPositions.clear();
                player->setPosition(160, 950);
                guard->setPosition(130, 1000);
                guard->setShooting(false);
                doorPositions.push_back({
                    std::make_tuple(608, 672, 96, 32,0), // Door In for Room 1
                    std::make_tuple(608, 672, 96, 32,0)   // Door Out for Room 1
                });
                doorPositions.push_back({
                    std::make_tuple(608, 1408, 96, 32,2), // Door In for Room 1
                    std::make_tuple(608, 1408, 96, 32,2)   // Door Out for Room 1
                });
                doorPositions.push_back({
                    std::make_tuple(1312, 992, 32, 64,1), // Door In for Room 1
                    std::make_tuple(1312, 992, 32, 64,1)   // Door Out for Room 1
                });
                playerInRoom = false;

            }
            
            std::cout << "Loaded map for level " << currentLevelIndex + 1 << std::endl;
            cout << "Cur Index " << currentLevelIndex << endl;
            loadEnemiesForRooms(currentLevelIndex);
            curLevel->loadDoors(doorPositions);
            loadItemsForRooms(currentLevelIndex);
            updateDamageManager();

        }
        else {
            std::cout << "You have completed the game!" << std::endl;
        }
    }
    bool isGuardDead() {
        if (guard->isDead()) {
            guard.reset();
            

            return true;
        }
        return false;
    }
    bool updateDoors() {
        
        if (curLevel->checkDoorEntry(player)) {
            int currentRoomIndex = curLevel->getRoomIndex();
            auto room = curLevel->getRoom(currentRoomIndex);
            auto doors = curLevel->getDoors();
            if (currentLevelIndex == 2) {
                updateDamageManager();
                if (guard != nullptr && curLevel->checkDoorEntry(guard)) {


                    if (!room->isCleared()) {
                        gameMap->obstacles.push_back(doors[currentRoomIndex].first);  // "door in"
                        gameMap->obstacles.push_back(doors[currentRoomIndex].second); // "door out"
                        room->setDoorsActive(true);

                        return true;
                    }
                }
                else if (!guard) {

                    if (!room->isCleared()) {
                        gameMap->obstacles.push_back(doors[currentRoomIndex].first);  // "door in"
                        gameMap->obstacles.push_back(doors[currentRoomIndex].second); // "door out"
                        room->setDoorsActive(true);

                        return true;
                    }
                }
            }           
            else {
                if (!room->isCleared()) {
                    // Copy shared_ptr of doors into gameMap obstacles
                    gameMap->obstacles.push_back(doors[currentRoomIndex].first);  // "door in"
                    gameMap->obstacles.push_back(doors[currentRoomIndex].second); // "door out"
                    room->setDoorsActive(true);

                    return true;
                }
            }
            
        }

        int currentRoomIndex = curLevel->getRoomIndex();
        auto room = curLevel->getRoom(currentRoomIndex);

        if (room->isCleared() && room->areDoorsActive()) {
            auto& doors = curLevel->getDoors();
            if (currentLevelIndex == 2) cout << "KILLED THE DRAGON";
            // Retrieve shared_ptr back from gameMap obstacles
            gameMap->obstacles.pop_back();
            gameMap->obstacles.pop_back();

            
            room->setDoorsActive(false);
 
            if ((currentLevelIndex == 1 || currentLevelIndex == 0) && curLevel->getRoomIndex() == 0) {
                room = curLevel->getRoom(curLevel->getRoomIndex() + 1);
                curLevel->setRoomIndex(curLevel->getRoomIndex() + 1);
                updateDamageManager();
            }
            


            
        }
        return false;
    }
    void updateLevel2() {
        int curRoom = curLevel->getRoomIndex();
        if (currentLevelIndex == 1 && curRoom == 1 && curLevel->getRoom(curRoom)->isCleared()) {
            static bool transitioning = false;      
            static sf::Clock transitionClock;       

            if (!transitioning) {
                // Check for gate 2 entry
                if (curLevel->checkGate2Entry(player)) {
                    transitioning = true;
                    transitionClock.restart();
                    player->setPosition(2200, 1430); 
                    return;
                }

                // Check for gate 3 entry
               
                if (curLevel->checkGate3Entry(player)) {
                    transitioning = true;
                    transitionClock.restart();
                    player->setPosition(900, 1430);

                    return;
                }
            }
            else {
                // Transition delay handling
                if (transitionClock.getElapsedTime().asSeconds() >= 0.5f) {
                    transitioning = false; 
                }
            }
        }
    }

    bool update() {
        static bool transitioning = false;  
        static sf::Clock transitionClock;   
        if (!curLevel) return false;
        if (!transitioning) {
            
            curLevel->moveEnemies(gameMap, player,guard);
            
            if (curLevel->checkGateEntry(player)) {
                transitioning = true;        
                transitionClock.restart();    
            }
        }
        else {
            if (transitionClock.getElapsedTime().asSeconds() >= 0.5f) { 
                loadNextLevel();        
                transitioning = false;  
                return true;    
            }
        }

        return false;
    }

    std::shared_ptr<Level> getCurLevel() {
        return curLevel;
    }
    int getCurLevelIndex() {
        return currentLevelIndex;
    }
    std::shared_ptr<Level> getLevel(size_t index) {
        if (index >= levels.size()) {
            throw std::out_of_range("Level index out of range!");
        }
        return levels[index];
    }
    int getSize() {
        return levels.size();
    }
    void render(sf::RenderWindow& window) {
        curLevel->render(window);
        if (playerInRoom) window.draw(doorSprite);
        

    }

    void updateDamageManager() {
        auto room = curLevel->getRoom(curLevel->getRoomIndex());
        if (damageManager) delete damageManager; 
        damageManager = new DamageManager(player, guard, room->getEnemies());
        player->setMediator(damageManager);
        if (guard) {
            guard->setMediator(damageManager);
        }
        room->setMed(damageManager);
    }
    ~GameLoop() {
        delete damageManager;
        cout << "DELETE MANAGER";
    }
};
