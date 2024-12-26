#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "State.h"
#include <functional>
#include "Character.h"
#include "Camera.h"
#include "Map.h"
#include "GameLoop.h"
class GameState : public State {
public:
    using Callback = std::function<void()>;
    GameState(int char_id, Callback gameOverCallback, Callback gameWinCallback);
    
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    WeaponType type;
    int char_id;
private:
    std::shared_ptr<Character> player; // Player character
    std::shared_ptr<Character> guard;
    std::shared_ptr<Map> gameMap;      // Game map
    std::unique_ptr<GameLoop> gameLoop;
    std::shared_ptr<Level> level;
    std::shared_ptr<Room> room;
    bool used = false;
    Camera camera;                     // Camera for the game
    bool isFighting;
    int num = 2;// Fighting state
    sf::Clock stayClock;               // Clock to track time spent at target location
    bool playerAtLocation = false;     // Whether player is at the target location
    const float requiredTimeAtLocation = 4.0f;  // Time threshold to trigger game win
    sf::Vector2f targetLocation;       // Target position to check
    float locationTolerance = 50.0f;
    Callback gameOverCallback;
    Callback gameWinCallback;
};

#endif // GAMESTATE_H
