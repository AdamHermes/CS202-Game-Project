#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <functional>

class GameOverState : public State {
public:
    using Callback = std::function<void()>;
    GameOverState(const std::string& texturePath, Callback returnToMenu);

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Texture gameOverTexture;
    sf::Sprite gameOverSprite;
    Callback returnToMenu;
};

#endif // GAMEOVERSTATE_H
