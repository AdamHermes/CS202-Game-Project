#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "State.h"
#include "MenuState.h"
#include "GameState.h"
#include "GameOverState.h"
#include "ChooseCharacter.h"
class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    bool gameOverRequested = false;
    void transitionToMenu();
    void transitionToGame(int char_id);
    void transitionToChooseCharacter();
    void transitionToGameOver();
    std::unique_ptr<State> currentState;

};
