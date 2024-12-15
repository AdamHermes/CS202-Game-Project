#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
public:
    // Constructor to initialize button properties
    Button(const std::string& textureFile, const sf::Vector2f& position);

    // Function to render the button
    void render(sf::RenderWindow& window);

    bool isClicked(const sf::Vector2f& mousePosition);

private:
    sf::Sprite buttonSprite;
    sf::Texture buttonTexture;
};

#endif // BUTTON_H
