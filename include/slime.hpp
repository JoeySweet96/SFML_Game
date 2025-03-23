#pragma once
#include "entity.hpp"


class Slime: public Entity {
public:
	sf::Vector2f velocity;
	sf::FloatRect hitBox;
	int health;
	bool isHit;
	int timer;
public:
	Slime(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin);
	void update(float gravity, float playerPos);
	void takeDamage(float deltaTime, sf::Texture& texture, sf::Texture& hurtTexture);
	void respawn(int& counter);
};
