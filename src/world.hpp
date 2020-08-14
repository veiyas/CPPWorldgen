#include <vector>
#include <tuple>
#include <chrono>
#include <iostream>

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>

#include "noisegenerator.hpp"
#include "cube.hpp"

class World
{
public:
	World() = default;
	World(size_t worldLength, size_t worldWidth, size_t maxHeight);

	void render() const;
private:
	NoiseGenerator _ng;

	//Holds all coordinates to be filled with cubes
	std::vector<Cube> _theWorld;

	GLuint _instanceVBO;

	const size_t _WORLDLENGTH;
	const size_t _WORLDWIDTH;
	const size_t _WORLDHEIGHT;
	static constexpr double _noiseFrequency = 75.0;
};