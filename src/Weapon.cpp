#include <iostream>

#include "Weapon.h"
#include "InputManager.h"

#define TILE_SIZE 225

Weapon::Weapon(float vel, float rate, int dmg) : bulletVelocity{vel}, fireRate{rate}, damage{dmg}, currentBullet{bulletTexture}
{
    cooldownTimer = 1.f / fireRate;
}

void Weapon::Load()
{
    if (bulletTexture.loadFromFile("../assets/bullet.png"))
    {
        std::cout << "Failed to load bullet texture\n ";
        return;
    }
}

void Weapon::Fire(const sf::Vector2f &origin, const sf::Angle &direction, const std::string &source)
{
    currentBullet.source = source;
    currentBullet.direction = direction;
    currentBullet.origin = origin;
    currentBullet.damage = damage;

    currentBullet.sprite = sf::Sprite(bulletTexture, sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));
    currentBullet.sprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
    currentBullet.sprite.scale({8.f / TILE_SIZE, 8.f / TILE_SIZE});
    currentBullet.sprite.rotate(direction);
    currentBullet.sprite.setPosition(currentBullet.origin);

    firing = true;
}

void Weapon::StopFire()
{
    firing = false;
}

const Bullet &Weapon::GetCurrentBullet()
{
    return currentBullet;
}

void Weapon::Update(float deltaTime, sf::View &camera)
{
    cooldownTimer -= deltaTime;

    if (firing && cooldownTimer <= 0.f)
    {
        cooldownTimer = 1 / fireRate;

        bullets.push_back(currentBullet);
    }

    for (Bullet &bullet : bullets)
    {
        bullet.sprite.move({cos(bullet.direction.asRadians()) * bulletVelocity * deltaTime, sin(bullet.direction.asRadians()) * bulletVelocity * deltaTime});
    }

    // bullets.erase(
    //     std::remove_if(bullets.begin(), bullets.end(),
    //                    [&](Bullet &bullet)
    //                    {
    //                        if (bullet.sprite.getPosition().x <= (camera.getCenter().x + camera.getSize().x / 2) && bullet.sprite.getPosition().x >= (camera.getCenter().x - camera.getSize().x / 2))
    //                        {
    //                            if (bullet.sprite.getPosition().y <= (camera.getCenter().y + camera.getSize().y / 2) && bullet.sprite.getPosition().y >= (camera.getCenter().y - camera.getSize().y / 2))
    //                            {

    //                                return false;
    //                            }
    //                        }

    //                        return true;
    //                    }),
    //     bullets.end());
    // what the actual fuck is this
}

void Weapon::Draw(sf::RenderWindow &window)
{
    for (const Bullet &bullet : bullets)
        window.draw(bullet.sprite);
}