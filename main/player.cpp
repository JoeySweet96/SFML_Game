#include "../include/player.hpp"

Player::Player(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin)
	: Entity(texture, position, scale), health(5), velocity(0.f, 0.f), attacking(false), facingRight(false)
{
	sprite.setOrigin(origin);
}

void Player::handleInput()
{
	if(attacking == false)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			velocity.x = -3.5;
			sprite.setScale({-5.f, 5.f});
			facingRight = false;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			velocity.x = 3.5;
			sprite.setScale({5.f, 5.f});
			facingRight = true;
		}
		else
		{
			velocity.x = 0.f;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		velocity.x = 0.f;
		attacking = true;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && sprite.getPosition().y == 500)
	{
		velocity.y = -8.f;
	}

	/*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		window.close();*/

}
void Player::update(float gravity)
{
	velocity.y += gravity;
	sprite.move(velocity);
	hitpointLeft = {sprite.getPosition().x - 10, sprite.getPosition().y - 25};
	hitpointRight = {sprite.getPosition().x + 10, sprite.getPosition().y - 25};

	if(velocity.y > 8)
	{
		velocity.y = 8;
	}
	if(sprite.getPosition().y > 500)
	{
		velocity.y = 0;
		sprite.setPosition({sprite.getPosition().x, 500});
	}
	if(facingRight)
		attackPoint = {sprite.getPosition().x + 75, sprite.getPosition().y};
	else
		attackPoint = {sprite.getPosition().x - 75, sprite.getPosition().y};

}
