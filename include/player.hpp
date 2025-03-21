#pragma once
#include "entity.hpp"


class Player : public Entity {
public:
	sf::Vector2f velocity;
	sf::Vector2f attackPoint;
	sf::Vector2f hitpointLeft;
	sf::Vector2f hitpointRight;
	int health;
	bool facingRight;
	bool attacking;

public:
	Player(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin);

	void handleInput();

	void update(float Gravity);
};
