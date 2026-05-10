#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Officer.h"
#include "Tank.h"

class MobSpawner
{
public:
    MobSpawner();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);

private:
    int wantedLevel;

    std::vector<Officer> officers;
    std::vector<Tank> tanks;
};