#include "../include/slime.hpp"

Slime::Slime(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin)
	: Entity(texture, position, scale), health(3), velocity(0.f, 0.f), isHit(false)
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
	if(sprite.getPosition().y > 520)
	{
		velocity.y = 0;
		sprite.setPosition({sprite.getPosition().x, 520});
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

	sprite.move(velocity);

}
