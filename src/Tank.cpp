#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tank.h"
#include "Player.h"
#include "utils.h"

#define TILE_SIZE_TANK 125

Tank::Tank() : tankTexture{}, tankSprite{tankTexture} {}

sf::FloatRect Tank::GetBounds() const
{
    return tankSprite.getGlobalBounds();
}

const sf::Sprite &Tank::GetSprite() const
{
    return tankSprite;
}

void Tank::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage - sourceDamage * (defense / 100));

    if (health <= 0)
    {
        alive = false;
    }
}

void Tank::Load()
{
    if (!tankTexture.loadFromFile("../assets/tank.png"))
    {
        std::cout << "Failed to load tank texture\n";
        return;
    }

    tankSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_TANK, TILE_SIZE_TANK}));

    tankSprite.setPosition({900.f, 900.f});

    tankSprite.setOrigin({TILE_SIZE_TANK / 2, TILE_SIZE_TANK / 2});

    tankSprite.setScale({16.f / TILE_SIZE_TANK, 16.f / TILE_SIZE_TANK});

    heavyCannon.Load("../assets/bullet.png");
}

void Tank::Update(float deltaTime, sf::View &camera, Player &player)
{
    moveTimer -= deltaTime;

    if (moveTimer <= 0.f && !moving)
    {
        heavyCannon.StopFire();
        moving = true;

        float randomX = randomNumGen(camera.getCenter().x - 200, camera.getCenter().x + 200);
        float randomY = randomNumGen(camera.getCenter().y - 200, camera.getCenter().y + 200);
        currentDestination = sf::Vector2f{randomX, randomY};
    }

    if (moving)
    {
        if ((tankSprite.getPosition() - currentDestination).length() > 2.f)
        {
            tankSprite.move((currentDestination - tankSprite.getPosition()) / ((currentDestination - tankSprite.getPosition()).length()) * speed * deltaTime);
        }
        else
        {
            moving = false;
            moveTimer = 6.f;
        }
    }

    if (!moving)
    {
        heavyCannon.Fire(Bullet{heavyCannon.GetProjectileTexture(), tankSprite.getPosition(), (player.GetSprite().getPosition() - tankSprite.getPosition()).angle(), 50});
    }

    heavyCannon.Update(deltaTime);

    for (const Bullet &bullet : heavyCannon.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(bullet.GetBounds()))
        {
            player.TakeDamage(bullet.GetDamage());
        }
    }
}

void Tank::Draw(sf::RenderWindow &window)
{
    heavyCannon.Draw(window);
    window.draw(tankSprite);
}