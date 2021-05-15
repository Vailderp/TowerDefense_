#include "libdef.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GUI_inits.h"

int main()
{

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Tower Defense", sf::Style::Default);
	window.setFramerateLimit(60);
	sf::CircleShape circle(10);
	circle.setPosition(100, 100);

	/*sf::Image image;
	image.create(100, 100, sf::Color(255, 255, 255, 0));
	image.saveToFile("data/textures/w.png");*/
	
	World world(word_types::normal, &window);

	world.newGun(1);

	Menu menu(&window);

	menu.onMode(
		[&](word_types type)
		{
			world.setMode(type);
		});
	
	int you_are_win = 2;
	
	//MAIN SOUND
	sf::SoundBuffer sound_buffer;
	sf::Sound sound;
	if (!sound_buffer.loadFromFile("data/sounds/main.ogg"))
	{
		std::cout << "Error >> World >> main >> music >> not loaded" << std::endl;
	}
	sound.setBuffer(sound_buffer);
	sound.setLoop(true);
	bool already_played = false;

	//MAIN GUI
	gui::Drawable drawable(1280.0F, 720.0F, "data/textures/w.png", &window);

	bool gun_upgradeable = false;
	Gun* gun = nullptr;
	gui::Button shadow_button
	{
		0.75F * 1280.0F, 0.0F * 720.0F, 0.25F * 1280.0F, 1.0F * 720.0F,
		"data/textures/gray.png",
		"data/textures/gray.png",
		"data/textures/w.png"
	};

	//shadow_button.setOnOutTexture(texture_none);
	
	gui::Button button_wave
	{
		0.80F * 1280.0F, 0.25F * 720.0F, 0.15F * 1280.0F, 0.075F * 720.0F,
		"data/textures/buttons/wavet.png",
		"data/textures/buttons/wave_ont.png",
		"data/textures/buttons/wave_click.png"
	};

	gui::Button button_gun
	{
		0.80F * 1280.0F, 0.40F * 720.0F, 0.15F * 1280.0F, 0.27F * 720.0F,
		"data/textures/cannon1.png",
		"data/textures/cannon1.png",
		"data/textures/cannon1.png"
	};


	gui::Button button_upgrade
	{
		0.80F * 1280.0F, 0.40F * 720.0F, 0.15F * 1280.0F, 0.08F * 720.0F,
		"data/textures/buttons/upgrade_click.png",
		"data/textures/buttons/upgrade_on.png",
		"data/textures/buttons/upgrade.png"
	};

	int coins = 0;
	gui::Button button_coin
	{
		0.77F * 1280.0F, 0.02F * 720.0F, 0.05F * 1280.0F, 0.08F * 720.0F,
		"data/textures/coin.png",
		"data/textures/coin.png",
		"data/textures/coin.png"
	};

	gui::Text text;
	text.loadFontFromFile("data/fonts/Hanalei-Regular.ttf").setPosition(0.830F * 1280.0F, 0.010F * 720.0F);
	text.text->setString("0");
	text.text->setCharacterSize(0.080F * 720.0F);

	gui::Button button_lose
	{
			0.25F * 1280.0F, 0.25F * 720.0F, 0.50F * 1280.0F, 0.50F * 720.0F,
		"data/textures/buttons/lose.png",
		"data/textures/buttons/lose.png",
		 "data/textures/buttons/lose.png"
	};

	gui::Button button_win
	{
			0.25F * 1280.0F, 0.25F * 720.0F, 0.50F * 1280.0F, 0.50F * 720.0F,
		"data/textures/buttons/win.png",
		"data/textures/buttons/win.png",
		 "data/textures/buttons/win.png"
	};
	
	drawable << &shadow_button << &button_wave.setActive(false)
	<< &button_coin.setActive(false) << &button_gun.setActive(false)
	<< &text.setActive(false) << &button_upgrade.setActive(false)
	<< &button_lose.setActive(false) << &button_win.setActive(false);

	gui::Button button_hearts
	{ 0.90F * 1280.0F, 0.02F * 720.0F, 0.05F * 1280.0F, 0.03F * 720.0F,
		"data/textures/hearts3.png",
		"data/textures/hearts3.png",
		"data/textures/hearts3.png"
	};

	drawable << &button_hearts.setActive(false);
	
	shadow_button.setOnMoveFunction(
		[&]()
		{
			if (you_are_win == 2)
			{
				button_wave.setActive(true);
				button_coin.setActive(true);
				text.setActive(true);
				button_hearts.setActive(true);
				if (!gun_upgradeable)
				{
					button_gun.setActive(true);
				}
				else
				{
					button_upgrade.setActive(true);
				}
			}
		});

	shadow_button.setOnOutFunction(
		[&]()
		{
			button_wave.setActive(false);
			button_coin.setActive(false);
			button_gun.setActive(false);
			text.setActive(false);
			button_upgrade.setActive(false);
			button_hearts.setActive(false);
		});

	button_upgrade.setOnDownFunction(
		[&]()
		{
			gun_upgradeable = true;
			if (coins >= 8)
			{
				if (gun_upgradeable == true)
				{
					gun_upgradeable = false;
					gun->setLevel(2, 300, 3, 20);
					coins -= 8;
					text.text->setString(std::to_string(coins));
				}
			}
		});

	button_wave.setOnDownFunction(
		[&]()
		{
			world.newWave(world.getWave() + 1);
		});

	button_gun.setOnDownFunction(
		[&]()
		{
			gun_upgradeable = false;
			if (coins >= 4)
			{
				if (gun_upgradeable == false)
				{
					gun_upgradeable = false;
					world.newGun(1);
					coins -= 4;
					text.text->setString(std::to_string(coins));
				}
			}
		});

	world.onMonsterRemove(
		[&](Monster* monster)
		{
			coins += 2;
			text.text->setString(std::to_string(coins));
		});

	int hearts = 3;
	world.onMonsterOut(
		[&](Monster /*monster*/)
		{
			const std::string n = std::to_string(hearts - 1);
			hearts--;
			if (hearts > 0)
			{
				button_hearts.setOnDownTexture("data/textures/hearts" + n + ".png").setOnMoveTexture("data/textures/hearts" + n + ".png").setOnOutTexture("data/textures/hearts" + n + ".png");
			}
			else
			{
				you_are_win = 0;
				world.stop(true);
				button_lose.setActive(true);
			}
		});

	world.onWin(
		[&]()
		{
			you_are_win = 1;
			world.stop(true);
			button_win.setActive(true);
		});

	
	
	
	while (window.isOpen())
	{

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || 
				sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{

				window.close();
			}
		}

		window.clear();

		if (menu.gameShouldBeStarted())
		{
			window.draw(world);
			if (already_played == false)
			{
				already_played = true;
				sound.play();
			}


			drawable.draw();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (F(sf::Mouse::getPosition(window).x) < 0.75F * 1280.0F)
					
				{
					gun = world.parseClick();
				}
				if (gun != nullptr)
				{
					if (gun->getLevel() < 2)
					{
						//std::cout << "null" << std::endl;
						gun_upgradeable = true;
					}
				}
				else
				{
					//std::cout << "yes" << std::endl;
					gun_upgradeable = false;
				}
			}
			
		}
		window.draw(menu);

		window.display();

	}

	return EXIT_SUCCESS;

}