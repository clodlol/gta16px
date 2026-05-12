#include "MobSpawner.h"
#include "Player.h"
#include "Tank.h"
#include "Officer.h"
#include "Boss.h"

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

    for (Boss *&boss : bosses)
    {
        delete boss;
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

std::vector<Boss *> &MobSpawner::GetBosses()
{
    return bosses;
}

void MobSpawner::Update(float deltaTime, int wantedLevel, sf::View &camera, Player &player)
{
    if (wantedLevel == 0 || wantedLevel == 5 || wantedLevel == 9)
    {
        if (wantedLevel == 5)
        {
            bossesSpawnCount = 1;
        }
        else if (wantedLevel == 9)
        {
            bossesSpawnCount = 2;
        }

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

    bosses.erase(std::remove_if(bosses.begin(), bosses.end(), [](Boss *&boss)
                                {
        if(!boss->IsAlive())
        {
            delete boss;
            return true;
        } 
        return false; }),
                 bosses.end());

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

    int bossesToSpawn = bossesSpawnCount - bosses.size();
    for (int i = 0; i < bossesToSpawn; ++i)
    {
        Boss *newBoss = new Boss({generateRandomSpawn(camera)});
        bosses.push_back(newBoss);
        newBoss->Load();
    }

    for (Officer *&officer : officers)
    {
        officer->Update(deltaTime, camera, player);
    }

    for (Tank *&tank : tanks)
    {
        tank->Update(deltaTime, camera, player);
    }

    for (Boss *&boss : bosses)
    {
        boss->Update(deltaTime, camera, player);
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

    for (Boss *&boss : bosses)
    {
        boss->Draw(window);
    }
}