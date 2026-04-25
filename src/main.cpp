#include <SFML/Graphics.hpp>
#include <iostream>

#include "Map.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_FPS 60
#define WINDOW_NAME "Grand Theft Auto 16px"

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), WINDOW_NAME);
    window.setFramerateLimit(MAX_FPS);

    // Create object here
    Map map;

    // Initialize here
    map.Initialize();

    // Load here
    map.Load();

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time deltatimeTimer = clock.restart();
        double deltaTime = deltatimeTimer.asMicroseconds() / 1000.0;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Update here
        map.Update(deltaTime);

        window.clear(sf::Color::Black);

        // Draw here
        map.Draw(window);

        window.display();
    }

    return 0;
}