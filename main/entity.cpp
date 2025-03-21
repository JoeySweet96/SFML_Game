#include "../include/entity.hpp"

Entity::Entity(const sf::Texture& texturePath, sf::Vector2f position, sf::Vector2f scale)
	: sprite(texturePath)
{
	sprite.setPosition(position);
	sprite.setScale(scale);
}
