#pragma once
#include <SFML/Graphics.hpp>
enum class ItemType {
	health,
	speed,
	power
};
class Items {
private:
	bool Removed = false;
	sf::Sprite sprite;
	sf::Texture texture;
	ItemType itemType;
public:
	Items(ItemType type, const std::string& path, sf::Vector2f spawnPosition) : itemType(type){
		if (!texture.loadFromFile(path)) {
			throw std::runtime_error("Failed to load item texture");

		}
		else std::cout << "Load Items success";
		sprite.setPosition(spawnPosition);
		sprite.setTexture(texture);
	}
	~Items() = default;
	ItemType getType() const { return itemType; }
	sf::Sprite& getSprite() { return sprite; }
	void removed() {
		Removed = true;
	}
	bool isRemoved() { return Removed; }
	void setPosition(float x, float y) { sprite.setPosition(x, y); }
	void drawTo(sf::RenderWindow& window) const {
		if (!Removed) window.draw(sprite);
	}

};