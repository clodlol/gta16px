#include <iostream>

#include "Weapon.h"
#include "InputManager.h"

#define TILE_SIZE 225

Weapon::Weapon(float vel, float rate) : bulletVelocity{vel}, fireRate{rate}, cooldownTimer{1 / fireRate} {}

void Weapon::Load()
{
    if (bulletTexture.loadFromFile("../assets/bullet.png"))
    {
        std::cout << "Failed to load bullet texture\n ";
        return;
    }
}

void Weapon::Fire()
{
    firing = true;
}

void Weapon::StopFire()
{
    firing = false;
}

void Weapon::Update(float deltaTime, InputManager &input, sf::View &camera)
{
    cooldownTimer -= deltaTime;

    if (firing && cooldownTimer <= 0.f)
    {
        cooldownTimer = 1 / fireRate;

        Bullet newBullet(bulletTexture);
        newBullet.direction = input.GetMousePosition();

        newBullet.sprite = sf::Sprite(bulletTexture, sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));
        newBullet.sprite.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
        newBullet.sprite.scale({8.f / TILE_SIZE, 8.f / TILE_SIZE});
        newBullet.sprite.rotate(newBullet.direction.angle());

        newBullet.origin = camera.getCenter();

        newBullet.sprite.setPosition(newBullet.origin);
        newBullet.direction = input.GetMousePosition();

        bullets.push_back(newBullet);
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [&](Bullet &bullet)
                       {
                           if (bullet.sprite.getPosition().x < (camera.getCenter().x + camera.getSize().x / 2) && bullet.sprite.getPosition().x > (camera.getCenter().x - camera.getSize().x / 2))
                           {
                               if (bullet.sprite.getPosition().y < (camera.getCenter().y + camera.getSize().y / 2) && bullet.sprite.getPosition().y > (camera.getCenter().y - camera.getSize().y / 2))
                               {
                                   bullet.sprite.move({cos(bullet.direction.angle().asRadians()) * bulletVelocity * deltaTime, sin(bullet.direction.angle().asRadians()) * bulletVelocity * deltaTime});
                               }

                               return false;
                           }

                           return true;
                       }),
        bullets.end());
    // what the actual fuck is this
}

void Weapon::Draw(sf::RenderWindow &window)
{
    for (const Bullet &bullet : bullets)
        window.draw(bullet.sprite);
}