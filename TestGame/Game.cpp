#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(1500, 1000), "Game Menu") {
    currentState = std::make_unique<MenuState>("../Assets/Menu/Menu.png", [this]() {

        currentState = std::make_unique<GameState>();
        });
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            //    //currentState->handleEvent(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
            //}
        }
        currentState->handleEvent(event, window);
        currentState->update();
        window.clear();
        currentState->draw(window);
        window.display();
    }
}
