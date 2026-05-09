#pragma once

#include <SFML/Graphics.hpp>

class MobSpawner
{
public:
    MobSpawner();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);

private:
    int wantedLevel = 1;
};