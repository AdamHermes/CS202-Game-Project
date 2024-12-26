#include "ChooseCharacter.h"
#include "Button.h"
#include <iostream>

ChooseCharacterState::ChooseCharacterState(const std::string& backgroundTextureFile, Callback startGameCallback) :
    audioManager(AudioManager::getInstance()), currentMusicFile("../Assets/SoundTrack/menu.mp3") {
    std::cout << "GOT HERE" << std::endl;
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
    buttons.push_back(std::make_shared<Button>("../Assets/Menu/char1.png", sf::Vector2f(200,250 )));
    buttons.push_back(std::make_shared<Button>("../Assets/Menu/char2.png", sf::Vector2f(600, 250)));
    buttons.push_back(std::make_shared<Button>("../Assets/Menu/char3.png", sf::Vector2f(1000, 250)));
    this->startGameCallback = startGameCallback;
}

void ChooseCharacterState::draw(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(backgroundSprite);

    for (auto& button : buttons) {
        button->render(window);
    }
}
void ChooseCharacterState::update() {

}
void ChooseCharacterState::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);


        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i]->isClicked(static_cast<sf::Vector2f>(mousePos))) {
                int char_id = static_cast<int>(i + 1);
                startGameCallback(char_id); 
                break;
            }
        }
    }
}

