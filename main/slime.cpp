#include "../include/slime.hpp"

Slime::Slime(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin)
	: Entity(texture, position, scale), health(3), velocity(0.f, 0.f), isHit(false), timer(60)
{
	sprite.setOrigin(origin);
}

void Slime::update(float gravity, float playerPos)
{

	hitBox = sprite.getGlobalBounds();
	velocity.y += gravity;
	if(velocity.y > 8)
	{
		velocity.y = 8;
	}
	if(sprite.getPosition().y > 640)
	{
		velocity.y = 0;
		sprite.setPosition({sprite.getPosition().x, 640});
	}

	if(playerPos - 20 > sprite.getPosition().x)
        {
                velocity.x = 1;
                sprite.setScale({-3.0f, 3.0f});

        }
	else if(playerPos + 20 < sprite.getPosition().x)
        {
                velocity.x = -1;
                sprite.setScale({3.0f, 3.0f});
        }
	else
        {
                velocity.x = 0;
        }
	if(isHit)
		velocity.x = 0;


	sprite.move(velocity);

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
		sprite.setPosition({400, -100});
		health = 3;
	}
}
