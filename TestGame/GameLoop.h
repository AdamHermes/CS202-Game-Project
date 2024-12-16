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
public:
    void drawBoundingBox(sf::RenderWindow& window, sf::FloatRect boundingBox) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        boundingBoxShape.setOutlineColor(sf::Color::Red);  // Red outline for visibility
        boundingBoxShape.setOutlineThickness(2);  // Thickness of the outline

        window.draw(boundingBoxShape);  // Draw the bounding box on the window
    }
    GameLoop(std::shared_ptr<Character> player, std::shared_ptr<Map> gameMap, std::shared_ptr<Character> guard)
        : player(player), gameMap(gameMap), guard(guard) {
        
    }   

    void addLevel(const sf::FloatRect& gatePosition) {
        auto newLevel = std::make_shared<Level>();
        newLevel->setGate(gatePosition); // Set the gate position in the level
        levels.push_back(newLevel);
    }

    void addLevels() {
        sf::FloatRect gatePositionLevel1(200.0f, 300.0f, 100.0f, 50.0f);  // Gate for level 1
        sf::FloatRect gatePositionLevel2(1030.0f, 1330.0f, 100.0f, 50.0f);  // Gate for level 2
        sf::FloatRect gatePositionLevel3(0, 0, 0, 0);
        // Add levels with corresponding gate positions
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
        }

        curLevel->populateEnemies(currentLevelIndex);
    }
    void loadItemsForRooms(int currentLevelIndex) {
        if (currentLevelIndex >= 1) {
            std::shared_ptr<Level> prevLevel = levels[currentLevelIndex - 1];
            prevLevel->getRoom(0)->clearItems();
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
                damageManager = nullptr; // Avoid dangling pointers
            }
            // Load the new map
            std::string texturePath = "../Assets/Character/Textures/map" + std::to_string(currentLevelIndex) + ".png";
            std::string tmxPath = "../Assets/Character/TMX MAP/map" + std::to_string(currentLevelIndex) + ".tmx";

            if (!gameMap->loadTexture(texturePath) || !gameMap->loadFromTMX(tmxPath)) {
                std::cerr << "Failed to load map for level " << currentLevelIndex + 1 << std::endl;
                return;
            }
            if (currentLevelIndex == 1) {
                player->loadTexture("../Assets/Character/Textures/character1.png", false, 2, 1060, 1400); //1300 260

            }
            else if (currentLevelIndex == 2) {
                player->loadTexture("../Assets/Character/Textures/character1.png", false, 2, 160, 950);
                guard->loadTexture("../Assets/Character/Textures/characters.png", false, 2, 240, 1000);
                guard->updateBoundingBox();
                guard->equipWeapon(WeaponType::Bow);
                auto start_weapon_guard = guard->getWeapon(0);
                guard->setCurWeapon(start_weapon_guard);

            }
            //player.updateBoundingBox();
            //player.equipWeapon(WeaponType::Sword);
            std::cout << "Loaded map for level " << currentLevelIndex + 1 << std::endl;
            cout << "Cur Index " << currentLevelIndex << endl;
            loadEnemiesForRooms(currentLevelIndex);
            loadItemsForRooms(currentLevelIndex);
            updateDamageManager();
        }
        else {
            std::cout << "You have completed the game!" << std::endl;
        }
    }
    bool update() {
        static bool transitioning = false;     // Flag to check if transition is in progress
        static sf::Clock transitionClock;     // Clock to measure delay duration

        if (!curLevel) return false;

        if (!transitioning) {
            curLevel->moveEnemies(gameMap, player,guard);

            // Check if player is entering the gate
            if (curLevel->checkGateEntry(player)) {
                transitioning = true;         // Start transition
                transitionClock.restart();    // Reset the clock for delay
                             // Signal transition has started
            }
        }
        else {
            // Wait for a short delay before loading the next level
            if (transitionClock.getElapsedTime().asSeconds() >= 0.5f) { // 1-second delay
                loadNextLevel();             // Load the next level
                transitioning = false;  
                return true;     // Reset the transition flag
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
        auto gate = curLevel->getGate();
        drawBoundingBox(window, gate->getBoundingBox());
    }

    void updateDamageManager() {
        auto room = curLevel->getRoom(0);
        if (damageManager) delete damageManager; // Clean up old manager
        damageManager = new DamageManager(player, guard, room->getEnemies());
        player->setMediator(damageManager);
        guard->setMediator(damageManager);
        room->setMed(damageManager);
    }
};
