#pragma once
#include "entity.hpp"
#include "slime.hpp"
#include <map>

class Player : public Entity {
public:
	sf::Vector2f velocity;
	sf::Vector2f attackPoint;
	sf::Vector2f hitpointLeft;
	sf::Vector2f hitpointRight;
	int health;
	bool facingRight;
	bool attacking;
	int iframes;

	float animationTimer = 0.f;
	int frameIndex = 0;
	struct Animation {
		int startY;
		int frameCount;
		float speed;
	};
	std::map<std::string, Animation> animations;
	std::string currentAnimation = "idle";

public:
	Player(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin);

	void handleInput();
	void update(float gravity, float deltatime);
	void setAnimation(const std::string& anim);
	void hitLanded(Slime& slime);
	void playerHit(Slime& slime);
};
