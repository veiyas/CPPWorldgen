#include "noisegenerator.hpp"

//NoiseGenerator::NoiseGenerator()
//{
//        _permutation = {
//            151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
//            8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
//            35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
//            134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
//            55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
//            18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
//            250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
//            189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
//            43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
//            97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
//            107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
//            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
//
//        _permutation.insert(_permutation.end(), _permutation.begin(), _permutation.end());
//}

NoiseGenerator::NoiseGenerator(unsigned seed)
{
    //Randomly generated seed to initialize random engine
    std::default_random_engine engine(seed);

    _permutation.resize(256);

    std::iota(_permutation.begin(), _permutation.end(), 0);

    std::shuffle(_permutation.begin(), _permutation.end(), engine);

    _permutation.insert(_permutation.begin(), _permutation.end(), _permutation.end());
}

void NoiseGenerator::updateSeed(unsigned seed)
{
	//Randomly generated seed to initialize random engine
	std::default_random_engine engine(seed);
	std::shuffle(_permutation.begin(), _permutation.end(), engine);
}


double NoiseGenerator::lerp(double w, double a, double b)
{
    return (1.0 - w)*a + w*b;
}

double NoiseGenerator::grad(int hs, double x, double y, double z)
{
    int h = hs & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double NoiseGenerator::fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double NoiseGenerator::perlinNoise(double x, double y)
{
	return perlinNoise(x, y, 0.0);
}

double NoiseGenerator::perlinNoise(double x, double z, double y)
{
// Find the unit cube that contains the point
	int X = (int) floor(x) & 255;
	int Y = (int) floor(y) & 255;
	int Z = (int) floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	size_t maxSize = _permutation.size();

	// Hash coordinates of the 8 cube corners
	int A = (_permutation[X % maxSize] + Y) % maxSize - 1;
	int AA = (_permutation[A % maxSize] + Z) % maxSize - 1;
	int AB = (_permutation[(A + 1) % maxSize] + Z) % maxSize - 1;
	int B = (_permutation[(X + 1) % maxSize] + Y) % maxSize - 1;
	int BA = (_permutation[B % maxSize] + Z) % maxSize - 1;
	int BB = (_permutation[(B + 1) % maxSize] + Z) % maxSize - 1;

	// Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(_permutation[AA % maxSize], x, y, z), grad(_permutation[BA % maxSize], x - 1, y, z)), lerp(u, grad(_permutation[AB % maxSize], x, y - 1, z), grad(_permutation[BB % maxSize], x - 1, y - 1, z))), lerp(v, lerp(u, grad(_permutation[(AA + 1) % maxSize], x, y, z - 1), grad(_permutation[(BA + 1) % maxSize], x - 1, y, z - 1)), lerp(u, grad(_permutation[(AB + 1) % maxSize], x, y - 1, z - 1), grad(_permutation[(BB + 1) % maxSize], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;
}



