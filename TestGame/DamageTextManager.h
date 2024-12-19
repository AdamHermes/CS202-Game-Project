#pragma once
#include <SFML/Graphics.hpp>
#include "Manager.h"
#include "Character.h"
#include "Enemy.h"
class DamageTextManager : public Manager {
public:
    void notify(float value, const sf::Vector2f& position) {
        sf::Text damageText = templateText;
        damageText.setString(std::to_string(static_cast<int>(value)));
        damageText.setPosition(position.x, position.y - 20);
        damageTexts.push_back(damageText);
    }
    
    void update(float deltaTime) {
        
        for (auto it = damageTexts.begin(); it != damageTexts.end(); ) {
            it->move(0, -1 * deltaTime); // Move up
            sf::Color color = it->getFillColor();
            if (color.a > 0) {
                color.a -= 1;
                it->setFillColor(color);
                ++it;
            }
            else {
                it = damageTexts.erase(it);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& text : damageTexts) {
            window.draw(text);
        }

    }

    void setTemplate(const sf::Text& text) {
        templateText = text;
    }

private:
    sf::Text templateText;
    std::vector<sf::Text> damageTexts;
};
