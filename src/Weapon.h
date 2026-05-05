#pragma once

#include <SFML/Graphics.hpp>

#include "InputManager.h"

struct Bullet
{
    Bullet(const sf::Texture &tex) : sprite(tex) {}

    sf::Sprite sprite;
    sf::Vector2f origin;
    sf::Vector2f direction;
};

class Weapon
{
public:
    Weapon(float vel = 75.0f, float rate = 1.f);

    void Fire();
    void StopFire();
    void Load();
    void Update(float deltaTime, InputManager &input, sf::View &camera);
    void Draw(sf::RenderWindow &window);

private:
    sf::Texture bulletTexture;
    std::vector<Bullet> bullets;

    const float bulletVelocity;
    const float fireRate; // bullets per second

    bool firing;
    float cooldownTimer;
};