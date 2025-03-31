#include "../include/entity.hpp"
#include "../include/player.hpp"
#include "../include/slime.hpp"
#include "../include/map.hpp"
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


void isColliding(const sf::FloatRect& playerBounds, const int* tiles, int twidth, int theight, int size, bool& fCollide);
void isWallColliding(const sf::FloatRect& playerBounds, const int* tiles, int twidth, int theight, int size, bool& wCollide);

float gravity = 1500.f;
int slimeCount = 0;

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Slime man and Knight");

int main()
{

	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::View view(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));


	//Player health & slimes killed strings
	/*sf::Font font("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf");
	sf::Text text(font);
	sf::Text slimeText(font);
	text.setFillColor(sf::Color::Red);
	slimeText.setFillColor(sf::Color::Red);
	text.setCharacterSize(40);
	slimeText.setCharacterSize(41);
	text.setPosition({20.f, 0.f});
	slimeText.setPosition({500.f, 0.f});*/

	//Textures
	sf::Texture playerTex("../assets/Tiny RPG Character Asset Pack v1.03 -Free Soldier&Orc/Characters(100x100)/Soldier/Soldier/Soldier.png");
	sf::Texture slimeTex("../assets/grouchslime.png");
	sf::Texture slimeHurtTex("../assets/slimehurt.png");

	//Entities
	Player player(playerTex, {400.f, 200.f}, {5.f, 5.f}, {50.f, 50.f});

	//slimes
	std::vector<Slime> slimes;
	float slimeSpawnTimer = 0;
	const float slimeSpawnInterval = 5.f;
	int slimeSpawnPicker = 1;


	constexpr std::array level = {
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	Map map;
	map.load("../assets/tileset.png", {64, 64}, level.data(), 40, 12);

    	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();
		//text.setString("Player Health: " + std::to_string(player.health));
		//slimeText.setString("Slimes Slain: " + std::to_string(slimeCount));

		//camera
		view.setCenter({std::round(player.sprite.getPosition().x),static_cast<float>(std::round(window.getSize().y / 2))});
		if(view.getCenter().x < 640)
			view.setCenter({640.f, window.getSize().y / 2.f});
		if(view.getCenter().x > 1920)
			view.setCenter({1920.f, window.getSize().y / 2.f});
		window.setView(view);


		//slime spawner
		slimeSpawnTimer += deltaTime;
		if(slimeSpawnTimer >= slimeSpawnInterval)
		{
			if(slimeSpawnPicker % 2 == 0)
				slimes.emplace_back(slimeTex, sf::Vector2f({100.f, 500.f}), sf::Vector2f({3.f, 3.f}),sf::Vector2f({slimeTex.getSize().x / 2.f, 20.f}));
			else
				slimes.emplace_back(slimeTex, sf::Vector2f({300.f, 500.f}), sf::Vector2f({3.f, 3.f}),sf::Vector2f({slimeTex.getSize().x / 2.f, 20.f}));

			slimeSpawnPicker++;
			slimeSpawnTimer = 0;
		}


		//collision detection
		isColliding(player.hitBox, level.data(), 40, 12, 64, player.fCollide);
		isWallColliding(player.collisionBox, level.data(), 40, 12, 64, player.wCollide);
		//movement
		player.handleInput();
		player.update(gravity, deltaTime);

		//player slime logic
		for(auto& slime: slimes)
		{
			isColliding(slime.hitBox, level.data(), 40, 12, 64, slime.fCollide);
			if(!slime.fCollide)
				slime.velocity.y += gravity * deltaTime;
			else
				slime.velocity.y = 0;
			slime.update(gravity, player.sprite.getPosition().x, deltaTime);
			player.hitLanded(slime);
			slime.takeDamage(deltaTime, slimeTex, slimeHurtTex);
			if(player.iframes <=0)
				player.playerHit(slime);

			//respawn slime if killed
			slime.respawn(slimeCount);
		}

		if(player.iframes > 0)
			player.iframes--;
		if(player.health == 0)
			window.close();
		//-----------------------------------------------------------------------------------------------------------------------------------//

		//Drawing
		window.clear();
		window.draw(map);
		for(auto& slime: slimes)
		{
			window.draw(slime.sprite);
			//window.draw(slime.rectangle);
		}
		window.draw(player.sprite);
		//window.draw(player.attackPointHB);
		window.draw(player.rectangle);
		window.display();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();


		//---Window will stop responding if its not polling for events---//
		while (const std::optional event = window.pollEvent())
		{
            		if (event->is<sf::Event::Closed>())
                		window.close();
        	}

    	}
}

void isColliding(const sf::FloatRect& playerBounds, const int* tiles, int twidth, int theight, int size, bool& fCollide)
{

	fCollide = false;

	for(int i = 0; i < twidth * theight; i++)
	{
		int x = (i % twidth) * size;
		int y = (i / twidth) * size;

		if(tiles[i] == 0 || tiles[i] == 1)
		{
			sf::FloatRect tileBounds({static_cast<float>(x),static_cast<float>(y)}, {static_cast<float>(size), static_cast<float>(size)});
			auto intersection = playerBounds.findIntersection(tileBounds);

			if(intersection)
			{

					fCollide = true;
			}

		}
	}

}

void isWallColliding(const sf::FloatRect& playerBounds, const int* tiles, int twidth, int theight, int size, bool& wCollide)
{

	wCollide = false;

	for(int i = 0; i < twidth * theight; i++)
	{
		int x = (i % twidth) * size;
		int y = (i / twidth) * size;

		if(tiles[i] == 0 || tiles[i] == 1)
		{
			sf::FloatRect tileBounds({static_cast<float>(x),static_cast<float>(y)}, {static_cast<float>(size), static_cast<float>(size)});
			auto intersection = playerBounds.findIntersection(tileBounds);

			if(intersection && (intersection->size.x > intersection->size.y))
			{
					wCollide = true;
			}

		}
	}

}



















