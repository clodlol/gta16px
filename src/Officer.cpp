#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "Officer.h"
#include "Player.h"

#define TILE_SIZE 110

float randomNumGen(float min, float max)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

Officer::Officer() : officerTexture{}, officerSprite{officerTexture} {}

sf::FloatRect Officer::GetBounds()
{
    return officerSprite.getGlobalBounds();
}

void Officer::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage - sourceDamage * (defense / 100));
}

void Officer::Kill()
{
    alive = false;
}

void Officer::Load()
{
    if (!officerTexture.loadFromFile("../assets/officer.png"))
    {
        std::cout << "Failed to load officer texture\n";
        return;
    }

    officerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));

    officerSprite.setPosition({900.f, 900.f});

    officerSprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
    officerSprite.setScale({16.f / TILE_SIZE, 16.f / TILE_SIZE});

    pistol.Load();
}

void Officer::Update(float deltaTime, sf::View &camera, Player &player)
{
    moveTimer -= deltaTime;

    if (moveTimer <= 0.f && !moving)
    {
        pistol.StopFire();
        moving = true;

        float randomX = randomNumGen(camera.getCenter().x - camera.getSize().x / 2, camera.getCenter().x + camera.getSize().x / 2);
        float randomY = randomNumGen(camera.getCenter().y - camera.getSize().y / 2, camera.getCenter().y + camera.getSize().y / 2);
        currentDestination = sf::Vector2f{randomX, randomY};
    }

    if (moving)
    {
        if ((officerSprite.getPosition() - currentDestination).length() > 1e-2)
        {
            officerSprite.move((currentDestination - officerSprite.getPosition()) / ((currentDestination - officerSprite.getPosition()).length()) * speed * deltaTime);
        }
        else
        {
            moving = false;
            moveTimer = 5.f;
        }
    }

    if (!moving)
    {
        pistol.Fire(officerSprite.getPosition(), (player.GetSprite().getPosition() - officerSprite.getPosition()).angle(), "Officer");
    }

    pistol.Update(deltaTime, camera);
}

void Officer::Draw(sf::RenderWindow &window)
{
    pistol.Draw(window);
    window.draw(officerSprite);
}