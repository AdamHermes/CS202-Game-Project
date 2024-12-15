#pragma once
#include <SFML/Graphics.hpp>

class State {
public:
    virtual ~State() = default;
    virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};
