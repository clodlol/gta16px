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
