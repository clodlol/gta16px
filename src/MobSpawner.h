#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Officer.h"
#include "Tank.h"

class MobSpawner
{
public:
    void Update(float deltaTime, int wantedLevel);
    void Draw(sf::RenderWindow &window);

private:
    std::vector<Officer> officers;
    std::vector<Tank> tanks;
};