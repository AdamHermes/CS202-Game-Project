#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "Weapon.h"
#include "GameEntity.h"
#include "HealthBar.h"
#include "Camera.h"
#include <tuple>
using namespace std;
class Enemy;
class Character : public GameEntity {
private:
    float health = 100;
    bool alive;
    bool isShooting = false; 

    vector<std::shared_ptr<Weapon>> equippedWeapons;
    std::shared_ptr<Weapon> curWeapon;
    bool isMoving = false;
    sf::Sprite sprite;
    bool isFighting;
    sf::Texture texture;
    bool isHiden = false;
    int currentFrame = 0;
    float frameDuration = 0.1f;
    sf::Clock animationClock;
    enum Direction {
        Up = 0,
        Down = 2,
        Left = 1,
        Right = 3
    };
    HealthBar healthBar;
    sf::Sprite healthsprite;
    sf::Texture healthtexture;
    sf::Sprite arrowSprite;
    sf::Texture arrowTexture;
    sf::Clock attackCooldownClock;  
    std::vector<std::tuple<sf::Sprite, sf::Vector2f, float>> vectorArrow;
    float attackCooldown = 0.5f;
    sf::Clock damageFlashTimer;
    sf::Clock healingTimer;  // Timer to control healing over time

public:
    Character(const std::string& characterTexturePath, const std::string& healthTexturePath)
        : healthBar(healthTexturePath, 300,1200 ), health(100) { 
        loadTexture(characterTexturePath, false, 2, 240, 310);
        
    }
    std::shared_ptr<Weapon> getWeapon(int num) {
        return equippedWeapons[num];
    }
    std::shared_ptr<Weapon> getCurWeapon() {
        return curWeapon;
    }
    void takePortions();
    void setCurWeapon(const std::shared_ptr<Weapon>& weapon) {
        curWeapon = weapon;
    }
    bool hit = false;
    void setShooting(bool shooting);
    void takeDamage(float damage) {
        health -= damage;
        if (health > 0) {
            healthBar.startShake();

            sprite.setColor(sf::Color(255, 0, 0, 128));
            damageFlashTimer.restart();

            cout << health;
        }        
        else if (health <= 0) {
            health = 0;
            cout << "Player dead" << endl;
            alive = false;
        }

    }
    bool getisFighting() {
        return isFighting;
    }
    sf::FloatRect boundingBox;
    sf::FloatRect attackRangeBox;
    float offsetX = (64 - 33) / 2;
    float offsetY = (64 - 52);
    sf::Sprite getSprite() {
        return sprite;
    }

    void updateBoundingBox() {
        sf::Vector2f position = sprite.getPosition();

        // Adjust offsets based on texture size
        if (sprite.getTextureRect().width == 192) {
            // Fighting sprite
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX, position.y - 32.0f + offsetY,33, 52);
        }
        else if (sprite.getTextureRect().width == 64) {
            // Walking sprite
            boundingBox = sf::FloatRect(position.x - 32.0f + offsetX,position.y - 32.0f + offsetY, 33, 52);
        }
        
    }
    /*void showHealthBar() {
        string path = "../Assets/Character/Textures/health_bar.png";
        if (healthtexture.loadFromFile(path)) {
            healthsprite.setTexture(healthtexture);
            healthsprite.setTextureRect(sf::IntRect(0, 847, 468, ));
        }
        else {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
    }*/
    bool checkCollision(const sf::FloatRect& otherBox) const;
    void equipWeapon(WeaponType type);
    void changePos(int direction);
    void resetAnimation() {
        currentFrame = 0;
    }
    void handleMovement(Map& gameMap, const std::vector<std::shared_ptr<Enemy>>& enemies, int& num, bool& isMoving);
    void fight(int direction);
    void fightBow(int direction, const std::vector<std::shared_ptr<Enemy>>& enemies);
    void fightSword(int direction, const std::vector<std::shared_ptr<Enemy>>& enemies);
    void loadTexture(const std::string& path, bool isBig, int num, float x, float y);
    void drawTo(sf::RenderWindow& window) const;   
    void hide() {
        isHiden = true;
    }
    void reveal() {
        isHiden = false;
    }
    void updateSpriteHealth(const Camera& camera);
    void drawBoundingBox(sf::RenderWindow& window) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        boundingBoxShape.setOutlineColor(sf::Color::Red);  // Red outline for visibility
        boundingBoxShape.setOutlineThickness(2);  // Thickness of the outline

        window.draw(boundingBoxShape);  // Draw the bounding box on the window
    }
    void updateState(bool fighting, int num, WeaponType weaponType);
    void adjustPositionForNewSize(int oldWidth, int oldHeight, int newWidth, int newHeight);
};


