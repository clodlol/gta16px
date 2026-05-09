#pragma once

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    virtual ~Projectile() = default;
    virtual sf::FloatRect GetBounds() const = 0;
    virtual const sf::Sprite &GetSprite() const = 0;
    virtual const sf::Vector2f &GetOrigin() const = 0;
    virtual const sf::Angle &GetDirection() const = 0;
    virtual int GetDamage() const = 0;

    virtual void MoveSprite(const sf::Vector2f &difference) = 0;
};