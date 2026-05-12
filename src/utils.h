#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <SFML/Graphics.hpp>

float randomNumGen(float min, float max);
std::vector<int> readCsvFile(std::filesystem::path path);
sf::Vector2f generateRandomSpawn(const sf::View &camera);