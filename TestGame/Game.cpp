#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(1500, 1000), "Game Menu") {
    transitionToMenu();
}

void Game::transitionToMenu() {
    currentState = std::make_unique<MenuState>("../Assets/Menu/Menu.png", [this]() {
        transitionToChooseCharacter();
        });
}

void Game::transitionToChooseCharacter() {
    currentState = std::make_unique<ChooseCharacterState>(
        "../Assets/Menu/chooseChar.png",
        [this](int char_id) { 
            transitionToGame(char_id);
        }
    );
}
void Game::transitionToGame(int char_id) {
    currentState = std::make_unique<GameState>(
        char_id,
        [this]() { transitionToGameOver(); },  
        [this]() { transitionToMenu(); }      
    );
}

void Game::transitionToGameOver() {
    currentState = std::make_unique<GameOverState>("../Assets/Menu/gameOver.jpg", [this]() {
        transitionToMenu();
        });
}
void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
        }
        currentState->handleEvent(event, window);
        currentState->update();
        


        window.clear();
        currentState->draw(window);
        window.display();
    }
}
