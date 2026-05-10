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
    Melee(float rate, int dmg) : meleeTexture{}, meleeSprite{meleeTexture}, swingRate(rate), damage(dmg)
    {
        angleSwung = -PI / 2;
        cooldownTimer = 1.f / swingRate;
    }

    sf::FloatRect GetBounds() const override { return meleeSprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return meleeSprite; }

    void Swing(const Collidable &player)
    {
        angleSwung = -PI / 2;
        meleeSprite.setPosition({player.GetSprite().getPosition().x, player.GetSprite().getPosition().y + player.GetBounds().size.y / 2});
        swinging = true;
    }

    const sf::Texture &GetTexture() const { return meleeTexture; }
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
            angleSwung += (deltaTime * 10);
            if (angleSwung >= PI / 2)
            {
                if (swinging)
                    swinging = false;

                angleSwung = -PI / 2;
            }

            meleeSprite.setPosition({player.GetSprite().getPosition().x + 16.f * cos(angleSwung) * deltaTime, player.GetSprite().getPosition().y + 16.f * sin(angleSwung) * deltaTime});
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

    float swingRate = 1.f; // swings per second
    int damage = 0;

    bool swinging = false;
    float angleSwung;
    float cooldownTimer;
};