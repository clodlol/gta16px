#include <iostream>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "MobSpawner.h"
#include "Officer.h"
#include "Tank.h"
#include "Melee.h"

#define TILE_SIZE_PLAYER 59

Player::Player(const sf::Vector2f &spwnPos) : playerTexture{}, playerSprite{playerTexture}, spawnLocation{spwnPos}
{
    immunityTimer = immunityTime;
    deathTimer = respawnTime;
}

sf::FloatRect Player::GetBounds() const
{
    return playerSprite.getGlobalBounds();
}

const sf::Sprite &Player::GetSprite() const
{
    return playerSprite;
}

void Player::TakeDamage(int sourceDamage)
{
    if (!alive)
        return;

    if (immunityTimer <= 0.f && alive)
    {
        int damageTaken = sourceDamage - float(sourceDamage * defense / 100.f);

        health -= damageTaken;

        if (damageTaken > 10)
        {
            immunityTimer = immunityTime;
        }
        else
        {
            immunityTimer = 0.1f;
        }
    }

    if (health <= 0)
    {
        alive = false;
        deathTimer = respawnTime;
    }
}

void Player::Load()
{
    if (!playerTexture.loadFromFile("../assets/player.png"))
    {
        std::cout << "Failed to load player texture\n";
        return;
    }

    if (!font.openFromFile("../assets/font.ttf"))
    {
        std::cout << "Failed to load font\n";
        return;
    }

    playerSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));

    playerSprite.setPosition({1000.f, 1000.f});
    playerSprite.setOrigin({TILE_SIZE_PLAYER / 2, TILE_SIZE_PLAYER / 2});
    playerSprite.setScale({16.f / TILE_SIZE_PLAYER, 16.f / TILE_SIZE_PLAYER});

    gun.Load();
    sword.Load();
    flamethrower.Load();
    rocketLauncher.Load();
}

void Player::Update(float deltaTime, InputManager &input, sf::View &camera, MobSpawner &spawner)
{
    immunityTimer -= deltaTime;

    if (!alive)
    {
        deathTimer -= deltaTime;
        if (deathTimer <= 0.f)
        {
            playerSprite.setPosition(spawnLocation);
            alive = true;
            health = maxHealth;
            deathTimer = respawnTime;
        }
        else
            return;
    }

    if (input.IsActionActive("Hero0"))
    {
        hero = 0;
        std::cout << "Hero 0\n";
    }
    if (input.IsActionActive("Hero1"))
    {
        hero = 1;
        std::cout << "Hero 1\n";
    }
    if (input.IsActionActive("Hero2"))
    {
        hero = 2;
        std::cout << "Hero 2\n";
    }
    if (input.IsActionActive("Hero3"))
    {
        hero = 3;
        std::cout << "Hero 3\n";
    }

    sf::Vector2f direction{0.f, 0.f};

    if (input.IsActionActive("MoveLeft"))
    {
        direction.x -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 1}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveRight"))
    {
        direction.x += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 2}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveUp"))
    {
        direction.y -= 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 3}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }
    if (input.IsActionActive("MoveDown"))
    {
        direction.y += 1.f;
        playerSprite.setTextureRect(sf::IntRect({0, TILE_SIZE_PLAYER * 0}, {TILE_SIZE_PLAYER, TILE_SIZE_PLAYER}));
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0)
    {
        direction /= length;
    }

    sf::Vector2f movement = direction * speed * deltaTime;

    sf::Vector2f currentPos = playerSprite.getPosition();
    playerSprite.setPosition({std::clamp(currentPos.x + movement.x, 0.f, 1600.f),
                              std::clamp(currentPos.y + movement.y, 0.f, 1600.f)});

    float newCamTopEdge = playerSprite.getPosition().y - camera.getSize().y / 2;
    float newCamLeftEdge = playerSprite.getPosition().x - camera.getSize().x / 2;
    float newCamRightEdge = playerSprite.getPosition().x + camera.getSize().x / 2;
    float newCamBotEdge = playerSprite.getPosition().y + camera.getSize().y / 2;

    sf::Vector2f newCamCenter = camera.getCenter();
    if (newCamLeftEdge > 0.f && newCamRightEdge < 1600.f)
    {
        newCamCenter.x = playerSprite.getPosition().x;
    }
    if (newCamTopEdge > 0.f && newCamBotEdge < 1600.f)
    {
        newCamCenter.y = playerSprite.getPosition().y;
    }

    camera.setCenter(newCamCenter);

    if (input.IsActionActive("Fire") && hero == 0)
    {
        gun.Fire(Bullet{gun.GetProjectileTexture(), playerSprite.getPosition(), input.GetMousePosition(), bulletDamage, bulletVelocity});
    }
    else if (input.IsActionActive("Fire") && !sword.IsSwinging() && hero == 1)
    {
        sword.Swing(*this, input.GetMousePosition().angle());
    }
    else if (input.IsActionActive("Fire") && hero == 2)
    {
        flamethrower.Fire(FlameProjectile{flamethrower.GetProjectileTexture(), playerSprite.getPosition(), input.GetMousePosition(), flameDamage, flameVelocity, flameMaxDistance});
    }
    else if (input.IsActionActive("Fire") && hero == 3)
    {
        rocketLauncher.Fire(RocketBullet{rocketLauncher.GetProjectileTexture(), playerSprite.getPosition(), input.GetMousePosition(), rocketDamage, rocketVelocity, rocketTimeLimit, rocketBlastRad, rocketBlastTime});
    }
    else
    {
        gun.StopFire();
        flamethrower.StopFire();
        rocketLauncher.StopFire();
    }

    gun.Update(deltaTime);
    sword.Update(deltaTime, *this);
    flamethrower.Update(deltaTime);
    rocketLauncher.Update(deltaTime);

    for (Officer *&officer : spawner.GetOfficers())
    {
        if (officer->GetBounds().findIntersection(sword.GetBounds()))
        {
            officer->TakeDamage(sword.GetDamage());
        }
    }

    for (Tank *&tank : spawner.GetTanks())
    {
        if (tank->GetBounds().findIntersection(sword.GetBounds()))
        {
            tank->TakeDamage(sword.GetDamage());
        }
    }

    for (Boss *&boss : spawner.GetBosses())
    {

        if (boss->GetBounds().findIntersection(sword.GetBounds()))
        {
            boss->TakeDamage(sword.GetDamage());
        }
    }

    for (Bullet &bullet : gun.GetProjectiles())
    {
        for (Officer *&officer : spawner.GetOfficers())
        {
            if (officer->GetBounds().findIntersection(bullet.GetBounds()))
            {
                officer->TakeDamage(gun.GetDamage() + bullet.GetDamage());
                bullet.Erase();
            }
        }

        for (Tank *&tank : spawner.GetTanks())
        {

            if (tank->GetBounds().findIntersection(bullet.GetBounds()))
            {
                tank->TakeDamage(gun.GetDamage() + bullet.GetDamage());
                bullet.Erase();
            }
        }

        for (Boss *&boss : spawner.GetBosses())
        {

            if (boss->GetBounds().findIntersection(bullet.GetBounds()))
            {
                boss->TakeDamage(gun.GetDamage() + bullet.GetDamage());
                bullet.Erase();
            }
        }
    }

    for (FlameProjectile &flame : flamethrower.GetProjectiles())
    {
        for (Officer *&officer : spawner.GetOfficers())
        {
            if (officer->GetBounds().findIntersection(flame.GetBounds()))
            {
                officer->TakeDamage(flamethrower.GetDamage() + flame.GetDamage());
                flame.Erase();
            }
        }

        for (Tank *&tank : spawner.GetTanks())
        {

            if (tank->GetBounds().findIntersection(flame.GetBounds()))
            {
                tank->TakeDamage(flamethrower.GetDamage() + flame.GetDamage());
                flame.Erase();
            }
        }

        for (Boss *&boss : spawner.GetBosses())
        {

            if (boss->GetBounds().findIntersection(flame.GetBounds()))
            {
                boss->TakeDamage(flamethrower.GetDamage() + flame.GetDamage());
                flame.Erase();
            }
        }
    }

    for (RocketBullet &rocket : rocketLauncher.GetProjectiles())
    {
        for (Officer *&officer : spawner.GetOfficers())
        {
            if (officer->GetBounds().findIntersection(rocket.GetBounds()))
            {
                if (!rocket.IsExploded())
                    rocket.Explode();

                officer->TakeDamage(rocketLauncher.GetDamage() + rocket.GetDamage());
            }
        }

        for (Tank *&tank : spawner.GetTanks())
        {

            if (tank->GetBounds().findIntersection(rocket.GetBounds()))
            {
                if (!rocket.IsExploded())
                    rocket.Explode();

                tank->TakeDamage(rocketLauncher.GetDamage() + rocket.GetDamage());
            }
        }

        for (Boss *&boss : spawner.GetBosses())
        {

            if (boss->GetBounds().findIntersection(rocket.GetBounds()))
            {
                if (!rocket.IsExploded())
                    rocket.Explode();

                boss->TakeDamage(rocketLauncher.GetDamage() + rocket.GetDamage());
            }
        }
    }
}

void Player::Draw(sf::RenderWindow &window)
{
    if (alive)
    {
        window.draw(playerSprite);
        gun.Draw(window);
        sword.Draw(window);
        flamethrower.Draw(window);
        rocketLauncher.Draw(window);

        sf::RectangleShape healthBar({(10.f * float(float(health) / maxHealth)), 2.5f});
        healthBar.setPosition({playerSprite.getPosition().x - 5.f, playerSprite.getPosition().y + 16.f});
        healthBar.setFillColor(sf::Color(255, 255, 255));

        window.draw(healthBar);
    }
    else
    {
        sf::Text text(font);
        text.setString(std::to_string((int)deathTimer));
        text.setCharacterSize(16);
        text.setPosition(playerSprite.getPosition());
        window.draw(text);
    }
}