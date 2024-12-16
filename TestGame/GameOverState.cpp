#include "GameOverState.h"
#include <iostream>

GameOverState::GameOverState(const std::string& texturePath, Callback returnToMenu)
    : returnToMenu(returnToMenu) {
    if (!gameOverTexture.loadFromFile(texturePath)) {
        throw std::runtime_error("Failed to load Game Over texture.");
    }
    gameOverSprite.setTexture(gameOverTexture);

    // Scale the image to fit the screen
    float textureWidth = gameOverTexture.getSize().x;
    float textureHeight = gameOverTexture.getSize().y;
    float scaleX = 1500.0f / textureWidth;
    float scaleY = 1000.0f / textureHeight;
    gameOverSprite.setScale(scaleX, scaleY);
}

void GameOverState::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        returnToMenu(); // Go back to menu
        return; // Exit the event handling
    }
}

void GameOverState::update() {
    // No updates needed for the static Game Over screen
}

void GameOverState::draw(sf::RenderWindow& window) {
    // Center the game over sprite
    window.setView(window.getDefaultView());
    window.draw(gameOverSprite);
}
