/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RandomGenerator
*/

#include "RandomGenerator.hpp"
#include <random>

float eng::RandomGenerator::generate(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int eng::RandomGenerator::generate(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}
