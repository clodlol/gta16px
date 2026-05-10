#include "MobSpawner.h"
#include "Player.h"
#include "Tank.h"
#include "Officer.h"

sf::Vector2f generateRandomSpawn(const sf::View &camera)
{
    float randomSpawnX = randomNumGen(0.f, 1.f) > 0.5f ? ((camera.getCenter().x - camera.getSize().x / 2) - randomNumGen(0.f, 100.f)) : ((camera.getCenter().x + camera.getSize().x / 2) + randomNumGen(0.f, 100.f));
    float randomSpawnY = randomNumGen(0.f, 1.f) > 0.5f ? ((camera.getCenter().y + camera.getSize().y / 2) + randomNumGen(0.f, 100.f)) : ((camera.getCenter().y - camera.getSize().y / 2) - randomNumGen(0.f, 100.f));

    randomSpawnX = std::clamp(randomSpawnX, 0.f, 1600.f);
    randomSpawnY = std::clamp(randomSpawnY, 0.f, 1600.f);

    return sf::Vector2f{randomSpawnX, randomSpawnY};
}

MobSpawner::~MobSpawner()
{
    for (Officer *&officer : officers)
    {
        delete officer;
    }

    for (Tank *&tank : tanks)
    {
        delete tank;
    }
}

std::vector<Officer *> &MobSpawner::GetOfficers()
{
    return officers;
}

std::vector<Tank *> &MobSpawner::GetTanks()
{
    return tanks;
}

void MobSpawner::Update(float deltaTime, int wantedLevel, sf::View &camera, Player &player)
{
    if (wantedLevel == 0 || wantedLevel == 5 || wantedLevel == 9)
    {
        officersSpawnCount = 0;
        tanksSpawnCount = 0;
    }
    else if (wantedLevel == 1)
    {
        officersSpawnCount = 2;
        tanksSpawnCount = 0;
    }
    else if (wantedLevel == 2 || wantedLevel == 6)
    {
        officersSpawnCount = 2;
        tanksSpawnCount = 1;
    }
    else if (wantedLevel == 3 || wantedLevel == 7)
    {
        officersSpawnCount = 3;
        tanksSpawnCount = 2;
    }
    else if (wantedLevel == 8 || wantedLevel == 4)
    {
        officersSpawnCount = 5;
        tanksSpawnCount = 5;
    }

    officers.erase(std::remove_if(officers.begin(), officers.end(), [](Officer *&officer)
                                  {
        if(!officer->IsAlive())
        {
            delete officer;
            return true;
        }
        return false; }),
                   officers.end());

    tanks.erase(std::remove_if(tanks.begin(), tanks.end(), [](Tank *&tank)
                               {
        if(!tank->IsAlive())
        {
            delete tank;
            return true;
        } 
        return false; }),
                tanks.end());

    int officersToSpawn = officersSpawnCount - officers.size();
    for (int i = 0; i < officersToSpawn; ++i)
    {
        Officer *newOfficer = new Officer({generateRandomSpawn(camera)});
        officers.push_back(newOfficer);
        newOfficer->Load();
    }

    int tanksToSpawn = tanksSpawnCount - tanks.size();
    for (int i = 0; i < tanksToSpawn; ++i)
    {
        Tank *newTank = new Tank({generateRandomSpawn(camera)});
        tanks.push_back(newTank);
        newTank->Load();
    }

    for (Officer *&officer : officers)
    {
        officer->Update(deltaTime, camera, player);
    }

    for (Tank *&tank : tanks)
    {
        tank->Update(deltaTime, camera, player);
    }
}

void MobSpawner::Draw(sf::RenderWindow &window)
{
    for (Officer *&officer : officers)
    {
        officer->Draw(window);
    }

    for (Tank *&tank : tanks)
    {
        tank->Draw(window);
    }
}