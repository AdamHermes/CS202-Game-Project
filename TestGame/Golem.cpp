#include "Golem.h"
#include "Character.h"
void Golem::fighting(int direction,  const string target) {
    const int frameWidth = 64;   // Width of a single frame
    const int frameHeight = 96;  // Height of a single frame
    const int totalFrames = 7;   // Number of frames per direction
    const int frameHeight2 = 256;
    sprite.setOrigin(32.0f, 64.0f);
    if (animationClock.getElapsedTime().asSeconds() > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;  // Loop attack frames
        animationClock.restart();
    }

    if (direction == Right) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Left) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Up) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
    }
    else if (direction == Down) {
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, frameHeight2 + direction * frameHeight, frameWidth, frameHeight));
    }
      // Centering the origin for better alignment

    
    if (attackCooldownClock.getElapsedTime().asSeconds() > attackCooldown) {


        if (target == "player") {
            manager->notify("EnemyAttackPlayer", 10);
        }

        else if (target == "guard") manager->notify("EnemyAttackGuard", 10);
        attackCooldownClock.restart();
    }
    


}
