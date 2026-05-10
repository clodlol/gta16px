#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>

float randomNumGen(float min, float max);
std::vector<int> readCsvFile(std::filesystem::path path);