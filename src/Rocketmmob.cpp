#include <iostream>
#include <SFML/Graphics.hpp>
#include "Rocketmob.h";
#include "utils.h";
#include "Player.h";
#define TILE_SIZE_ROCKETMOB 110

// constructor addition
RocketMob::RocketMob(const sf::Vector2f &spwnPos) : rocketMobTexture{}, rocketMobSprite{rocketMobTexture}, spawnLocation{spwnPos}
{
    moveTimer = moveCooldown;
}
sf::FloatRect RocketMob::GetBounds() const
{
    return rocketMobSprite.getGlobalBounds();
}

const sf::Sprite &RocketMob::GetSprite() const
{
    return rocketMobSprite;
}

void RocketMob::TakeDamage(int sourceDamage)
{
    health -= (sourceDamage - sourceDamage * (defense / 100));

    if (health <= 0)
    {
        alive = false;
    }
}

void RocketMob::Load()
{
    if (!rocketMobTexture.loadFromFile("../assets/RocketMob.png"))
    {
        std::cout << "Failed to load rocketmob texture\n";
        return;
    }

    rocketMobSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_ROCKETMOB, TILE_SIZE_ROCKETMOB}));
    rocketMobSprite.setPosition(spawnLocation);
    rocketMobSprite.setOrigin({TILE_SIZE_ROCKETMOB / 2, TILE_SIZE_ROCKETMOB / 2});
    rocketMobSprite.setScale({16.f / TILE_SIZE_ROCKETMOB, 16.f / TILE_SIZE_ROCKETMOB});

    rocketLauncher.Load("../assets/rocket.png");
}

void RocketMob::Update(float deltaTime, sf::View &camera, Player &player)
{
    if (!alive)
        return;

    moveTimer -= deltaTime;

    if (moveTimer <= 0.f && !moving)
    {
        rocketLauncher.StopFire();
        moving = true;

        float randomX = randomNumGen(camera.getCenter().x - camera.getSize().x / 2, camera.getCenter().x + camera.getSize().x / 2);
        float randomY = randomNumGen(camera.getCenter().y - camera.getSize().y / 2, camera.getCenter().y + camera.getSize().y / 2);
        currentDestination = sf::Vector2f{randomX, randomY};
    }

    if (moving)
    {
        if ((rocketMobSprite.getPosition() - currentDestination).length() > 1.f)
        {
            rocketMobSprite.move((currentDestination - rocketMobSprite.getPosition()) / ((currentDestination - rocketMobSprite.getPosition()).length()) * speed * deltaTime);
        }
        else
        {
            moving = false;
            moveTimer = moveCooldown;
        }
    }

    if (!moving)
    {
        rocketLauncher.Fire(RocketBullet{rocketLauncher.GetProjectileTexture(), rocketMobSprite.getPosition(), (player.GetSprite().getPosition() - rocketMobSprite.getPosition()), rocketDamage, rocketVelocity});
    }

    rocketLauncher.Update(deltaTime);

    for (const RocketBullet &rocket : rocketLauncher.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(rocket.GetBounds()))
        {
            player.TakeDamage(rocketLauncher.GetDamage() + rocket.GetDamage());
        }
    }
}

void RocketMob::Draw(sf::RenderWindow &window)
{
    if (alive)
    {
        rocketLauncher.Draw(window);
        window.draw(rocketMobSprite);
    }
}