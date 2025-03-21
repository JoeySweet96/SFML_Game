#include "../include/entity.hpp"
#include "../include/player.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


//animation variables
int frameWidth = 100;
int frameHeight = 100;
int idleIndex = 0;
int walkIndex = 0;
int attackIndex = 0;
int frameCounter = 0;

float gravity = .4;
float sGravity = .7;

int iFrames = 60;
int iTimer = 0;

int slimeCount = 0;

sf::RenderWindow window(sf::VideoMode({800, 600}), "Slime man and Knight");

int main()
{
	window.setVerticalSyncEnabled(true);


	//Player health & slimes killed strings
	sf::Font font("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf");
	sf::Text text(font);
	sf::Text slimeText(font);
	text.setFillColor(sf::Color::Red);
	slimeText.setFillColor(sf::Color::Red);
	text.setCharacterSize(40);
	slimeText.setCharacterSize(41);
	text.setPosition({20.f, 0.f});
	slimeText.setPosition({500.f, 0.f});

	//Textures
	sf::Texture backgroundTex("../assets/sun.png");
	sf::Texture floorTex("../assets/floor.png");
	sf::Texture playerTex("../assets/Tiny RPG Character Asset Pack v1.03 -Free Soldier&Orc/Characters(100x100)/Soldier/Soldier/Soldier.png");

	//Entities
	Entity background(backgroundTex, {0.f, 0.f}, {5.f, 5.f});
	Entity floor(floorTex, {-50.f, 520.f}, {30.f, 5.f});
	Player player(playerTex, {200.f, 500.f}, {5.f, 5.f}, {50.f, 46.f});


	//npc slime
	int sHealth = 5;
	bool isHit = false;
	sf::Vector2f sPosition = {400.f, 520.f};
	sf::Vector2f sVelocity = {0, 0};
	sf::Texture slimeTex("../assets/grouchslime.png");
	sf::Texture slimeHurtTex("../assets/slimehurt.png");
	sf::Sprite slime(slimeTex);
	slime.setPosition(sPosition);
	slime.setScale({3.0f, 3.0f});
	slime.setOrigin({slimeTex.getSize().x / 2.f, slimeTex.getSize().y / 2.f});



    	while (window.isOpen())
	{
		//text.setString("Player Health: " + std::to_string(pHealth));
		slimeText.setString("Slimes Slain: " + std::to_string(slimeCount));
		//movement
		sPosition += sVelocity;
		slime.setPosition(sPosition);

		player.handleInput();
		player.update(gravity);

		//Gravity
		if(sPosition.y < 520)
			sVelocity.y += sGravity;
		if(sPosition.y > 520)
		{
			sVelocity.y = 0;
			sPosition.y = 520;
		}

		//Enemy tracking
		/*if(pPosition.x - 20 > sPosition.x)
		{
			sVelocity.x = 1;
			slime.setScale({-3.0f, 3.0f});

		}
		else if(pPosition.x + 20 < sPosition.x)
		{
			sVelocity.x = -1;
			slime.setScale({3.0f, 3.0f});
		}
		else
		{
			sVelocity.x = 0;
		}*/

		//Animation
		frameCounter++;
		if(frameCounter > 10)
		{	//idle
			if(player.velocity.x == 0 && player.attacking == false)
			{
				walkIndex = 0;
				player.sprite.setTextureRect(sf::IntRect({idleIndex * 100, 0}, {100, 100}));
				idleIndex++;
				frameCounter = 0;
				if(idleIndex > 5)
					idleIndex = 0;
			}
			//attack
			if(player.attacking)
			{
				idleIndex = 0;
				player.sprite.setTextureRect(sf::IntRect({attackIndex * 100, 200}, {100, 100}));
				attackIndex++;
				frameCounter = 0;
				if(attackIndex > 5)
				{
					attackIndex = 0;
					player.attacking = false;
				}
			}
		}
		if(frameCounter > 6)
		{	//walking
			if(player.velocity.x != 0)
			{
				idleIndex = 0;
				player.sprite.setTextureRect(sf::IntRect({walkIndex * 100, 100}, {100, 100}));
				walkIndex++;
				frameCounter = 0;
				if(walkIndex > 7)
					walkIndex = 0;
			}

		}

		//set your attack points
		sf::FloatRect slimeBox = slime.getGlobalBounds();

		//Check Hitbox --- Switch enemy texture
		if(player.attacking && slimeBox.contains(player.attackPoint))
		{

			if(attackIndex > 3 && attackIndex < 5)
			{
				isHit = true;
				slime.setTexture(slimeHurtTex);
				sVelocity.x = 0;
			}


		}
		else
		{
			slime.setTexture(slimeTex);
		}

		//Deinrcrement slimes health on hit
		if(isHit && !player.attacking)
		{
			sHealth--;
			isHit = false;
		}
		//deincrement player health on hit, start iframes
		if((slimeBox.contains(player.hitpointLeft) || slimeBox.contains(player.hitpointRight)) && iTimer == 0)
		{
			iTimer = iFrames;
			player.health--;
			player.velocity.y = -3;
		}
		if(iTimer > 0)
			iTimer--;

		//respawn slime if killed
		if(sHealth == 0)
		{
			slimeCount++;
			sPosition = {400, -100};
			sHealth = 5;
		}

		//test output
		//std::cout << sHealth << ", " << pHealth << std::endl;
		//std::cout << frameCounter << ", " << idleIndex << ", " << walkIndex << ", " << attackIndex << ", " << pAttacking << std::endl;

		//Drawing
		window.clear();
		window.draw(background.sprite);
		window.draw(floor.sprite);
		window.draw(text);
		window.draw(slimeText);
		window.draw(slime);
		window.draw(player.sprite);
		window.display();

		if(player.health == 0)
			window.close();

		//---Window will stop responding if its not polling for events---//
		while (const std::optional event = window.pollEvent())
		{
            		if (event->is<sf::Event::Closed>())
                		window.close();
        	}

    	}
}

