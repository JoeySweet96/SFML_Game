#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Entity {
public:
	Entity(const sf::Texture& texturePath, sf::Vector2f position, sf::Vector2f scale);
	virtual ~Entity() = default;
public:
	sf::Sprite sprite;
};
