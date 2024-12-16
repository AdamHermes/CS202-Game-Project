#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "State.h"
#include "MenuState.h"
#include "GameState.h"
#include "GameOverState.h"
class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    bool gameOverRequested = false;
    void transitionToMenu();
    void transitionToGame();
    void transitionToGameOver();
    std::unique_ptr<State> currentState;

};
