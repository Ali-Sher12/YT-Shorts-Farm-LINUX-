#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "globals.h"
#include "Animation.h"
#include "background.h"
#include "Room.h"
#include "placables.h"
#include "Ray.h"
#include "Player.h"
#include "Levels.h"

using namespace sf;

int main()
{

	RenderWindow window(VideoMode({ 800, 600 }), "??", State::Fullscreen);
	// Hide the mouse cursor0
	window.setMouseCursorVisible(false);

	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	int pass = 0;
	level l1;
	
	Event* event;
	while (window.isOpen())
	{

		while (optional event = window.pollEvent())
		{
			if (event->is<sf::Event::FocusLost>())
				pass = 0;

			if (event->is<sf::Event::FocusGained>())
				pass = 0;

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}

		}

		window.clear();

		//draw here
		l1.draw_level(window);
		window.display();
	}
	

	return 0;
}

