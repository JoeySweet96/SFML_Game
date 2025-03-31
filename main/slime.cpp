#include "../include/slime.hpp"
#include <iostream>

Slime::Slime(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin)
	: Entity(texture, position, scale), health(3), velocity(0.f, 0.f), isHit(false), timer(60)
{
	sprite.setOrigin(origin);
	hitBox = sf::FloatRect({sprite.getPosition().x, sprite.getPosition().y}, {60, 32});
}

void Slime::update(float gravity, float playerPos, float deltatime)
{
	rectangle.setSize({60.f, 60.f});
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition({sprite.getPosition().x - 16, sprite.getPosition().y - 16});

	hitBox.position.x = sprite.getPosition().x - 16;
	hitBox.position.y = sprite.getPosition().y - 16;
	//std::cout << hitBox.getCenter().x << ", " <<  hitBox.getCenter().y << std::endl;
	if(velocity.y > 600)
	{
		velocity.y = 600;
	}
	/*if(sprite.getPosition().y > 640)
	{
		velocity.y = 0;
		sprite.setPosition({sprite.getPosition().x, 640});
	}*/

	if(playerPos - 20 > sprite.getPosition().x)
        {
                velocity.x = 100;
                sprite.setScale({-3.0f, 3.0f});

        }
	else if(playerPos + 20 < sprite.getPosition().x)
        {
                velocity.x = -100;
                sprite.setScale({3.0f, 3.0f});
        }
	else
        {
                velocity.x = 0;
        }
	if(isHit)
		velocity.x = 0;


	sprite.move({velocity.x * deltatime, velocity.y * deltatime});

}

void Slime::takeDamage(float deltaTime, sf::Texture& texture, sf::Texture& hurtTexture)
{
	if(isHit)
	{
		sprite.setTexture(hurtTexture);
    		timer -= deltaTime;
    		if (timer <= 0)
		{
			health--;
			sprite.setTexture(texture);
			isHit = false;
			timer = 60;
		}
	}
}

void Slime::respawn(int& counter)
{
	if(health == 0)
	{
		counter++;
		sprite.setPosition({400, 10});
		health = 3;
	}
}
