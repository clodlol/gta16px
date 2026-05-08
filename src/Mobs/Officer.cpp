#include <SFML/Graphics.hpp>

#include "Officer.h"

#define TILE_SIZE 110

Officer::Officer() : officerTexture{}, officerSprite{officerTexture} {}

sf::FloatRect Officer::GetBounds()
{
    return officerSprite.getGlobalBounds();
}

void Officer::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage * (defense / 100));
}

void Officer::Kill()
{
    alive = false;
}

void Officer::Load()
{
    if (officerTexture.loadFromFile("../assets/officer.png"))
    {
        std::cout << "Failed to load officer texture\n";
        return;
    }

    officerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));

    officerSprite.setPosition({0.f, 0.f});

    officerSprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
    officerSprite.setScale({16.f / TILE_SIZE, 16.f / TILE_SIZE});

    pistol.Load();
}

void Officer::Update(float deltaTime, sf::View &camera)
{
    moveCooldown -= deltaTime;

    if (moveCooldown <= 0.f)
    {
        moveCooldown = 5.f;

        // move
    }

    pistol.Update(deltaTime, camera);
}

void Officer::Draw(sf::RenderWindow &window)
{
    pistol.Draw(window);
    window.draw(officerSprite);
}