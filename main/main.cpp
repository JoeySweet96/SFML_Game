#include "../include/entity.hpp"
#include "../include/player.hpp"
#include "../include/slime.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


float gravity = .4;
int iFrames = 60;
int iTimer = 0;
int slimeCount = 0;

sf::RenderWindow window(sf::VideoMode({1280, 720}), "Slime man and Knight");

int main()
{
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;

	sf::View view(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));


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
	sf::Texture slimeTex("../assets/grouchslime.png");
	sf::Texture slimeHurtTex("../assets/slimehurt.png");

	//Entities
	Entity background(backgroundTex, {0.f, 0.f}, {8.f, 7.f});
	Entity leftBg(backgroundTex, {-1280.f, 0.f}, {8.f, 7.f});
	Entity rightBg(backgroundTex, {1280.f, 0.f}, {8.f, 7.f});
	Entity floor(floorTex, {0.f, 640.f}, {45.f, 5.f});
	Entity lfloor(floorTex, {-1340.f, 640.f}, {47.f, 5.f});
	Entity rfloor(floorTex, {1280.f, 640.f}, {45.f, 5.f});
	Player player(playerTex, {200.f, 620.f}, {5.f, 5.f}, {50.f, 46.f});

	//slimes
	std::vector<Slime> slimes;
	float slimeSpawnTimer = 0;
	const float slimeSpawnInterval = 10.f;
	slimes.emplace_back(slimeTex, sf::Vector2f({400.f, -100.f}), sf::Vector2f({3.f, 3.f}),sf::Vector2f({slimeTex.getSize().x / 2.f, slimeTex.getSize().y / 2.f}));
	int slimeSpawnPicker = 1;

    	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();
		text.setString("Player Health: " + std::to_string(player.health));
		slimeText.setString("Slimes Slain: " + std::to_string(slimeCount));

		//camera
		view.setCenter({player.sprite.getPosition().x, window.getSize().y / 2.f});
		if(view.getCenter().x < -640)
			view.setCenter({-640.f, window.getSize().y / 2.f});
		if(view.getCenter().x > 1920)
			view.setCenter({1920.f, window.getSize().y / 2.f});
		window.setView(view);

		slimeSpawnTimer += deltaTime;
		if(slimeSpawnTimer >= slimeSpawnInterval)
		{
			if(slimeSpawnPicker % 2 == 0)
				slimes.emplace_back(slimeTex, sf::Vector2f({400.f, -100.f}), sf::Vector2f({3.f, 3.f}),sf::Vector2f({slimeTex.getSize().x / 2.f, slimeTex.getSize().y / 2.f}));
			else
				slimes.emplace_back(slimeTex, sf::Vector2f({-800.f, -100.f}), sf::Vector2f({3.f, 3.f}),sf::Vector2f({slimeTex.getSize().x / 2.f, slimeTex.getSize().y / 2.f}));
			slimeSpawnPicker++;
			slimeSpawnTimer = 0;
		}

		//movement
		player.handleInput();
		player.update(gravity, deltaTime);

		//player slime logic
		for(auto& slime: slimes)
		{
			slime.update(gravity, player.sprite.getPosition().x);
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
		window.draw(leftBg.sprite);
		window.draw(rightBg.sprite);
		window.draw(background.sprite);
		window.draw(floor.sprite);
		window.draw(lfloor.sprite);
		window.draw(rfloor.sprite);
		window.draw(text);
		window.draw(slimeText);
		for(auto& slime: slimes)
			window.draw(slime.sprite);
		window.draw(player.sprite);
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

