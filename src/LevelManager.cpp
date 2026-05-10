#include "LevelManager.h"
#include "Player.h"

int LevelManager::GetCurrentWantedLevel() const { return wantedLevel; }

void LevelManager::Load()
{
    if (!upStarTex.loadFromFile("../assets/upstar.png"))
    {
        std::cout << "Error while loading UpStar texture\n";
        return;
    }

    if (!downStarTex.loadFromFile("../assets/downstar.png"))
    {
        std::cout << "Error while loading DownStar texture\n";
        return;
    }

    std::vector<int> pickups = readCsvFile("../world/pickups.csv");

    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            Pickup currentPickup(upStarTex);

            if (pickups[i * j] == 1)
            {
                currentPickup.sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_UPSTAR, TILE_SIZE_UPSTAR}));
                currentPickup.sprite.setPosition({float(i * 16), float(j * 16)});
                currentPickup.sprite.setOrigin({float(TILE_SIZE_UPSTAR / 2), float(TILE_SIZE_UPSTAR / 2)});
                currentPickup.sprite.scale({8.f / TILE_SIZE_UPSTAR, 8.f / TILE_SIZE_UPSTAR});
                upStars.push_back(currentPickup);
            }

            if (pickups[i * j] == 2)
            {
                currentPickup.sprite.setTexture(downStarTex, true);
                currentPickup.sprite.setTextureRect(sf::IntRect({0, 0}, {TILE_SIZE_DOWNSTAR, TILE_SIZE_DOWNSTAR}));
                currentPickup.sprite.setPosition({float(i * 16), float(j * 16)});
                currentPickup.sprite.setOrigin({float(TILE_SIZE_DOWNSTAR / 2), float(TILE_SIZE_DOWNSTAR / 2)});
                currentPickup.sprite.scale({8.f / TILE_SIZE_DOWNSTAR, 8.f / TILE_SIZE_DOWNSTAR});

                downStars.push_back(currentPickup);
            }
        }
    }
}

void LevelManager::Update(float deltaTime, Player &player)
{
    for (Pickup &upStar : upStars)
    {
        if (player.GetBounds().findIntersection(upStar.GetBounds()) && !upStar.pickedUp)
        {
            upStar.pickedUp = true;
            wantedLevel++;
        }
    }

    if (wantedLevel > 0 && wantedLevel < 5)
    {
        for (Pickup &downStar : downStars)
        {
            if (player.GetBounds().findIntersection(downStar.GetBounds()) && !downStar.pickedUp)
            {
                downStar.pickedUp = true;
                wantedLevel--;
            }
        }
    }
}

void LevelManager::Draw(sf::RenderWindow &window)
{
    for (const Pickup &upStar : upStars)
    {
        if (!upStar.pickedUp)
            window.draw(upStar.GetSprite());
    }

    if (wantedLevel > 0 && wantedLevel < 5)
    {
        for (const Pickup &downStar : downStars)
        {
            if (!downStar.pickedUp)
                window.draw(downStar.GetSprite());
        }
    }
}