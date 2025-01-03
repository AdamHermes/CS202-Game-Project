#include "MenuState.h"
#include "Button.h"
#include <iostream>

MenuState::MenuState(const std::string& backgroundTextureFile, Callback chooseCharacterCallback) :
    audioManager(AudioManager::getInstance()), currentMusicFile("../Assets/SoundTrack/menu.mp3") {
    if (!backgroundTexture.loadFromFile(backgroundTextureFile)) {
        std::cerr << "Error loading background texture from file: " << backgroundTextureFile << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);

    float textureWidth = backgroundTexture.getSize().x;
    float textureHeight = backgroundTexture.getSize().y;


    float scaleX = 1500.0f / textureWidth;  
    float scaleY = 1000.0f / textureHeight; 

    backgroundSprite.setScale(scaleX, scaleY);
    audioManager.playMusic(currentMusicFile, true);
    buttons.push_back(std::make_shared<Button>("../Assets/Menu/play_button.png", sf::Vector2f(600, 550)));
    this->chooseCharacterCallback = chooseCharacterCallback;
}

void MenuState::draw(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(backgroundSprite);

    for (auto& button : buttons) {
        button->render(window);
    }
}
void MenuState::update() {

}
void MenuState::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        for (auto& button : buttons) {
            if (button->isClicked(static_cast<sf::Vector2f>(mousePos))) {
                chooseCharacterCallback();
                break;
            }
        }
    }
}
