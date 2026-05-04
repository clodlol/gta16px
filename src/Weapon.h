#pragma once

#include <SFML/Graphics.hpp>

#include "InputManager.h"

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
    std::vector<sf::Sprite> bullets;

    bool firing;
    float bulletVelocity;
    float fireRate; // bullets per second
};