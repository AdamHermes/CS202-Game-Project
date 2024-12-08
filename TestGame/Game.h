#pragma once
#include "Level.h"
#include "DamageManager.h"
#include "Map.h"
class Game {
private:
    std::vector<std::shared_ptr<Level>> levels;
    std::shared_ptr<Level> curLevel;
    Character& player;
    Map& gameMap;
    int currentLevelIndex = 0;                // Index of the current level
    DamageManager* damageManager;
    void cleanupLevel() {
        curLevel.reset(); // Release the current level
    }
public:
    Game(Character& player, Map& gameMap) : player(player), gameMap(gameMap) {};
    void addLevel(const sf::FloatRect& gatePosition) {
        auto newLevel = std::make_shared<Level>();
        newLevel->setGate(gatePosition); // Set the gate position in the level
        levels.push_back(newLevel);
    }

    void addLevels() {
        sf::FloatRect gatePositionLevel1(200.0f, 300.0f, 100.0f, 50.0f);  // Gate for level 1
        sf::FloatRect gatePositionLevel2(800.0f, 1000.0f, 50.0f, 50.0f);  // Gate for level 2

        // Add levels with corresponding gate positions
        addLevel(gatePositionLevel1);  // Level 1
        addLevel(gatePositionLevel2);  // Level 2
    }
    void loadEnemiesForRooms(int currentLevelIndex) {
        curLevel = levels[currentLevelIndex];
        curLevel->populateEnemies(currentLevelIndex);
    }
    void loadNextLevel() {
        if (currentLevelIndex + 1 < levels.size()) {
            cleanupLevel();
            currentLevelIndex++;
            curLevel = levels[currentLevelIndex];
            gameMap.obstacles.clear();
            if (damageManager) {
                delete damageManager;
                damageManager = nullptr; // Avoid dangling pointers
            }
            // Load the new map
            std::string texturePath = "../Assets/Character/Textures/map" + std::to_string(currentLevelIndex) + ".png";
            std::string tmxPath = "../Assets/Character/TMX MAP/map" + std::to_string(currentLevelIndex) + ".tmx";

            if (!gameMap.loadTexture(texturePath) || !gameMap.loadFromTMX(tmxPath)) {
                std::cerr << "Failed to load map for level " << currentLevelIndex + 1 << std::endl;
                return;
            }
            player.loadTexture("../Assets/Character/Textures/characters.png", false, 2, 760, 450);
            //player.updateBoundingBox();
            //player.equipWeapon(WeaponType::Sword);
            std::cout << "Loaded map for level " << currentLevelIndex + 1 << std::endl;
            cout << "Cur Index " << currentLevelIndex << endl;
            loadEnemiesForRooms(currentLevelIndex);
            updateDamageManager();
        }
        else {
            std::cout << "You have completed the game!" << std::endl;
        }
    }
    bool update() {
        if(!curLevel) return false;
        curLevel->moveEnemies(gameMap, player);
        if (curLevel->checkGateEntry(player)) {
            loadNextLevel();
            return true;
        }
        return false;
    }
    std::shared_ptr<Level> getCurLevel() {
        return curLevel;
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
    }

    void updateDamageManager() {
        auto room = curLevel->getRoom(0);
        if (damageManager) delete damageManager; // Clean up old manager
        damageManager = new DamageManager(player, room->getEnemies() );
        player.setMediator(damageManager);
        room->setMed(damageManager);
    }
};
