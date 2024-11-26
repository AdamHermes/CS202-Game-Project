#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    view.setSize(screenWidth, screenHeight);
}

void Camera::update(const sf::Vector2f& playerPosition) {
    view.setCenter(playerPosition);

    // Apply boundaries if worldBounds are set
    if (worldBounds.width > 0 && worldBounds.height > 0) {
        float leftBound = worldBounds.left + screenWidth / 2;
        float rightBound = worldBounds.left + worldBounds.width - screenWidth / 2;
        float topBound = worldBounds.top + screenHeight / 2;
        float bottomBound = worldBounds.top + worldBounds.height - screenHeight / 2;

        sf::Vector2f clampedPosition = view.getCenter();
        if (clampedPosition.x < leftBound) clampedPosition.x = leftBound;
        if (clampedPosition.x > rightBound) clampedPosition.x = rightBound;
        if (clampedPosition.y < topBound) clampedPosition.y = topBound;
        if (clampedPosition.y > bottomBound) clampedPosition.y = bottomBound;

        view.setCenter(clampedPosition);
    }
}

void Camera::applyView(sf::RenderWindow& window) {
    window.setView(view);
}

void Camera::setWorldBounds(const sf::FloatRect& bounds) {
    worldBounds = bounds;
}

// Adjust zoom level of the view
void Camera::setZoom(float zoomLevel) {
    view.zoom(zoomLevel);
}
