#include "Button.h"

// Constructor to initialize button properties
Button::Button(const std::string& textureFile, const sf::Vector2f& position) {
    // Load the button texture
    if (!buttonTexture.loadFromFile(textureFile)) {
        // Handle error if texture loading fails
        std::cout << "Fail to load Texture from file." << std::endl;
    }

    // Set the button sprite with the loaded texture
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setPosition(position);
}

// Function to render the button
void Button::render(sf::RenderWindow& window) {
    window.draw(buttonSprite);
}

// Function to check if the button is clicked
bool Button::isClicked(const sf::Vector2f& mousePosition) {
    return buttonSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}
