#include "MobSpawner.h"

void MobSpawner::Update(float deltaTime, int wantedLevel)
{
    officers.erase(std::remove_if(officers.begin(), officers.end(), [](Officer &officer)
                                  { return !officer.IsAlive(); }),
                   officers.end());
}

void MobSpawner::Draw(sf::RenderWindow &window)
{
}