#ifndef CHOOSECHARACTERSTATE_H
#define CHOOSECHARACTERSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Button.h"
#include "State.h"
#include "AudioManager.h"
class ChooseCharacterState : public State {
public:
    using Callback = std::function<void(int)>; 

    ChooseCharacterState(const std::string& backgroundTextureFile, Callback startGameCallback);
    AudioManager& audioManager;
    std::string currentMusicFile;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    std::vector<std::shared_ptr<Button>> buttons;
    Callback startGameCallback;
};

#endif // CHOOSECHARACTER_
