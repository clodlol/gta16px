#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tank.h"
#include "utils.h"
#include "Player.h"

#define TILE_SIZE_TANK 125

Tank::Tank(const sf::Vector2f &spwnPos) : tankTexture{}, tankSprite{tankTexture}, spawnLocation{spwnPos}
{
    immunityTimer = immunityTime;
    moveTimer = moveCooldown;
    health = maxHealth;
}

sf::FloatRect Tank::GetBounds() const
{
    return tankSprite.getGlobalBounds();
}

const sf::Sprite &Tank::GetSprite() const
{
    return tankSprite;
}

bool Tank::IsAlive() const { return alive; }

void Tank::TakeDamage(int sourceDamage)
{
    if (immunityTimer <= 0.f && alive)
    {
        int damageTaken = (sourceDamage - sourceDamage * float(defense / 100.f));
        health -= damageTaken;

        if (damageTaken > 10)
        {
            immunityTimer = immunityTime;
        }
        else
        {
            immunityTimer = 0.05f;
        }
    }

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

    tankSprite.setPosition(spawnLocation);

    tankSprite.setOrigin({TILE_SIZE_TANK / 2, TILE_SIZE_TANK / 2});

    tankSprite.setScale({16.f / TILE_SIZE_TANK, 16.f / TILE_SIZE_TANK});

    heavyCannon.Load();
}

void Tank::Update(float deltaTime, sf::View &camera, Player &player)
{
    if (!alive)
        return;

    immunityTimer -= deltaTime;
    moveTimer -= deltaTime;

    if (moveTimer <= 0.f && !moving)
    {
        heavyCannon.StopFire();
        moving = true;

        float deltaRadiusX = (camera.getSize().x / 2.f) * (1.f - aggro);
        float deltaRadiusY = (camera.getSize().y / 2.f) * (1.f - aggro);

        float randomX = randomNumGen(camera.getCenter().x - deltaRadiusX, camera.getCenter().x + deltaRadiusX);
        float randomY = randomNumGen(camera.getCenter().y - deltaRadiusY, camera.getCenter().y + deltaRadiusY);

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
            moveTimer = moveCooldown;
        }
    }

    if (!moving)
    {
        heavyCannon.Fire(Bullet{heavyCannon.GetProjectileTexture(), tankSprite.getPosition(), (player.GetSprite().getPosition() - tankSprite.getPosition()), bulletDamage, bulletVelocity});
    }

    heavyCannon.Update(deltaTime);

    for (Bullet &bullet : heavyCannon.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(bullet.GetBounds()))
        {
            player.TakeDamage(heavyCannon.GetDamage() + bullet.GetDamage());
            bullet.Erase();
        }
    }
}

void Tank::Draw(sf::RenderWindow &window)
{
    if (alive)
    {
        heavyCannon.Draw(window);
        window.draw(tankSprite);

        sf::RectangleShape healthBar({(10.f * float(float(health) / maxHealth)), 2.5f});
        healthBar.setPosition({tankSprite.getPosition().x, tankSprite.getPosition().y});
        healthBar.setFillColor(sf::Color(255, 255, 255));
        window.draw(healthBar);
    }
}