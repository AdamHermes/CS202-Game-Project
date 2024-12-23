#pragma once
#include <SFML/Graphics.hpp>
#include "Dialog.h"
#include <iostream>
#include <stdexcept>

enum class ItemType {
    health,
    speed,
    power,
    statue
};

class Items {
private:
    bool Removed = false;
    sf::Sprite sprite;
    sf::Texture texture;
    ItemType itemType;

    // Dialog only relevant if the item type is statue
    Dialog* dialog = nullptr;
    bool isHighlighted = false;
public:
    Items() = default;

    Items(ItemType type, const std::string& path, sf::Vector2f spawnPosition,
        const std::string& dialogText = "", const std::string& dialogTexturePath = "",
        const std::string& fontPath = "")
        : itemType(type)
    {
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load item texture");
        }
        else {
            std::cout << "Load Items success\n";
        }

        sprite.setPosition(spawnPosition);
        sprite.setTexture(texture);

        // If the item is a statue, initialize the dialog
        if (type == ItemType::statue) {
            if (dialogTexturePath.empty() || fontPath.empty()) {
                throw std::runtime_error("Missing dialog texture or font path for statue");
            }
            dialog = new Dialog(dialogTexturePath, fontPath, sf::Vector2f(spawnPosition.x + 10, spawnPosition.y - 60));
            dialog->setText(dialogText);
            //removed();
        }
    }
    void highlight() {
        sprite.setColor(sf::Color(255, 255, 128)); // Yellowish color
        isHighlighted = true;
    }

    // Remove the highlight
    void removeHighlight() {
        sprite.setColor(sf::Color(255, 255, 255)); // Reset to normal color
        isHighlighted = false;
    }
    bool getHighlightState() const {
        return isHighlighted;
    }
    ~Items() {
        if (dialog) {
            delete dialog;
        }
    }

    ItemType getType() const { return itemType; }

    sf::Sprite& getSprite() { return sprite; }

    void removed() {
        Removed = true;
    }

    bool isRemoved() const { return Removed; }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
        if (dialog) {
            dialog->setPosition(x, y - 80); // Adjust dialog position if it's a statue
        }
    }

    void drawTo(sf::RenderWindow& window) const {
        if (!Removed) {
            window.draw(sprite);
            if (dialog && dialog->isShowing()) {
                dialog->draw(window);
            }
        }
    }

    // Statue-specific dialog functions
    void showDialog() {
        if (dialog) {
            dialog->show();
        }
    }

    void hideDialog() {
        if (dialog) {
            dialog->hide();
        }
    }

    bool isDialogVisible() const {
        return dialog && dialog->isShowing();
    }
};
