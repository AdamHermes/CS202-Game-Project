#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Button.h"
#include "State.h"
class MenuState : public State {
public:
    using Callback = std::function<void()>;
    MenuState(const std::string& backgroundTextureFile, Callback startGameCallback);

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Sprite backgroundSprite;   
    sf::Texture backgroundTexture;  
    std::vector<std::shared_ptr<Button>> buttons;    
    Callback startGameCallback;
};

#endif // MENUSTATE_H
