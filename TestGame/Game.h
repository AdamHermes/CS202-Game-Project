#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "State.h"
#include "MenuState.h"
#include "GameState.h"
class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    std::unique_ptr<State> currentState;

};
