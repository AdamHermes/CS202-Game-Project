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
    GameState(Callback gameOverCallback);
    
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

private:
    std::shared_ptr<Character> player; // Player character
    std::shared_ptr<Character> guard;
    std::shared_ptr<Map> gameMap;      // Game map
    std::unique_ptr<GameLoop> gameLoop;
    std::shared_ptr<Level> level;
    std::shared_ptr<Room> room;
    Camera camera;                     // Camera for the game
    bool isFighting;
    int num = 2;// Fighting state
    Callback gameOverCallback;
};

#endif // GAMESTATE_H
