
#include "utils.h"

float randomNumGen(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

std::vector<int> readCsvFile(std::filesystem::path path)
{
    std::vector<int> v;
    std::ifstream fin(path);
    if (!fin.is_open())
    {
        std::cout << "Could not open pickups.csv file\n";
        return v;
    }
    int val;

    while (fin >> val)
    {
        v.push_back(val);

        if (fin.peek() == ',')
            fin.ignore();
    }

    return v;
}

sf::Vector2f generateRandomSpawn(const sf::View &camera)
{
    float randomSpawnX = randomNumGen(0.f, 1.f) > 0.5f ? ((camera.getCenter().x - camera.getSize().x / 2) - randomNumGen(0.f, 100.f)) : ((camera.getCenter().x + camera.getSize().x / 2) + randomNumGen(0.f, 100.f));
    float randomSpawnY = randomNumGen(0.f, 1.f) > 0.5f ? ((camera.getCenter().y + camera.getSize().y / 2) + randomNumGen(0.f, 100.f)) : ((camera.getCenter().y - camera.getSize().y / 2) - randomNumGen(0.f, 100.f));

    randomSpawnX = std::clamp(randomSpawnX, 0.f, 1600.f);
    randomSpawnY = std::clamp(randomSpawnY, 0.f, 1600.f);

    return sf::Vector2f{randomSpawnX, randomSpawnY};
}