#include "Dialog.h"
#include <stdexcept>
#include <iostream>
Dialog::Dialog(const std::string& texturePath, const std::string& fontPath, sf::Vector2f position) {
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Failed to load dialog texture");
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);

    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Failed to load dialog font");
    }
    std::cout << fontPath << std::endl;
    text.setFont(font);
    text.setCharacterSize(12); // Adjust size as needed
    text.setFillColor(sf::Color::Black); // Set text color
    //text.setPosition(position.x + 10, position.y + 10); // Adjust text position within the dialog
}

void Dialog::setText(const std::string& message) {
    text.setString(message);

    // Get bounds of the dialog sprite
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    // Check if the text width exceeds the dialog width
    if (text.getGlobalBounds().width > spriteBounds.width - 20) { // Allow padding
        wrapText(message, spriteBounds.width - 20); // 10px padding on each side
    }

    // Align the text to start from the top-left corner of the dialog
    text.setPosition(sprite.getPosition().x + 15, sprite.getPosition().y + 10); // 10px padding from top and left
}



void Dialog::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    text.setPosition(x + 10, y + 10); // Keep text offset from the top-left corner
}

void Dialog::show() {
    visible = true;
}

void Dialog::hide() {
    visible = false;
}

bool Dialog::isShowing() const {
    return visible;
}

void Dialog::draw(sf::RenderWindow& window) const {
    if (visible) {
        window.draw(sprite);
        window.draw(text);
    }
}

void Dialog::wrapText(const std::string& message, float maxWidth) {
    std::string rawString = message;
    std::string wrappedString;

    float lineWidth = 0;
    std::string word;
    sf::Text tempText = text; // Use a temporary text to measure word width

    for (char c : rawString) {
        if (c == ' ' || c == '\n') {
            tempText.setString(word + c);
            float wordWidth = tempText.getLocalBounds().width;

            if (lineWidth + wordWidth > maxWidth) {
                wrappedString += '\n';
                lineWidth = 0;
            }

            wrappedString += word + c;
            lineWidth += wordWidth;
            word.clear();
        }
        else {
            word += c;
        }
    }

    // Add the last word
    if (!word.empty()) {
        tempText.setString(word);
        if (lineWidth + tempText.getLocalBounds().width > maxWidth) {
            wrappedString += '\n';
        }
        wrappedString += word;
    }

    text.setString(wrappedString);
}

