#pragma once

#include <SFML/Graphics.hpp>

class Collidable
{
public:
    virtual ~Collidable() = default;
    virtual sf::FloatRect GetBounds() const = 0;
    virtual const sf::Sprite &GetSprite() const = 0;
};