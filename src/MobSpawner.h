#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Officer.h"
#include "Tank.h"
#include "utils.h"
#include "Player.h"

class Player;
class Tank;
class Officer;

class MobSpawner
{
public:
    ~MobSpawner();

    std::vector<Officer *> &GetOfficers();
    std::vector<Tank *> &GetTanks();

    void Update(float deltaTime, int wantedLevel, sf::View &camera, Player &player);
    void Draw(sf::RenderWindow &window);

private:
    int officersSpawnCount = 0;
    int tanksSpawnCount = 0;

    std::vector<Officer *> officers;
    std::vector<Tank *> tanks;
};