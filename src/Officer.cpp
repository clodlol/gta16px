#include <SFML/Graphics.hpp>
#include <iostream>

#include "Officer.h"
#include "Player.h"
#include "utils.h"

#define TILE_SIZE_OFFICER 110

Officer::Officer() : officerTexture{}, officerSprite{officerTexture} {}

sf::FloatRect Officer::GetBounds() const
{
    return officerSprite.getGlobalBounds();
}

const sf::Sprite &Officer::GetSprite() const
{
    return officerSprite;
}

void Officer::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage - sourceDamage * (defense / 100));

    if (health <= 0)
    {
        alive = false;
    }
}

void Officer::Load()
{
    if (!officerTexture.loadFromFile("../assets/officer.png"))
    {
        std::cout << "Failed to load officer texture\n";
        return;
    }

    officerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_OFFICER, TILE_SIZE_OFFICER}));

    officerSprite.setPosition({900.f, 900.f});

    officerSprite.setOrigin({TILE_SIZE_OFFICER / 2, TILE_SIZE_OFFICER / 2});
    officerSprite.setScale({16.f / TILE_SIZE_OFFICER, 16.f / TILE_SIZE_OFFICER});

    pistol.Load("../assets/bullet.png");
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
        if ((officerSprite.getPosition() - currentDestination).length() > 1)
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
        pistol.Fire(Bullet{pistol.GetProjectileTexture(), officerSprite.getPosition(), (player.GetSprite().getPosition() - officerSprite.getPosition()), 20, 10.f});
    }

    pistol.Update(deltaTime);

    for (const Bullet &bullet : pistol.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(bullet.GetBounds()))
        {
            player.TakeDamage(pistol.GetDamage() + bullet.GetDamage());
        }
    }
}

void Officer::Draw(sf::RenderWindow &window)
{
    pistol.Draw(window);
    window.draw(officerSprite);
}