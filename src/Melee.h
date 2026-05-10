#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Collidable.h"
#include "Player.h"

#define TILE_SIZE_SWORD 369
#define PI 3.14159

class Player;

class Melee : Collidable
{
public:
    Melee(float speed, float rate, int dmg) : meleeTexture{}, meleeSprite{meleeTexture}, damage(dmg), swingSpeed(speed)
    {
        angleSwung = -PI / 2.f;
    }

    sf::FloatRect GetBounds() const override { return meleeSprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return meleeSprite; }

    void Swing(const Collidable &player, sf::Angle dir)
    {
        direction = dir;

        angleSwung = direction.asRadians() - PI / 2.f; // start pi/4 behind current direction
        meleeSprite.setPosition({player.GetSprite().getPosition().x, player.GetSprite().getPosition().y});
        swinging = true;
    }

    bool IsSwinging() const { return swinging; }
    int GetDamage() const { return damage; }

    void Load()
    {
        if (!meleeTexture.loadFromFile("../assets/sword.png"))
        {
            std::cout << "Error while loading sword texture\n";
            return;
        }

        meleeSprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_SWORD, TILE_SIZE_SWORD}));
        meleeSprite.setOrigin({float(TILE_SIZE_SWORD / 2), float(TILE_SIZE_SWORD)});
        meleeSprite.setScale({float(16.f / TILE_SIZE_SWORD), float(16.f / TILE_SIZE_SWORD)});
        meleeSprite.setPosition({0.f, 0.f});
    }

    void Update(float deltaTime, Collidable &player)
    {
        if (swinging)
        {
            angleSwung += (deltaTime * swingSpeed);
            if (angleSwung >= direction.asRadians() + PI / 2.f)
            {
                swinging = false;

                angleSwung = direction.asRadians() - PI / 2.f;
            }

            meleeSprite.setRotation(sf::radians(angleSwung));

            meleeSprite.setPosition({player.GetSprite().getPosition().x + 4.f * cos(angleSwung), player.GetSprite().getPosition().y + 4.f * sin(angleSwung)});
        }
    }

    void Draw(sf::RenderWindow &window)
    {
        if (swinging)
        {
            window.draw(meleeSprite);
        }
    }

private:
    sf::Texture meleeTexture{};
    sf::Sprite meleeSprite;

    sf::Angle direction{};

    float swingSpeed = 10.f;
    int damage = 50;

    bool swinging = false;
    float angleSwung;
};