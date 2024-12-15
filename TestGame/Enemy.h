#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "GameEntity.h"
class Character;
enum class EnemyType {
    Frogman,
    Goblin,
    Demon,
    Golem,
    Sunflower,
    Spider,
    Dragon,
    Skeleton,
    Boarman
};
enum class EnemyState {
    Moving,
    Fighting
};
class Enemy : public GameEntity {
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
    float opacity = 255.0f;
    enum Direction {
        Up = 1,
        Down = 0,
        Left = 3,
        Right = 2
    };
    sf::Clock attackCooldownClock;
    sf::Clock damageFlashTimer;
    float attackCooldown = 0.0f;
    float patrolAngle = 0.0f;
    float directionCooldown = 0.0f;
    bool removed = false;
    float speed = 0.01f;
public:
    
    bool isRemoved() const {
        return removed;
    }
    EnemyType enemyType;
    void setState(EnemyState newState) { state = newState; }
    EnemyState getState() const { return state; }
    int getHealth() {
        return health;
    }
    sf::FloatRect boundingBox;
    float offsetX = (64 - 32) / 2;
    float offsetY = 4;
    void takeDamage(float damage) {
        health -= damage;
        sprite.setColor(sf::Color(255, 0, 0 , 128));// Flash red
        damageFlashTimer.restart();  // Restart the flash timer
        if (health <= 0) {
            alive = false;
        }
    }
    Enemy(EnemyType type) : alive(true), enemyType(type), health(0) {}
    void updateBoundingBox() {
        if (!alive) {
            // Set bounding box to an invalid area when enemy is dead
            boundingBox = sf::FloatRect(0, 0, 0, 0);
            return;
        }
        sf::Vector2f position = sprite.getPosition();
        if (enemyType == EnemyType::Demon) {
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX + 12.0f, position.y - 32.0f + offsetY + 20.0f, 8, 16);
        }
        else if (enemyType == EnemyType::Frogman) {
            boundingBox = sf::FloatRect(position.x - 24.0f + offsetX, position.y  -4.0f + offsetY, 16, 12);
        }
        else if (enemyType == EnemyType::Sunflower) {
            boundingBox = sf::FloatRect(position.x - 24.0f, position.y -36.0f+ offsetY, 48, 64);

        }
        else if (enemyType == EnemyType::Dragon) {
            boundingBox = sf::FloatRect(position.x - 52.0f, position.y - 32.0f, 104, 64);
        }
        else {
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX, position.y - 32.0f + offsetY, 32, 56);

        }

    }
    bool checkCollision(const sf::FloatRect& otherBox) const {
        return boundingBox.intersects(otherBox);
    }
    void updateDead();
    void changePos(int direction);
    void loadTexture(std::string filename, float x, float y);
    void drawTo(sf::RenderWindow& window) const;
    void handleMovement(std::shared_ptr<Map> gameMap, std::shared_ptr<Character> player);
    void drawBoundingBox(sf::RenderWindow& window) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        boundingBoxShape.setOutlineColor(sf::Color::Red);  // Red outline for visibility
        boundingBoxShape.setOutlineThickness(2);  // Thickness of the outline

        window.draw(boundingBoxShape);  // Draw the bounding box on the window
    }
    //void randomPatrol(Map& gameMap);
    virtual ~Enemy() = default;
    virtual void fighting(int direction, std::shared_ptr<Character>& player) = 0;

};
