#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Collidable.h"
#include "Player.h"
#include "utils.h"

#define TILE_SIZE_UPSTAR 79
#define TILE_SIZE_DOWNSTAR 324

struct Pickup : public Collidable
{
    Pickup(const sf::Texture &tex) : sprite(tex) {}

    sf::Sprite sprite;
    bool pickedUp = false;

    sf::FloatRect GetBounds() const override { return sprite.getGlobalBounds(); }
    const sf::Sprite &GetSprite() const override { return sprite; }
};

class LevelManager
{
public:
    int GetCurrentWantedLevel() const;

    void Load();
    void Update(float deltaTime, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    int wantedLevel = 0;

    sf::Texture upStarTex{};
    sf::Texture downStarTex{};

    std::vector<Pickup> upStars;
    std::vector<Pickup> downStars;
};