#pragma once
#include <SFML/Graphics.hpp>
class ArrowProjectile {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;

    float distanceTraveled = 0.0f;

    ArrowProjectile(sf::Sprite& arrowSprite, sf::Vector2f initVelocity)
        : sprite(arrowSprite), velocity(initVelocity) {}

    void update(float deltaTime) {
        sprite.move(velocity * deltaTime);
        distanceTraveled += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * deltaTime;
    }

    void reset(sf::Vector2f position, sf::Vector2f velocity) {
        sprite.setPosition(position);
        this->velocity = velocity;
        distanceTraveled = 0.0f;
    }
};