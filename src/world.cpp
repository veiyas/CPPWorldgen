#include "world.hpp"

World::World(size_t worldLength, size_t worldWidth, size_t maxHeight)
	:_ng{std::chrono::high_resolution_clock::now().time_since_epoch().count()}
{
	createWorld(worldLength, worldWidth, maxHeight);
}

void World::render() const
{
	//for (const auto& cube : _theWorld)
	//{
	//	cube.render();
	//}

	glBindVertexArray(_theWorld[0]->getVAO());
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, (GLsizei)_theWorld.size());
	glBindVertexArray(0);
}

void World::createWorld(size_t worldLength, size_t worldWidth, size_t maxHeight, bool replace)
{
	_theWorld.clear();
	_theWorld.reserve(worldLength * worldWidth);
	_ng.updateSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	int blockHeight = -1;
	double xSeed, ySeed = -1.0;
	for (size_t i = 0; i < worldLength; i++)
	{
		for (size_t j = 0; j < worldWidth; j++)
		{
			xSeed = static_cast<double>(i) / _noiseFrequency;
			ySeed = static_cast<double>(j) / _noiseFrequency;
			blockHeight = std::floor(_ng.perlinNoise(xSeed, ySeed) * maxHeight);
			_theWorld.emplace_back(std::unique_ptr<Cube>( new Cube(i, blockHeight, j)));
		}
	}

	std::cout << "Reserved size: " << _theWorld.capacity() << "\n";

	if (!replace) { glGenBuffers(1, &_instanceVBO); }
	
	glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * Cube::_translations.size(), Cube::_translations.data(), GL_STATIC_DRAW);

	//This big ugly chunk of code is necessary to use instance rendering
	//We're sending each instance array as a mat4 which consumes 4 consecutive layout locations
	for (size_t i = 0; i < _theWorld.size(); i++)
	{
		GLuint tempVAO = _theWorld[i]->getVAO();
		glBindVertexArray(tempVAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void World::recreateWorld(size_t worldLength, size_t worldWidth, size_t maxHeight)
{
	Cube::_translations.clear();
	createWorld(worldLength, worldWidth, maxHeight, true);
}



