#include <vector>
#include <tuple>
#include <chrono>
#include <iostream>
#include <algorithm>

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
	void recreateWorld(size_t worldLength, size_t worldWidth, size_t maxHeight);
private:
	NoiseGenerator _ng;
	std::vector<Cube> _theWorld;
	GLuint _instanceVBO;

	static constexpr double _noiseFrequency = 75.0;

	void createWorld(size_t worldLength, size_t worldWidth, size_t maxHeight, bool replace = false);
};