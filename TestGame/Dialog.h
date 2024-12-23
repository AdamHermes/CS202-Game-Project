#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Dialog {
private:
    sf::Sprite sprite;            // Dialog background
    sf::Texture texture;          // Dialog background texture
    sf::Text text;                // Text displayed on the dialog
    sf::Font font;                // Font used for the text
    bool visible = false;         // Controls visibility of the dialog

public:
    Dialog(const std::string& texturePath, const std::string& fontPath, sf::Vector2f position);

    void setText(const std::string& message);
    void setPosition(float x, float y);

    void show();
    void hide();
    bool isShowing() const;
    void wrapText(const std::string& message, float maxWidth);
    void draw(sf::RenderWindow& window) const;
};
