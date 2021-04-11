#include "libdef.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "World.h"

int main()
{

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Tower Defense", sf::Style::Default);
	window.setFramerateLimit(60);
	sf::CircleShape circle(10);
	circle.setPosition(100, 100);

	
	World world(word_types::normal, &window);

	world.newGun();
	
	
	while (window.isOpen())
	{

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{

				window.close();
			}
		}

		window.clear();

		//std::cout << circle.getGlobalBounds().left << std::endl;
		
		window.draw(world);

		window.display();

	}

	return EXIT_SUCCESS;

}