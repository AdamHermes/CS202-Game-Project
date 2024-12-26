#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Button.h"
#include "State.h"
#include "AudioManager.h"
class MenuState : public State {
public:
    using Callback = std::function<void()>;
    MenuState(const std::string& backgroundTextureFile, Callback startGameCallback);
    AudioManager& audioManager;
    std::string currentMusicFile;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite backgroundSprite;   
    sf::Texture backgroundTexture;  
    std::vector<std::shared_ptr<Button>> buttons;    
    Callback chooseCharacterCallback;
};

#endif // MENUSTATE_H
