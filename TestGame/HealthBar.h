#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class HealthBar {
private:
    sf::Texture healthTexture; // Holds the health bar sprite sheet
    sf::Sprite healthBarSprite;
    int frameWidth = 300;  // Width of each frame
    int frameHeight = 40; // Height of each frame
    int totalFrames = 11; // Total number of health states
    sf::Clock shakeTimer;           // Timer to track shake duration
    float shakeDuration = 0.3f;     // Duration of the shake effect
    float shakeAmount = 5.f;        // Maximum shake distance in pixels
    bool isShaking = false;
public:
    HealthBar(const std::string& texturePath, float x, float y) {
        if (!healthTexture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load " << texturePath << "!" << std::endl;
        }
        healthBarSprite.setTexture(healthTexture);
        healthBarSprite.setPosition(x, y); // Set initial position
        healthBarSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight)); // Start with full health
    }

    void update(int health) {
        // Calculate the frame index based on health
        int frameIndex = totalFrames - 1 - (health / 10); // Reverse the frame order
        if (frameIndex < 0) frameIndex = 0;                     // Clamp to first frame
        if (frameIndex >= totalFrames) frameIndex = totalFrames - 1; // Clamp to last frame

        // Set the texture rect to display the correct frame
        
        healthBarSprite.setTextureRect(sf::IntRect(0, frameIndex * frameHeight, frameWidth, frameHeight));
        
        //}
    }

    void startShake() {
        isShaking = true;
        shakeTimer.restart();
    }
    void stopShake() {
        if (isShaking) {
            if (shakeTimer.getElapsedTime().asSeconds() >= shakeDuration) {
                // Shake duration is over, reset position
                isShaking = false;
                shakeTimer.restart();
                healthBarSprite.setPosition(healthBarSprite.getPosition());
                shakeTimer.restart();// Reset to original position
            }
        }

    }
    void setPosition(float x, float y) {
        healthBarSprite.setPosition(x, y);
    }

    
    void updatePosition(const sf::View& cameraView) {
        // Calculate the base position of the health bar relative to the camera view
        float offsetX = 10.0f; // X offset from the left edge
        float offsetY = 10.0f; // Y offset from the top edge
        sf::Vector2f viewCenter = cameraView.getCenter();
        sf::Vector2f viewSize = cameraView.getSize();

        // Top-left corner relative to the view
        float x = viewCenter.x - (viewSize.x / 2) + offsetX;
        float y = viewCenter.y - (viewSize.y / 2) + offsetY;

        // Apply shake if the health bar is shaking
        if (isShaking) {
            float shakeOffsetX = (rand() % static_cast<int>(shakeAmount * 2)) - shakeAmount;
            float shakeOffsetY = (rand() % static_cast<int>(shakeAmount * 2)) - shakeAmount;

            x += shakeOffsetX; // Apply shake to the x position
            y += shakeOffsetY; // Apply shake to the y position
        }

        // Set the health bar sprite position with or without shake
        healthBarSprite.setPosition(x, y);
    }


    sf::Sprite getSprite() const {
        return healthBarSprite;
    }
};

