#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"

class Character;
enum class EnemyType {
    Goblin,
    Demon,
    Golem
};
enum class EnemyState {
    Moving,
    Fighting
};
class Enemy {
protected:
    EnemyState state = EnemyState::Moving;
    float health;
    bool alive;
    sf::Sprite sprite;
    sf::Texture texture;
    bool isHiden = false;
    int totalFrames = 0;
    int currentFrame = 0;
    float frameDuration = 0.0f;
    sf::Clock animationClock;

    enum Direction {
        Up = 1,
        Down = 0,
        Left = 3,
        Right = 2
    };
    sf::Clock attackCooldownClock;
    float attackCooldown = 0.0f;
    float patrolAngle = 0.0f;
public:
    EnemyType enemyType;
    void setState(EnemyState newState) { state = newState; }
    EnemyState getState() const { return state; }
    int getHealth() {
        return health;
    }
    sf::FloatRect boundingBox;
    float offsetX = (64 - 32) / 2;
    float offsetY = 0;
    void takeDamage(float damage) {
        health -= damage;
        cout << "Dam" << health;
        if (health <= 0) {
            alive = false;
        }
    }
    Enemy(EnemyType type) : alive(true), enemyType(type) {}
    void updateBoundingBox() {
        sf::Vector2f position = sprite.getPosition();
        if (enemyType == EnemyType::Demon) {
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX + 12.0f, position.y - 32.0f + offsetY + 24.0f, 8, 16);
        }
        else {
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX, position.y - 32.0f + offsetY, 32, 64);

        }
    }
    bool checkCollision(const sf::FloatRect& otherBox) const {
        return boundingBox.intersects(otherBox);
    }
    void changePos(int direction);
    void loadTexture(std::string filename, float x, float y);
    void drawTo(sf::RenderWindow& window) const;
    void handleMovement(Map& gameMap, Character& player);
    void drawBoundingBox(sf::RenderWindow& window) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        boundingBoxShape.setOutlineColor(sf::Color::Red);  // Red outline for visibility
        boundingBoxShape.setOutlineThickness(2);  // Thickness of the outline

        window.draw(boundingBoxShape);  // Draw the bounding box on the window
    }
    void randomPatrol(Map& gameMap);
    virtual ~Enemy() = default;
    virtual void fighting(int direction, Character& player) = 0;

};
