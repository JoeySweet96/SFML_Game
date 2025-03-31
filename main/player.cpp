#include "../include/player.hpp"
#include <iostream>

Player::Player(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f origin)
	: Entity(texture, position, scale), health(5), velocity(0.f, 0.f), attacking(false), facingRight(false), iframes(0)
{
	sprite.setOrigin(origin);
	attackPointHB.setRadius(2);
	attackPointHB.setPosition({0,0});
	attackPointHB.setFillColor(sf::Color::Red);
	animations["idle"] = {0, 6, 0.15};
	animations["walk"] = {100, 8, 0.1};
	animations["attack"] = {200, 6, 0.2f};
}

void Player::handleInput()
{
	if(attacking == false)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			setAnimation("walk");
			velocity.x = -300;
			sprite.setScale({-5.f, 5.f});
			facingRight = false;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			setAnimation("walk");
			velocity.x = 300;
			sprite.setScale({5.f, 5.f});
			facingRight = true;
		}
		else
		{
			setAnimation("idle");
			velocity.x = 0.f;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		setAnimation("attack");
		velocity.x = 0.f;
		attacking = true;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && fCollide)
	{
		velocity.y = -500.f;
		fCollide = false;
	}

}
void Player::update(float gravity, float deltatime)
{
	rectangle.setSize({60.f, 20.f});
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition({sprite.getPosition().x - 32, sprite.getPosition().y - 16});
	hitBox = sf::FloatRect({sprite.getPosition().x - 32, sprite.getPosition().y - 16}, {60.f, 60.f});
	collisionBox = sf::FloatRect({sprite.getPosition().x - 32, sprite.getPosition().y - 16}, {60.f, 20.f});

	if(fCollide)
		velocity.y = 0;
	else
		velocity.y += gravity * deltatime;

	//movement
	sprite.move({velocity.x * deltatime, velocity.y * deltatime});
	//set hitpoints
	hitpointLeft = {sprite.getPosition().x - 10, sprite.getPosition().y + 25};
	hitpointRight = {sprite.getPosition().x + 10, sprite.getPosition().y + 25};
	if(sprite.getPosition().x < 10)
		sprite.setPosition({10.f, sprite.getPosition().y});
	if(sprite.getPosition().x > 2540)
		sprite.setPosition({2540.f, sprite.getPosition().y});

	//increment animation frames by deltatime
	animationTimer += deltatime;
	const Animation& anim = animations[currentAnimation];
	if(animationTimer >= anim.speed)
	{
		animationTimer = 0.f;
		frameIndex = (frameIndex + 1) % anim.frameCount;
		sprite.setTextureRect(sf::IntRect({frameIndex * 100, anim.startY}, {100, 100}));
	}
	if(currentAnimation == "attack" && frameIndex > 4)
	{
		attacking = false;
	}

	//gravity
	if(velocity.y > 800)
	{
		velocity.y = 800;
	}
	/*if(sprite.getPosition().y > 620)
	{
		velocity.y = 0;
		sprite.setPosition({sprite.getPosition().x, 620});
	}*/

	//set attackpoint
	if(facingRight)
	{
		attackPoint = {sprite.getPosition().x + 75, sprite.getPosition().y + 25};
		attackPointHB.setPosition({sprite.getPosition().x + 75, sprite.getPosition().y + 25});
	}
	else if(!facingRight)
	{
		attackPoint = {sprite.getPosition().x - 75, sprite.getPosition().y + 25};
		attackPointHB.setPosition({sprite.getPosition().x + 75, sprite.getPosition().y + 25});
	}

}

void Player::setAnimation(const std::string& anim)
{
	if(currentAnimation != anim)
	{
		currentAnimation = anim;
		frameIndex = 0;
		animationTimer = 0.f;
	}
}

void Player::hitLanded(Slime& slime)
{
	if(attacking && slime.hitBox.contains(attackPoint))
	{
		if(frameIndex > 2 && frameIndex < 4)
		{
			slime.isHit = true;
		}
	}

}

void Player::playerHit(Slime& slime)
{
	if(slime.hitBox.contains(hitpointLeft) || slime.hitBox.contains(hitpointRight))
	{
		iframes = 180;
		health--;
		velocity.y = -300;
	}

}



































