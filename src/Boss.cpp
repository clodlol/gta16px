#include <iostream>

#include "Boss.h"
#include "Player.h"

Boss::Boss(const sf::Vector2f &spwnPos) : bossTexture{}, bossSprite{bossTexture}, spawnLocation{spwnPos}
{
    moveTimer = moveCooldown;
    immunityTimer = immunityTime;
    health = maxHealth;
    phase = 0;
}

sf::FloatRect Boss::GetBounds() const { return bossSprite.getGlobalBounds(); }

const sf::Sprite &Boss::GetSprite() const { return bossSprite; }

void Boss::TakeDamage(int sourceDamage)
{
    if (immunityTimer <= 0.f && alive)
    {
        int damageTaken = (sourceDamage - sourceDamage * float(defense / 100.f));
        health -= damageTaken;

        // only give immunity frames if damage taken is greater than 10
        if (damageTaken > 10)
        {
            immunityTimer = immunityTime;
        }
        else
        {
            immunityTimer = 0.05f;
        }
    }

    if (health <= int(3 * maxHealth / 4.f))
    {
        phase = 1;
        moveCooldown = 2.f;
        defense = 75;
        speed = 80.f;
    }

    if (health <= int(2 * (float)maxHealth / 4.f))
    {
        immunityTime = 2.f;
        phase = 2;
        moveCooldown = 5.f;
        defense = 90;
        speed = 40.f;
    }

    if (health <= int(1 * maxHealth / 4.f))
    {
        immunityTime = 1.f;
        phase = 3;
        moveCooldown = 0.5f;
        speed = 130.f;
        defense = 95;
    }

    if (health <= 0)
    {
        alive = false;
    }
}

bool Boss::IsAlive() const { return alive; }

void Boss::Load()
{
    if (!bossTexture.loadFromFile("../assets/boss.png"))
    {
        std::cout << "Failed to load boss texture\n";
        return;
    }

    bossSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_WIDTH_BOSS, TILE_HEIGHT_BOSS}));

    bossSprite.setPosition(spawnLocation);

    bossSprite.setOrigin({TILE_WIDTH_BOSS / 2, TILE_HEIGHT_BOSS / 2});
    bossSprite.setScale({32.f / TILE_WIDTH_BOSS, 32.f / TILE_HEIGHT_BOSS});

    gun.Load();
    turret.Load();
    flameThrower.Load();
    rocketLauncher.Load();
}

void Boss::Update(float deltaTime, sf::View &camera, Player &player)
{
    if (!alive)
        return;

    immunityTimer -= deltaTime;
    moveTimer -= deltaTime;

    if (moveTimer <= 0.f && !moving)
    {
        gun.StopFire();
        turret.StopFire();
        rocketLauncher.StopFire();
        flameThrower.StopFire();
        moving = true;

        float randomX = 0.f;
        float randomY = 0.f;

        if (phase == 0 || phase == 1)
        {
            randomX = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().x - camera.getSize().x / 2, camera.getCenter().x - camera.getSize().x / 4) : randomNumGen(camera.getCenter().x + camera.getSize().x / 4, camera.getCenter().x + camera.getSize().x / 2);
            randomY = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().y - camera.getSize().y / 2, camera.getCenter().y - camera.getSize().y / 4) : randomNumGen(camera.getCenter().y + camera.getSize().y / 4, camera.getCenter().y + camera.getSize().y / 2);
        }
        else if (phase == 2)
        {
            randomX = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().x - camera.getSize().x, camera.getCenter().x - camera.getSize().x / 2) : randomNumGen(camera.getCenter().x + camera.getSize().x / 2, camera.getCenter().x + camera.getSize().x);
            randomY = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().y - camera.getSize().y, camera.getCenter().y - camera.getSize().y / 2) : randomNumGen(camera.getCenter().y + camera.getSize().y / 2, camera.getCenter().y + camera.getSize().y);
        }
        else if (phase == 3)
        {
            randomX = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().x - camera.getSize().x / 4, camera.getCenter().x) : randomNumGen(camera.getCenter().x, camera.getCenter().x + camera.getSize().x / 4);
            randomY = randomNumGen(0, 1) > 0.5 ? randomNumGen(camera.getCenter().y - camera.getSize().y / 4, camera.getCenter().y) : randomNumGen(camera.getCenter().y, camera.getCenter().y + camera.getSize().y / 4);
        }

        currentDestination = sf::Vector2f{randomX, randomY};
    }

    if (moving)
    {
        if ((bossSprite.getPosition() - currentDestination).length() > 1)
        {
            bossSprite.move((currentDestination - bossSprite.getPosition()) / ((currentDestination - bossSprite.getPosition()).length()) * speed * deltaTime);
        }
        else
        {
            moving = false;
            moveTimer = moveCooldown;
        }
    }

    if (!moving)
    {
        if (phase != 3)
        {
            gun.Fire(Bullet{gun.GetProjectileTexture(), bossSprite.getPosition(), (player.GetSprite().getPosition() - bossSprite.getPosition()), bulletDamage, bulletVelocity});
        }

        if (phase == 2)
        {
            turret.Fire(Bullet{gun.GetProjectileTexture(), bossSprite.getPosition(), (player.GetSprite().getPosition() - bossSprite.getPosition()), bulletDamage, bulletVelocity});
        }

        if (phase == 1 || phase == 2)
        {
            rocketLauncher.Fire(
                RocketBullet{rocketLauncher.GetProjectileTexture(), bossSprite.getPosition(), (player.GetSprite().getPosition() - bossSprite.getPosition()), rocketDamage, rocketVelocity, rocketTimeLimit, rocketBlastRad, rocketBlastTime});
        }
    }

    if (phase == 3)
    {
        flameThrower.Fire(FlameProjectile{flameThrower.GetProjectileTexture(), bossSprite.getPosition(), (player.GetSprite().getPosition() - bossSprite.getPosition()), flameDmg, flameVel, flameMaxDist});
    }

    gun.Update(deltaTime);
    turret.Update(deltaTime);
    rocketLauncher.Update(deltaTime);
    flameThrower.Update(deltaTime);

    for (Bullet &bullet : gun.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(bullet.GetBounds()))
        {
            player.TakeDamage(gun.GetDamage() + bullet.GetDamage());
            bullet.Erase();
        }
    }

    for (Bullet &bullet : turret.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(bullet.GetBounds()))
        {
            player.TakeDamage(turret.GetDamage() + bullet.GetDamage());
            bullet.Erase();
        }
    }

    for (RocketBullet &rocket : rocketLauncher.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(rocket.GetBounds()))
        {
            if (!rocket.IsExploded())
                rocket.Explode();

            player.TakeDamage(rocketLauncher.GetDamage() + rocket.GetDamage());
        }
    }

    for (FlameProjectile &flame : flameThrower.GetProjectiles())
    {
        if (player.GetBounds().findIntersection(flame.GetBounds()))
        {
            player.TakeDamage(flameThrower.GetDamage() + flame.GetDamage());
            flame.Erase();
        }
    }
}

void Boss::Draw(sf::RenderWindow &window)
{
    if (alive)
    {
        window.draw(bossSprite);
        gun.Draw(window);
        turret.Draw(window);
        rocketLauncher.Draw(window);
        flameThrower.Draw(window);

        sf::RectangleShape healthBar({(20.f * float(float(health) / maxHealth)), 2.5f});
        healthBar.setPosition({bossSprite.getPosition().x - 10.f, bossSprite.getPosition().y + 3.f});
        healthBar.setFillColor(sf::Color(255, 255, 255));
        window.draw(healthBar);
    }
}