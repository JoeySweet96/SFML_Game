#include <SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>

//function declarations
void handleInput(sf::Vector2f& velocity, sf::Vector2f& position, sf::Sprite& sprite, bool& attack, bool& facingRight);

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

	//background
	sf::Texture backgroundTex("sun.png");
	sf::Sprite background(backgroundTex);
	background.setScale({5.f, 5.f});

	//floor
	sf::Vector2f fPosition = {-50.f, 520.f};
	sf::Texture floorTex("floor.png");
	sf::Sprite floor(floorTex);
	floor.setPosition(fPosition);
	floor.setScale({30.f, 5.f});


	//npc slime
	int sHealth = 5;
	bool isHit = false;
	sf::Vector2f sPosition = {400.f, 520.f};
	sf::Vector2f sVelocity = {0, 0};
	sf::Texture slimeTex("grouchslime.png");
	sf::Texture slimeHurtTex("slimehurt.png");
	sf::Sprite slime(slimeTex);
	slime.setPosition(sPosition);
	slime.setScale({3.0f, 3.0f});
	slime.setOrigin({slimeTex.getSize().x / 2.f, slimeTex.getSize().y / 2.f});

	//player
	int pHealth = 5;
	bool pAttacking = false;
	sf::Vector2f pPosition = {200.f, 500.f};
	sf::Vector2f pVelocity = {0.f, 0.f};
	sf::Texture soldierTex("Tiny RPG Character Asset Pack v1.03 -Free Soldier&Orc/Characters(100x100)/Soldier/Soldier/Soldier.png");
	sf::Sprite soldier(soldierTex);
	soldier.setOrigin({50.f, 46.f});
	soldier.setScale({5.f, 5.f});
	soldier.setPosition({400.f, 500.f});
	bool facingRight = true;
	sf::Vector2f attackPoint;
	sf::Vector2f hitPointLeft;
	sf::Vector2f hitPointRight;

    	while (window.isOpen())
	{
		text.setString("Player Health: " + std::to_string(pHealth));
		slimeText.setString("Slimes Slain: " + std::to_string(slimeCount));
		//movement
		pPosition += pVelocity;
		sPosition += sVelocity;
		hitPointLeft = {pPosition.x - 10, pPosition.y - 25};
		hitPointRight = {pPosition.x + 10, pPosition.y - 25};
		handleInput(pVelocity, pPosition, soldier, pAttacking, facingRight);
		soldier.setPosition(pPosition);
		slime.setPosition(sPosition);

		//Gravity
		pVelocity.y += gravity;
		if(sPosition.y < 520)
			sVelocity.y += sGravity;
		//player gravity
		if(pVelocity.y > 8)
			pVelocity.y = 8;

		if(pPosition.y > 500)
		{
			pVelocity.y = 0;
			pPosition.y = 500;
		}
		//enemy gravity
		if(sPosition.y > 520)
		{
			sVelocity.y = 0;
			sPosition.y = 520;
		}

		//Enemy tracking
		if(pPosition.x - 20 > sPosition.x)
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
		}

		//Animation
		frameCounter++;
		if(frameCounter > 10)
		{	//idle
			if(pVelocity.x == 0 && pAttacking == false)
			{
				walkIndex = 0;
				soldier.setTextureRect(sf::IntRect({idleIndex * 100, 0}, {100, 100}));
				idleIndex++;
				frameCounter = 0;
				if(idleIndex > 5)
					idleIndex = 0;
			}
			//attack
			if(pAttacking)
			{
				idleIndex = 0;
				soldier.setTextureRect(sf::IntRect({attackIndex * 100, 200}, {100, 100}));
				attackIndex++;
				frameCounter = 0;
				if(attackIndex > 5)
				{
					attackIndex = 0;
					pAttacking = false;
				}
			}
		}
		if(frameCounter > 6)
		{	//walking
			if(pVelocity.x != 0)
			{
				idleIndex = 0;
				soldier.setTextureRect(sf::IntRect({walkIndex * 100, 100}, {100, 100}));
				walkIndex++;
				frameCounter = 0;
				if(walkIndex > 7)
					walkIndex = 0;
			}

		}

		//set your attack points
		sf::FloatRect slimeBox = slime.getGlobalBounds();
		if(facingRight)
			attackPoint = {pPosition.x + 75, pPosition.y};
		else
			attackPoint = {pPosition.x - 75, pPosition.y};

		//Check Hitbox --- Switch enemy texture
		if(pAttacking && slimeBox.contains(attackPoint))
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
		if(isHit && !pAttacking)
		{
			sHealth--;
			isHit = false;
		}
		//deincrement player health on hit, start iframes
		if((slimeBox.contains(hitPointLeft) || slimeBox.contains(hitPointRight)) && iTimer == 0)
		{
			iTimer = iFrames;
			pHealth--;
			pVelocity.y = -3;
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
		window.draw(background);
		window.draw(floor);
		window.draw(text);
		window.draw(slimeText);
		window.draw(slime);
		window.draw(soldier);
		window.display();

		if(pHealth == 0)
			window.close();

		//---Window will stop responding if its not polling for events---//
		while (const std::optional event = window.pollEvent())
		{
            		if (event->is<sf::Event::Closed>())
                		window.close();
        	}

    	}
}


//function definitions
void handleInput(sf::Vector2f& velocity, sf::Vector2f& position, sf::Sprite& sprite, bool& attack, bool& facingRight)
{
	if(attack == false)
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
		attack = true;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && position.y == 500)
	{
		velocity.y = -8.f;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		window.close();

}
