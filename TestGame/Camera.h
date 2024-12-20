#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);
    ~Camera() = default;
    // Update camera position based on player
    void update(const sf::Vector2f& playerPosition,bool isAlive);

    // Apply the camera view to the window
    void applyView(sf::RenderWindow& window);

    // Set camera boundaries based on world size
    void setWorldBounds(const sf::FloatRect& bounds);

    // Set zoom level to make the view smaller or larger
    void setZoom(float zoomLevel);
    void setCenter(const sf::Vector2f& position);
    const sf::View& getView() const;
private:
    sf::View view;
    sf::FloatRect worldBounds;
    float screenWidth, screenHeight;
};

#endif // CAMERA_H
