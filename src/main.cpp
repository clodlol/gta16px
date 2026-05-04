#include <SFML/Graphics.hpp>
#include <iostream>

#include "Map.h"
#include "InputManager.h"
#include "Player.h"
#include "Weapon.h"

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
    InputManager input;
    Player player;
    sf::View camera(sf::FloatRect({0.f, 0.f}, {200.f, 200.f}));

    // Initialize here
    map.Initialize();
    player.Initialize();

    // Load here
    map.Load();
    player.Load();

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time deltatimeTimer = clock.restart();
        double deltaTime = deltatimeTimer.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Update here
        input.Update(window);
        player.Update(deltaTime, input, camera);

        if (input.IsActionActive("ZoomIn"))
            camera.zoom(0.99f);
        if (input.IsActionActive("ZoomOut"))
            camera.zoom(1.01f);

        window.clear(sf::Color::Black);

        window.setView(camera);

        // Draw here
        map.Draw(window);
        player.Draw(window);

        window.display();
    }

    return 0;
}