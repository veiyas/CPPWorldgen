#pragma once
#include <numeric>
#include <iterator>
#include <random>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <vector>

class NoiseGenerator
{
public:
    NoiseGenerator();
    NoiseGenerator(unsigned seed);

    double perlinNoise(double x, double y);
    double perlinNoise(double x, double y, double z);

private:
//Data members
    std::vector<int> _permutation;

//Seed choice
    bool seeded;

//Methods
    double fade(double t);
    double lerp(double w, double a, double b);
    double grad(int hs, double x, double y, double z);
};
