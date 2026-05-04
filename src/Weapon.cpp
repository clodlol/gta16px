#include <iostream>

#include "Weapon.h"
#include "InputManager.h"

#define TILE_SIZE 225

Weapon::Weapon(float vel, float rate) : bulletVelocity{vel}, fireRate{rate} {}

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
    if (firing)
    {
        sf::Sprite newBullet = sf::Sprite(bulletTexture, sf::IntRect({0, 0}, {TILE_SIZE, TILE_SIZE}));
        newBullet.setOrigin({TILE_SIZE / 2, TILE_SIZE / 2});
        newBullet.scale({8.f / TILE_SIZE, 8.f / TILE_SIZE});
        newBullet.setPosition(camera.getCenter());
        bullets.push_back(newBullet);
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [&](sf::Sprite &bullet)
                       {
                           if (bullet.getPosition().x < (camera.getCenter().x + camera.getSize().x / 2) && bullet.getPosition().x > (camera.getCenter().x - camera.getSize().x / 2))
                           {
                               bullet.move({bulletVelocity * deltaTime, 0});
                               return false;
                           }

                           if (bullet.getPosition().y < (camera.getCenter().y + camera.getSize().y / 2) && bullet.getPosition().y > (camera.getCenter().y - camera.getSize().y / 2))
                           {
                               bullet.move({bulletVelocity * deltaTime, 0});
                               return false;
                           }

                           return true;
                       }),
        bullets.end());
    // what the actual fuck is this
}

void Weapon::Draw(sf::RenderWindow &window)
{
    for (const sf::Sprite &bullet : bullets)
        window.draw(bullet);
}