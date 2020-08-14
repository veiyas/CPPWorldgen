#include "world.hpp"

World::World()
	:_ng{std::chrono::high_resolution_clock::now().time_since_epoch().count()} //Init with random seed
{
	_theWorld.reserve(_WORLDLENGTH * _WORLDWIDTH);

	int blockHeight = -1;
	double xSeed, ySeed = -1.0;
	for (size_t i = 0; i < _WORLDLENGTH; i++)
	{
		for (size_t j = 0; j < _WORLDWIDTH; j++)
		{
			xSeed = static_cast<double>(i) / _WORLDLENGTH;
			ySeed = static_cast<double>(j) / _WORLDWIDTH;
			blockHeight = std::floor(_ng.perlinNoise(xSeed, ySeed) * _WORLDHEIGHT);
			_theWorld.emplace_back(i, blockHeight, j);
		}
	}

	glGenBuffers(1, &_instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Cube::_translations.size(), Cube::_translations.data(), GL_STATIC_DRAW);

	for (size_t i = 0; i < Cube::getNumObjects(); i++)
	{
		GLuint tempVAO = _theWorld[i].getVAO();
		glBindVertexArray(tempVAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0); // offsets
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);
	}
}

void World::render() const
{
	//for (const auto& cube : _theWorld)
	//{
	//	cube.render();
	//}

	glBindVertexArray(_theWorld[0].getVAO());
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, (GLsizei)Cube::getNumObjects());
	glBindVertexArray(0);
}



